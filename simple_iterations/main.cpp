#include <QApplication>
#include "mainwindow.h"
#include <iostream>
#include "seq_iterations.h"

int main(int argc, char *argv[])
{
//  QApplication a(argc, argv);
//  MainWindow w;
//  w.show();

   std::vector<double> points = get_seq_iteration_points(2);
   for (auto i: points) {
      std::cout << i << " ";
   }
   std::cout << std::endl;

   return 0;
}
