#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <vector>
#include <map>
#include <memory>
#include <set>

#include <QMainWindow>
#include <QInputDialog>
#include "../qcustomplot.h"

using namespace std;

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
  Q_OBJECT
  
public:
  explicit MainWindow(QWidget *parent = 0);
  ~MainWindow();
  
private slots:
  void titleDoubleClick(QMouseEvent *event);
  void axisLabelDoubleClick(QCPAxis* axis, QCPAxis::SelectablePart part);
  void legendDoubleClick(QCPLegend* legend, QCPAbstractLegendItem* item);
  void selectionChanged();
  void mousePress();
  void mouseWheel();
  void addRandomGraph();
  void contextMenuRequest(QPoint pos);
  void moveLegend();
  void graphClicked(QCPAbstractPlottable *plottable, int dataIndex);
  
  void draw_path(double x, double y);
  set<shared_ptr<vector<QCPItemLine*>>> getSelectedPaths();
  void removeAllPaths();
  void removePath(void *ptr, bool replot = true);
  void removeSelectedPaths();
  
  void clickOnButton();
private:
  map<void*, shared_ptr<vector<QCPItemLine*>>> paths_of_line;
  
  Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
