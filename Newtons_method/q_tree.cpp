#include "q_tree.h"
#include "newtons_method.h"

q_tree *root = nullptr;

rectangle::rectangle(double x_1, double y_1, double x_2, double y_2)
: x_1(x_1), y_1(y_1), x_2(x_2), y_2(y_2) {
}
rectangle& rectangle::operator=(const rectangle& another) {
    x_1 = another.x_1;
    x_2 = another.x_2;
    y_1 = another.y_1;
    y_2 = another.y_2;
    return *this;
}
double rectangle::square() const {
    return (x_2 - x_1) * (y_2 - y_1);
}
rectangle rectangle::intersect(const rectangle& another) const {
    rectangle res(max(x_1, another.x_1), max(y_1, another.y_1),
                  min(x_2, another.x_2), min(y_2, another.y_2));
    if ((res.x_1 >= res.x_2) || (res.y_1 >= res.y_2)) {
        res.x_1 = res.x_2 = res.y_1 = res.y_2 = 0;
    }
    return res;
}
bool operator==(const rectangle& r_1, const rectangle& r_2) {
    return ((r_1.x_1 == r_2.x_1) &&
            (r_1.x_2 == r_2.x_2) &&
            (r_1.y_1 == r_2.y_1) &&
            (r_1.y_2 == r_2.y_2));
}
bool operator!=(const rectangle& r_1, const rectangle& r_2) {
    return (!(r_1 == r_2));
}

q_tree::q_tree(double x_1, double y_1, double x_2, double y_2)
    : rect(x_1, y_1, x_2, y_2) {
    for (int e = 0; e < 2; e++) {
        for (int r = 0; r < 2; r++) {
            field[e][r] = nullptr;
        }
    }
}
void q_tree::update(int required_depth, const rectangle& range_2d, QCustomPlot *plot) {
    {
        rectangle inter = rect.intersect(range_2d);
        if (inter.square() == 0) {
            initColorMap(plot);
            return;
        }
    }
    if (required_depth == 0) {
        initColorMap(plot);
    }
    if (required_depth <= min_depth) {
        return;
    }
    
    double dx = (rect.x_2 - rect.x_1) / 2;
    double dy = (rect.y_2 - rect.y_1) / 2;
    min_depth = 1<<30;
    for (int e = 0; e < 2; e++) {
        for (int r = 0; r < 2; r++) {
            if (field[e][r] == nullptr) {
                field[e][r] = new q_tree(rect.x_1 + dx * e      , rect.y_1 + dy * r       ,
                                         rect.x_1 + dx * (e + 1), rect.y_1 + dy * (r + 1));
            }
            field[e][r]->update(required_depth - 1, range_2d, plot);
            min_depth = min(min_depth, field[e][r]->min_depth + 1);
        }
    }
    deleteColorMap(plot);
}
void q_tree::initColorMap(QCustomPlot *plot) {
    if (colorMap != nullptr) {
        return;
    }
    colorMap = new QCPColorMap(plot->xAxis, plot->yAxis);
    
    colorMap->data()->setSize(COLOR_MAP_SIZE_1, COLOR_MAP_SIZE_2);
    colorMap->data()->setRange(QCPRange(rect.x_1, rect.x_2), QCPRange(rect.y_1, rect.y_2));
    
    colorMap->setInterpolate(false);
    colorMap->setGradient(QCPColorGradient::gpSpectrum);
    
    double x, y;
    for (int e = 0; e < COLOR_MAP_SIZE_1; e++) {
        for (int r = 0; r < COLOR_MAP_SIZE_2; r++) {
            colorMap->data()->cellToCoord(e, r, &x, &y);
            colorMap->data()->setCell(e, r, calculate_color(x, y));
        }
    }
    colorMap->setDataRange({-1, 1});
    colorMap->setLayer(colorMapLayer);
}
void q_tree::deleteColorMap(QCustomPlot *plot) {
    if (colorMap != nullptr) {
        bool res = plot->removePlottable(colorMap);
        if (res == false) {
            cout << "Error in q_tree::deleteColorMap\n";
            throw runtime_error("");
        }
        colorMap = nullptr;
    }
}
void q_tree::init(QCustomPlot *plot, double x_1, double y_1, double x_2, double y_2) {
    root = new q_tree(x_1, y_1, x_2, y_2);
    root->initColorMap(plot);
}
void q_tree::update_tree(QCustomPlot *plot) {
    QCPAxisRect *rect = plot->axisRect();
    QCPRange v_range = rect->rangeZoomAxis(Qt::Vertical)->range();
    QCPRange h_range = rect->rangeZoomAxis(Qt::Horizontal)->range();
    
    rectangle range_2d(h_range.lower, v_range.lower,
                       h_range.upper, v_range.upper);
    
    while (root->rect.intersect(range_2d) != range_2d) {
        int e = 0, r = 0;
        if (range_2d.x_1 < root->rect.x_1) {
            e = 1;
        }
        if (range_2d.y_1 < root->rect.y_1) {
            r = 1;
        }
        rectangle &rect = root->rect;
        double dx = rect.x_2 - rect.x_1;
        double dy = rect.y_2 - rect.y_1;
        
        q_tree *new_root = new q_tree(rect.x_1 - e * dx, rect.y_1 - r * dy,
                                      rect.x_2 + (1 - e) * dx, rect.y_2 + (1 - r) * dy);
        new_root->min_depth = 1;
        
        for (int c = 0; c < 2; c++) {
            for (int v = 0; v < 2; v++) {
                if ((c == e) && (v == r)) {
                    new_root->field[c][v] = root;
                } else {
                    rectangle &rect = new_root->rect;
                    new_root->field[c][v] = new q_tree(rect.x_1 + dx * c      , rect.y_1 + dy * v       ,
                                                       rect.x_1 + dx * (c + 1), rect.y_1 + dy * (v + 1));
                    new_root->field[c][v]->update(0, rect, plot);
                }
            }
        }
        root = new_root;
    }
    plot->replot();
    
    double dx = range_2d.x_2 - range_2d.x_1;
    double dy = range_2d.y_2 - range_2d.y_1;
    
    double r_dx = root->rect.x_2 - root->rect.x_1;
    double r_dy = root->rect.y_2 - root->rect.y_1;
    
    cout << root->rect.x_1 << " " << root->rect.x_2 << "   " << root->rect.y_1 << " " << root->rect.y_2 << "\n";
    
    cout << dx << " " << dy << "   " << r_dx << " " << r_dy << "\n";
    
    double zoom = max(r_dx / dx, r_dy / dy);
    
    cout << zoom << "\n\n";
    
    int depth = OPTIMAL_DEPTH;
    while (zoom > 1) {
        zoom /= 2;
        depth++;
    }
    root->update(depth, range_2d, plot);
    
    plot->replot();
    
    fflush(stdout);
}
