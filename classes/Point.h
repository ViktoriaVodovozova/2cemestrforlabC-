#ifndef POINT_H
#define POINT_H

#include <vector>

// Структура Point - это одна точка на плоскости с меткой класса
// Пример: точка (1.5, 2.3) принадлежит классу 1 (синий)
struct Point {
    double x;      // Координата X
    double y;      // Координата Y
    int label;     // Класс точки: 0 (жёлтый) или 1 (синий)

    Point();                    // Точка (0, 0) с классом 0
    Point(double x, double y, int label);  // Точка с координатами

    std::vector<double> toVector() const;  // Вернуть [x, y] как вектор
};

#endif // POINT_H