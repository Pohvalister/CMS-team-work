#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent, double r) :
  QMainWindow(parent),
  ui(new Ui::MainWindow),
  r(r)
{
  srand(QDateTime::currentDateTime().toTime_t());
  ui->setupUi(this);
  
  ui->customPlot->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom | QCP::iSelectAxes |
                                  QCP::iSelectLegend | QCP::iSelectPlottables);
  ui->customPlot->xAxis->setRange(-8, 8);
  ui->customPlot->yAxis->setRange(-5, 5);
  ui->customPlot->axisRect()->setupFullAxesBox();
  
  ui->customPlot->plotLayout()->insertRow(0);
  QCPTextElement *title = new QCPTextElement(ui->customPlot, "Simple iterations", QFont("sans", 17, QFont::Bold));
  ui->customPlot->plotLayout()->addElement(0, 0, title);
  
  ui->customPlot->xAxis->setLabel("x Axis");
  ui->customPlot->yAxis->setLabel("y Axis");
//  ui->customPlot->legend->setVisible(true);
  QFont legendFont = font();
  legendFont.setPointSize(10);
  ui->customPlot->legend->setFont(legendFont);
  ui->customPlot->legend->setSelectedFont(legendFont);
  ui->customPlot->legend->setSelectableParts(QCPLegend::spItems); // legend box shall not be selectable, only legend items
  ui->customPlot->rescaleAxes();
}


MainWindow::~MainWindow()
{
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

void MainWindow::legendDoubleClick(QCPLegend *legend, QCPAbstractLegendItem *item)
{
  // Rename a graph by double clicking on its legend item
  Q_UNUSED(legend)
  if (item) // only react if item was clicked (user could have clicked on border padding of legend where there is no item, then item is 0)
  {
    QCPPlottableLegendItem *plItem = qobject_cast<QCPPlottableLegendItem*>(item);
    bool ok;
    QString newName = QInputDialog::getText(this, "QCustomPlot example", "New graph name:", QLineEdit::Normal, plItem->plottable()->name(), &ok);
    if (ok)
    {
      plItem->plottable()->setName(newName);
      ui->customPlot->replot();
    }
  }
}

void MainWindow::selectionChanged()
{
  /*
   normally, axis base line, axis tick labels and axis labels are selectable separately, but we want
   the user only to be able to select the axis as a whole, so we tie the selected states of the tick labels
   and the axis base line together. However, the axis label shall be selectable individually.
   
   The selection state of the left and right axes shall be synchronized as well as the state of the
   bottom and top axes.
   
   Further, we want to synchronize the selection of the graphs with the selection state of the respective
   legend item belonging to that graph. So the user can select a graph by either clicking on the graph itself
   or on its legend item.
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
  
  // synchronize selection of graphs with selection of corresponding legend items:
  for (int i=0; i<ui->customPlot->graphCount(); ++i)
  {
    QCPGraph *graph = ui->customPlot->graph(i);
    QCPPlottableLegendItem *item = ui->customPlot->legend->itemWithPlottable(graph);
    if (item->selected() || graph->selected())
    {
      item->setSelected(true);
      graph->setSelection(QCPDataSelection(graph->data()->dataRange()));
    }
  }
}

void MainWindow::mousePress()
{
  // if an axis is selected, only allow the direction of that axis to be dragged
  // if no axis is selected, both directions may be dragged
  
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
  
  if (ui->customPlot->xAxis->selectedParts().testFlag(QCPAxis::spAxis))
    ui->customPlot->axisRect()->setRangeZoom(ui->customPlot->xAxis->orientation());
  else if (ui->customPlot->yAxis->selectedParts().testFlag(QCPAxis::spAxis))
    ui->customPlot->axisRect()->setRangeZoom(ui->customPlot->yAxis->orientation());
  else
    ui->customPlot->axisRect()->setRangeZoom(Qt::Horizontal|Qt::Vertical);
}

void MainWindow::addBaseGraph()
{
   using dbl = double;
   auto f = [this](dbl x) -> dbl {return -r*x*x + r*x - x;};

   const int N = 101;
   QVector<dbl> x, y; // initialize with entries 0..100

   this->h = (-(r - 1)*(r - 1)) / (4*r) + (r - 1) / 2 - (r - 1) / (2*r);
   from = (r - 1 - sqrt((r - 1) * (r - 1) + r * h))/(2 * r);
   to = (r - 1 + sqrt((r - 1) * (r - 1) + r * h))/(2 * r);
   dbl step = (to - from) / N;

   x.push_back(0);
   y.push_back(0);
   x.push_back((r - 1) / r);
   y.push_back(0);
   if (from > to) throw std::runtime_error("from > to");
   else if (from == to && step == 0) {
      from = -1;
      to = 1;
      h = 1;
      step = 2. / N;
   }
   for (dbl cur_x = from; cur_x <= to; cur_x += step) {
      x.push_back(cur_x);
      y.push_back(f(cur_x));
   }

   // create graph and assign data to it:
   ui->customPlot->addGraph();
   ui->customPlot->graph(0)->setData(x, y);

   // give the axes some labels:
   ui->customPlot->xAxis->setLabel("x");
   ui->customPlot->yAxis->setLabel("y");

   // set axes ranges, so we see all data:
   ui->customPlot->xAxis->setRange(from, to);
   ui->customPlot->yAxis->setRange(-h/4, h);
   ui->customPlot->replot();
}

void MainWindow::addPhiGraph()
{
   using dbl = double;
   auto f = [this](dbl x) -> dbl {return r*x*(1 - x);};
   const int N = 101;
   QVector<dbl> x, y; // initialize with entries 0..100

   this->h = static_cast<dbl>(r/4);
   from = (1 - sqrt(1 + h)) / 2;
   to = (1 + sqrt(1 + h)) / 2;
   dbl step = (to - from) / N;

   x.push_back(0);
   y.push_back(0);
   x.push_back(1);
   y.push_back(0);

   for (dbl cur_x = from; cur_x <= to; cur_x += step) {
      x.push_back(cur_x);
      y.push_back(f(cur_x));
   }

   // create graph and assign data to it:
   ui->customPlot->addGraph();
   ui->customPlot->graph(0)->setData(x, y);

   // give the axes some labels:
   ui->customPlot->xAxis->setLabel("x");
   ui->customPlot->yAxis->setLabel("y");

   // set axes ranges, so we see all data:
   ui->customPlot->xAxis->setRange(from, to);
   ui->customPlot->yAxis->setRange(-h/4, h);

   x.clear();
   y.clear();
   for (dbl cur_x = from; cur_x <= to; cur_x += step) {
      x.push_back(cur_x);
      y.push_back(cur_x);
   }
   ui->customPlot->addGraph();
   ui->customPlot->graph(++cnt)->setData(x, y);
   ui->customPlot->graph(cnt)->setPen(QPen(Qt::red));

   ui->customPlot->replot();
}

void MainWindow::addBuildLine(double k)
{
   auto f = [this](double x) -> double {return r*x*(1 - x);};
   QVector<double> x, y;
   const int N = 4;
   const double step = (h + h/4) / N;
   double b = f(k);
   for (double cur_x = from; cur_x <= to; cur_x += step) {
      x.push_back(cur_x);
      y.push_back(cur_x - k + b);
   }
   ui->customPlot->addGraph();
   ui->customPlot->graph(++cnt)->setData(x, y);
   ui->customPlot->replot();
}

void MainWindow::addSeqGraph(const std::vector<double> &points)
{
   QVector<double> x, y;
   int i = 0;
   for (; i < (int) points.size(); i++) {
      x.push_back(double(i));
      y.push_back(points[i]);
   }
   // create graph and assign data to it:
   ui->customPlot->addGraph();
   ui->customPlot->graph(0)->setData(x, y);

   // give the axes some labels:
   ui->customPlot->xAxis->setLabel("x");
   ui->customPlot->yAxis->setLabel("y");

   // set axes ranges, so we see all data:
   ui->customPlot->xAxis->setRange(0, i + 1);
   ui->customPlot->yAxis->setRange(-abs(1.2 * points[0]), abs(1.2 * points[0]));
}

void MainWindow::addStraightLine(double k)
{
   QVector<double> x, y;
   const int N = 4;
   const double step = (h + h/4) / N;
   for (double cur_y = -h/4; cur_y <= h; cur_y += step) {
      x.push_back(k);
      y.push_back(cur_y);
   }
   ui->customPlot->addGraph();
   ui->customPlot->graph(++cnt)->setData(x, y);
   ui->customPlot->replot();
}

void MainWindow::removeSelectedGraph()
{
  if (ui->customPlot->selectedGraphs().size() > 0)
  {
    ui->customPlot->removeGraph(ui->customPlot->selectedGraphs().first());
    ui->customPlot->replot();
  }
}

void MainWindow::removeAllGraphs()
{
  ui->customPlot->clearGraphs();
  ui->customPlot->replot();
}

void MainWindow::contextMenuRequest(QPoint pos)
{
  QMenu *menu = new QMenu(this);
  menu->setAttribute(Qt::WA_DeleteOnClose);
  
  if (ui->customPlot->legend->selectTest(pos, false) >= 0) // context menu on legend requested
  {
    menu->addAction("Move to top left", this, SLOT(moveLegend()))->setData((int)(Qt::AlignTop|Qt::AlignLeft));
    menu->addAction("Move to top center", this, SLOT(moveLegend()))->setData((int)(Qt::AlignTop|Qt::AlignHCenter));
    menu->addAction("Move to top right", this, SLOT(moveLegend()))->setData((int)(Qt::AlignTop|Qt::AlignRight));
    menu->addAction("Move to bottom right", this, SLOT(moveLegend()))->setData((int)(Qt::AlignBottom|Qt::AlignRight));
    menu->addAction("Move to bottom left", this, SLOT(moveLegend()))->setData((int)(Qt::AlignBottom|Qt::AlignLeft));
  } else  // general context menu on graphs requested
  {
    menu->addAction("Add random graph", this, SLOT(addRandomGraph()));
    if (ui->customPlot->selectedGraphs().size() > 0)
      menu->addAction("Remove selected graph", this, SLOT(removeSelectedGraph()));
    if (ui->customPlot->graphCount() > 0)
      menu->addAction("Remove all graphs", this, SLOT(removeAllGraphs()));
  }
  
  menu->popup(ui->customPlot->mapToGlobal(pos));
}

void MainWindow::moveLegend()
{
  if (QAction* contextAction = qobject_cast<QAction*>(sender())) // make sure this slot is really called by a context menu action, so it carries the data we need
  {
    bool ok;
    int dataInt = contextAction->data().toInt(&ok);
    if (ok)
    {
      ui->customPlot->axisRect()->insetLayout()->setInsetAlignment(0, (Qt::Alignment)dataInt);
      ui->customPlot->replot();
    }
  }
}

void MainWindow::graphClicked(QCPAbstractPlottable *plottable, int dataIndex)
{
  // since we know we only have QCPGraphs in the plot, we can immediately access interface1D()
  // usually it's better to first check whether interface1D() returns non-zero, and only then use it.
  double dataValue = plottable->interface1D()->dataMainValue(dataIndex);
  QString message = QString("Clicked on graph '%1' at data point #%2 with value %3.").arg(plottable->name()).arg(dataIndex).arg(dataValue);
  ui->statusBar->showMessage(message, 2500);
}




