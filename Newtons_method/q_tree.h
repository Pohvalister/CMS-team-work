#ifndef Q_TREE_H
#define Q_TREE_H

#include "mainwindow.h"
#include <iostream>
using namespace std;

class rectangle {
public:
    rectangle(double x_1, double y_1, double x_2, double y_2);
    rectangle& operator=(const rectangle& another);
    
    double square() const;
    rectangle intersect(const rectangle& another) const;
    friend bool operator==(const rectangle& r_1, const rectangle& r_2);
    friend bool operator!=(const rectangle& r_1, const rectangle& r_2);
    
    double x_1, y_1, x_2, y_2;
};

class q_tree {
// Some point (x, y) belongs to q_tree only if (x_1 <= x < x_2) and (y_1 <= y < y_2)
public:
    static const int COLOR_MAP_SIZE_1 = 100;
    static const int COLOR_MAP_SIZE_2 = 100;
    static const int OPTIMAL_DEPTH = 1;
    
    q_tree(double x_1, double y_1, double x_2, double y_2);
    
    void initColorMap(QCustomPlot *plot);
    void deleteColorMap(QCustomPlot *plot);
    
    void update(int required_depth, const rectangle& range, QCustomPlot *plot);
    
    static void init(QCustomPlot *plot, double x_1, double y_1, double x_2, double y_2);
    
    static void update_tree(QCustomPlot *plot);
private:
    q_tree *field[2][2];
    int min_depth = 0;
    QCPColorMap *colorMap = nullptr;
    rectangle rect;
};

extern q_tree *root;
extern QCPLayer *colorMapLayer;

#endif // Q_TREE_H
