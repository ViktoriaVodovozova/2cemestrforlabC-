#ifndef DATASET_H
#define DATASET_H

#include "Point.h"
#include "Matrix.h"
#include <vector>
#include <string>

// класс Dataset - это набор точек для обучения нейронки
class Dataset {
private:
    std::vector<Point> points;  // список всех точек

public:
    Dataset();  // пустой набор данных

    void addPoint(const Point& point);              // добавить точку
    void addPoint(double x, double y, int label);   // добавить точку по координатам
    Point getIndex(int index) const;                // получить точку по индексу
    std::vector<Point>& getPoints();                // получить точки в виде вектора
    int size() const;                               // количество точек
    void clear();                                   // очистить все точки

    Matrix toMatrix() const;         // все координаты в матрицу Nx2
    Matrix labelsToMatrix() const;   // все метки в матрицу Nx1

};

#endif // DATASET_H