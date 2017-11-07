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

   MainWindow w(nullptr, r, d.need_to_show_id);
   w.do_new_visualisation();
   w.show();

   return a.exec();
}
