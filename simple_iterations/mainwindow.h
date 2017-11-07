#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QInputDialog>
#include "../qcustomplot.h"
#include "ui_mainwindow.h"
#include <bitset>
#include <vector>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
  Q_OBJECT
  
public:
  template <size_t bitSetSize>
  MainWindow(QWidget *parent, double r, const std::bitset<bitSetSize> &need_to_show)  : QMainWindow(parent), r(r), ui(new Ui::MainWindow)
  {
    srand(QDateTime::currentDateTime().toTime_t());
    ui->setupUi(this);

    for (size_t i = 0; i < bitSetSize; i++) this->need_to_show.push_back(need_to_show[i]);
    
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

  ~MainWindow();
  
private slots:
  void titleDoubleClick(QMouseEvent *event);
  void axisLabelDoubleClick(QCPAxis* axis, QCPAxis::SelectablePart part);
  void legendDoubleClick(QCPLegend* legend, QCPAbstractLegendItem* item);
  void selectionChanged();
  void mousePress();
  void mouseWheel();
  void removeSelectedGraph();
  void removeAllGraphs();
  void contextMenuRequest(QPoint pos);
  void moveLegend();
  void graphClicked(QCPAbstractPlottable *plottable, int dataIndex);

  void on_pushButton_2_clicked();

public slots:
  void addStraightLine(double k);
  void addBaseGraph();
  void addPhiGraph();
  void addBuildLine(double k);
  void addSeqGraph(const std::vector<double> &points);
  void addBifurGiag(); //TODO for Anna
  
private:
  std::vector<bool> need_to_show;
  Ui::MainWindow *ui;
  double from;
  double to;
  double r;
  double h = 0;
};

#endif // MAINWINDOW_H
