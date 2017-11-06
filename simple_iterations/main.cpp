#include <QApplication>
#include "mainwindow.h"
#include <iostream>
#include "seq_iterations.h"


int main(int argc, char *argv[])
{
   const double r = 1.5;

   QApplication a(argc, argv);
   MainWindow w(nullptr, r);
   w.show();

   bool f = true;
   for (size_t i = 0; i < get_amount_of_iterations(r); ++i) {
      w.addStraightLine(get_next_point(r, f));
      f = false;
   }

   return a.exec();
}
