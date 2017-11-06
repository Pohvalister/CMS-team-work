#include <QApplication>
#include "mainwindow.h"
#include <iostream>
#include "seq_iterations.h"


int main(int argc, char *argv[])
{
   const double r = 2.0;

   QApplication a(argc, argv);
   MainWindow w(nullptr, r);
   w.show();

   std::vector<double> points = get_seq_iteration_points(r);
   for (auto p: points) {
      w.addStraightLine(p);
   }

   return a.exec();
}
