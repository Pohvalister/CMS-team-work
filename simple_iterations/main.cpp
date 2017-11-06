#include <QApplication>
#include "mainwindow.h"
#include <iostream>
#include "seq_iterations.h"


int main(int argc, char *argv[])
{
   const double r = 0.5;

   QApplication a(argc, argv);
   MainWindow w(nullptr, r);
   w.show();

   bool f = true;
   /*for (size_t i = 0; i < get_amount_of_iterations(r); ++i) {
      w.addStraightLine(get_next_point(r, f));
      f = false;
   }*/
   std::vector<double> cons = get_sequence_of_x_n(r);
   for (size_t i=0;i<cons.size();i++){
       w.addStraightLine(cons[i]);
   }

   return a.exec();
}
