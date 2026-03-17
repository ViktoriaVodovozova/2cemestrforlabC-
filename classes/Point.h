#ifndef POINT_H
#define POINT_H

#include <vector>

// структура Point - это одна точка на плоскости с меткой класса
// пример: точка (1.5, 2.3) принадлежит классу 1 (синий)
struct Point {
    double x;      // координата X
    double y;      // координата Y
    int label;     // класс точки: 0 (жёлтый) или 1 (синий)

    Point();                    // точка (0, 0) с классом 0
    Point(double x, double y, int label);  // точка с координатами

    std::vector<double> toVector() const;  // вернуть [x, y] как вектор
};

#endif // POINT_H