#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "../qcustomplot.h"
#include <functional>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
  Q_OBJECT
  
public:
      size_t iterations_amount = 0;
    double convergence_scope = 0.0001;
   explicit MainWindow(QWidget *parent, double r, int target_id);
  ~MainWindow();
   void do_new_visualisation();
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
   void addStraightLine(double k);
   void addBaseGraph();
   void addPhiGraph();
   void addBuildLine(double k);

   void addSimpleItVis();
   void addPhiItViss();
   void addSeqGraph();
   void addBifurGiag(); //TODO for Anna
   void on_It_Set_button_clicked();
  
private:
   Ui::MainWindow *ui;
   double r;
   std::function<void(MainWindow*)> target_vis;
   double from;
   double to;
   double h = 0;

};

#endif // MAINWINDOW_H
