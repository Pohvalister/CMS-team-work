#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QInputDialog>
#include "../qcustomplot.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
  Q_OBJECT
  
public:
  explicit MainWindow(QWidget *parent, double r);
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

public slots:
  void addStraightLine(double k);
  void addBaseGraph();
  void addPhiGraph();
  void addBuildLine(double k);
  void addSeqGraph(const std::vector<double> &points);
  void addBifurGiag(); //TODO for Anna
  
private:
  Ui::MainWindow *ui;
  double from;
  double to;
  const double r;
  double h = 0;
  size_t cnt = 0;
};

#endif // MAINWINDOW_H
