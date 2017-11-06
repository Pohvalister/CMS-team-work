#include "newtons_method.h"
#include <complex>
#include <cmath>
#include <vector>
#include <map>

#include <iostream>
using namespace std;

const int MAX_ITERATIONS = 57;

const int DIVERGES_KEY = 0;
const int ROOT_1_KEY = 1;
const int ROOT_2_KEY = 2;
const int ROOT_3_KEY = 3;

const double PI = acos(0) * 2;

typedef complex<double> comp_d;

const vector<comp_d> ROOTS = {
    {1, 0},
    {cos(2*PI/3), sin(2*PI/3)},
    {cos(4*PI/3), sin(4*PI/3)}
};
const map<int, double> disp = {
    {0, -1},
    {1, 0.5},
    {2, 0},
    {3, -0.5}
};
const vector<int> KEYS = {ROOT_1_KEY, ROOT_2_KEY, ROOT_3_KEY};

int calculate(double x, double y, double precision) {
    double sq_precision = precision * precision;
    comp_d prev(x, y);
    
    for (int w = 0; w < MAX_ITERATIONS; w++) {
        comp_d next = prev - comp_d(1/3.0) * (prev - comp_d(1) / (prev * prev));
        swap(next, prev);
        if (norm(prev - next) <= sq_precision) {
            break;
        }
    }
    for (int c = 0; c < 3; c++) {
        if (norm(ROOTS[c] - prev) <= sq_precision + 1e-9) {
            return KEYS[c];
        }
    }
    return DIVERGES_KEY;
}
double calculate_color(double x, double y, double precision) {
    return (*disp.find(calculate(x, y, precision))).second;
}
