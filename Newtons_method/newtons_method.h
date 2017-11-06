#ifndef NEWTONS_METHOD_H
#define NEWTONS_METHOD_H

#include <vector>

int calculate(double x, double y, double precision = 0.01);
double calculate_color(double x, double y, double precision = 0.01);
std::vector<std::pair<double, double>> calculate_path(double x, double y, double precision = 0.01);

#endif // NEWTONS_METHOD_H
