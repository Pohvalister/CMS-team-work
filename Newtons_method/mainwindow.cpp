#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "newtons_method.h"
#include "q_tree.h"

#include <QColor>
#include <iostream>
#include <cassert>

using namespace std;

QCPLayer *colorMapLayer;
QCPLayer *itemLineLayer;

void MainWindow::draw_path(double x, double y) {
    vector<pair<double, double>> path = calculate_path(x, y);
    QCustomPlot *plot = ui->customPlot;
    
    shared_ptr<vector<QCPItemLine*>> res = make_shared<vector<QCPItemLine*>>();
    
    for (size_t w = 1; w < path.size(); w++) {
        QCPItemLine *arrow = new QCPItemLine(plot);
        arrow->start->setCoords(path[w - 1].first, path[w - 1].second);
        arrow->end->setCoords(path[w].first, path[w].second);
        arrow->setHead(QCPLineEnding::esLineArrow);
        arrow->setTail(QCPLineEnding::esDisc);
        arrow->setLayer(itemLineLayer);
        
        arrow->setSelectable(true);
        
        QPen pen;
        pen.setColor(QColor(Qt::darkMagenta));
        pen.setWidthF(2);
        arrow->setSelectedPen(pen);
        
        res->push_back(arrow);
        paths_of_line[arrow] = res;
    }
    plot->replot();
}

MainWindow::MainWindow(QWidget *parent) :
  QMainWindow(parent),
  ui(new Ui::MainWindow)
{
  srand(QDateTime::currentDateTime().toTime_t());
  ui->setupUi(this);
  
  ui->customPlot->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom | QCP::iSelectAxes |
                                  QCP::iSelectLegend | QCP::iSelectPlottables | QCP::iSelectItems);
  ui->customPlot->xAxis->setRange(-4, 4);
  ui->customPlot->yAxis->setRange(-4, 4);
  ui->customPlot->axisRect()->setupFullAxesBox();
  
  ui->customPlot->plotLayout()->insertRow(0);
  QCPTextElement *title = new QCPTextElement(ui->customPlot, "Newton's method", QFont("sans", 17, QFont::Bold));
  ui->customPlot->plotLayout()->addElement(0, 0, title);
  
  ui->customPlot->xAxis->setLabel("x Axis");
  ui->customPlot->yAxis->setLabel("y Axis");
  
    QCustomPlot *plot = ui->customPlot;
    
    bool res;
    res = plot->addLayer("colorMapLayer");
    if (!res) {
        throw runtime_error("Unable to create colorMapLayer");
    }
    colorMapLayer = plot->layer("colorMapLayer");
    
    res = plot->addLayer("itemLineLayer", colorMapLayer);
    if (!res) {
        throw runtime_error("Unable to create itemLineLayer");
    }
    itemLineLayer = plot->layer("itemLineLayer");
    
    q_tree::init(plot, -4, -4, 4, 4);
    q_tree::update_tree(plot);
  
  // connect slot that ties some axis selections together (especially opposite axes):
  connect(ui->customPlot, SIGNAL(selectionChangedByUser()), this, SLOT(selectionChanged()));
  // connect slots that takes care that when an axis is selected, only that direction can be dragged and zoomed:
  connect(ui->customPlot, SIGNAL(mousePress(QMouseEvent*)), this, SLOT(mousePress()));
  connect(ui->customPlot, SIGNAL(mouseWheel(QWheelEvent*)), this, SLOT(mouseWheel()));
  
  // make bottom and left axes transfer their ranges to top and right axes:
  connect(ui->customPlot->xAxis, SIGNAL(rangeChanged(QCPRange)), ui->customPlot->xAxis2, SLOT(setRange(QCPRange)));
  connect(ui->customPlot->yAxis, SIGNAL(rangeChanged(QCPRange)), ui->customPlot->yAxis2, SLOT(setRange(QCPRange)));
  
  // connect some interaction slots:
  connect(ui->customPlot, SIGNAL(axisDoubleClick(QCPAxis*,QCPAxis::SelectablePart,QMouseEvent*)), this, SLOT(axisLabelDoubleClick(QCPAxis*,QCPAxis::SelectablePart)));
  connect(title, SIGNAL(doubleClicked(QMouseEvent*)), this, SLOT(titleDoubleClick(QMouseEvent*)));
  
  connect(ui->pushButton, SIGNAL(clicked(bool)), this, SLOT(clickOnButton()));
  
  // setup policy and connect slot for context menu popup:
  ui->customPlot->setContextMenuPolicy(Qt::CustomContextMenu);
  connect(ui->customPlot, SIGNAL(customContextMenuRequested(QPoint)), this, SLOT(contextMenuRequest(QPoint)));
}

MainWindow::~MainWindow() {
    delete ui;
}

void MainWindow::titleDoubleClick(QMouseEvent* event)
{
  Q_UNUSED(event)
  if (QCPTextElement *title = qobject_cast<QCPTextElement*>(sender()))
  {
    // Set the plot title by double clicking on it
    bool ok;
    QString newTitle = QInputDialog::getText(this, "QCustomPlot example", "New plot title:", QLineEdit::Normal, title->text(), &ok);
    if (ok)
    {
      title->setText(newTitle);
      ui->customPlot->replot();
    }
  }
}

void MainWindow::axisLabelDoubleClick(QCPAxis *axis, QCPAxis::SelectablePart part)
{
  // Set an axis label by double clicking on it
  if (part == QCPAxis::spAxisLabel) // only react when the actual axis label is clicked, not tick label or axis backbone
  {
    bool ok;
    QString newLabel = QInputDialog::getText(this, "QCustomPlot example", "New axis label:", QLineEdit::Normal, axis->label(), &ok);
    if (ok)
    {
      axis->setLabel(newLabel);
      ui->customPlot->replot();
    }
  }
}

set<shared_ptr<vector<QCPItemLine*>>> MainWindow::getSelectedPaths() {
    set<shared_ptr<vector<QCPItemLine*>>> paths;
    auto selectedItems = ui->customPlot->selectedItems();
    for (auto w : selectedItems) {
        if (paths_of_line.count(w) > 0) {
            paths.insert(paths_of_line[w]);
        }
    }
    return paths;
}

void MainWindow::selectionChanged()
{
  /*
   normally, axis base line, axis tick labels and axis labels are selectable separately, but we want
   the user only to be able to select the axis as a whole, so we tie the selected states of the tick labels
   and the axis base line together. However, the axis label shall be selectable individually.
   
   The selection state of the left and right axes shall be synchronized as well as the state of the
   bottom and top axes.
  */
  
  // make top and bottom axes be selected synchronously, and handle axis and tick labels as one selectable object:
  if (ui->customPlot->xAxis->selectedParts().testFlag(QCPAxis::spAxis) || ui->customPlot->xAxis->selectedParts().testFlag(QCPAxis::spTickLabels) ||
      ui->customPlot->xAxis2->selectedParts().testFlag(QCPAxis::spAxis) || ui->customPlot->xAxis2->selectedParts().testFlag(QCPAxis::spTickLabels))
  {
    ui->customPlot->xAxis2->setSelectedParts(QCPAxis::spAxis|QCPAxis::spTickLabels);
    ui->customPlot->xAxis->setSelectedParts(QCPAxis::spAxis|QCPAxis::spTickLabels);
  }
  // make left and right axes be selected synchronously, and handle axis and tick labels as one selectable object:
  if (ui->customPlot->yAxis->selectedParts().testFlag(QCPAxis::spAxis) || ui->customPlot->yAxis->selectedParts().testFlag(QCPAxis::spTickLabels) ||
      ui->customPlot->yAxis2->selectedParts().testFlag(QCPAxis::spAxis) || ui->customPlot->yAxis2->selectedParts().testFlag(QCPAxis::spTickLabels))
  {
    ui->customPlot->yAxis2->setSelectedParts(QCPAxis::spAxis|QCPAxis::spTickLabels);
    ui->customPlot->yAxis->setSelectedParts(QCPAxis::spAxis|QCPAxis::spTickLabels);
  }
  
    for (auto path : getSelectedPaths()) {
        for (QCPItemLine* line : *path) {
            line->setSelected(true);
        }
    }
}

void MainWindow::mousePress()
{
  // if an axis is selected, only allow the direction of that axis to be dragged
  // if no axis is selected, both directions may be dragged
    
    q_tree::update_tree(ui->customPlot);
    
  if (ui->customPlot->xAxis->selectedParts().testFlag(QCPAxis::spAxis))
    ui->customPlot->axisRect()->setRangeDrag(ui->customPlot->xAxis->orientation());
  else if (ui->customPlot->yAxis->selectedParts().testFlag(QCPAxis::spAxis))
    ui->customPlot->axisRect()->setRangeDrag(ui->customPlot->yAxis->orientation());
  else
    ui->customPlot->axisRect()->setRangeDrag(Qt::Horizontal|Qt::Vertical);
}

void MainWindow::mouseWheel()
{
  // if an axis is selected, only allow the direction of that axis to be zoomed
  // if no axis is selected, both directions may be zoomed
  
    q_tree::update_tree(ui->customPlot);
    
  if (ui->customPlot->xAxis->selectedParts().testFlag(QCPAxis::spAxis))
    ui->customPlot->axisRect()->setRangeZoom(ui->customPlot->xAxis->orientation());
  else if (ui->customPlot->yAxis->selectedParts().testFlag(QCPAxis::spAxis))
    ui->customPlot->axisRect()->setRangeZoom(ui->customPlot->yAxis->orientation());
  else
    ui->customPlot->axisRect()->setRangeZoom(Qt::Horizontal|Qt::Vertical);
}

void MainWindow::removePath(void *ptr, bool replot) {
    assert(paths_of_line.count(ptr) > 0);
    auto path = paths_of_line[ptr];
    for (auto w : *path) {
        ui->customPlot->removeItem(w);
        paths_of_line.erase(w);
    }
    if (replot) {
        ui->customPlot->replot();
    }
}
void MainWindow::removeAllPaths() {
    while (paths_of_line.size() > 0) {
        removePath((*paths_of_line.begin()).first, false);
    }
    ui->customPlot->replot();
}
void MainWindow::removeSelectedPaths() {
    auto selectedPaths = getSelectedPaths();
    for (auto path_ptr : selectedPaths) {
        removePath((*path_ptr)[0]);
    }
}

void MainWindow::contextMenuRequest(QPoint pos) {
    if (!ui->radioButton->isChecked()) {
        QMenu *menu = new QMenu(this);
        menu->setAttribute(Qt::WA_DeleteOnClose);
        
        menu->addAction("Remove all paths", this, SLOT(removeAllPaths()));
        
        if (getSelectedPaths().size() > 0) {
            menu->addAction("Remove selected paths", this, SLOT(removeSelectedPaths()));
        }
        menu->popup(ui->customPlot->mapToGlobal(pos));
    } else {
        QCustomPlot *plot = ui->customPlot;
        double x = plot->xAxis->pixelToCoord(pos.x());
        double y = plot->yAxis->pixelToCoord(pos.y());
        draw_path(x, y);
    }
}

void MainWindow::clickOnButton() {
    q_tree::redraw_tree(ui->customPlot);
}


