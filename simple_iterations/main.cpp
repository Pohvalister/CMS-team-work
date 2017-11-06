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
   if (d.is_closed) return 0;

   MainWindow w(nullptr, r);

   if (d.need_to_show[0])    //Vis of simple iteration method
   {
      w.addBaseGraph();
      bool f = true;
      for (size_t i = 0; i < get_amount_of_iterations(r); ++i) {
         w.addStraightLine(get_next_point(r, f));
         f = false;
      }
   } else if (d.need_to_show[1])  //Vis of iteration method on phi(x)
   {
      w.addPhiGraph();
      bool f = true;
      for (size_t i = 0; i < get_amount_of_iterations(r); ++i) {
         w.addBuildLine(get_next_point(r, f));
         f = false;
      }
   } else if (d.need_to_show[2]) //Vis of sequence
   {
      w.addSeqGraph(get_seq_iteration_points(r, get_amount_of_iterations(r)));
   }
   w.show();

   return a.exec();
}
