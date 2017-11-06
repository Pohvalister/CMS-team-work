#include <QApplication>
#include "mainwindow.h"
#include <iostream>
#include "seq_iterations.h"
#include <dialog.h>

int main(int argc, char *argv[])
{
   QApplication a(argc, argv);

   Dialog d;
   d.exec();

   double r = d.value;

   MainWindow w(nullptr, r);
   w.show();

   bool f = true;
   for (size_t i = 0; i < get_amount_of_iterations(r); ++i) {
      w.addStraightLine(get_next_point(r, f));
      f = false;
   }

   return a.exec();
}
