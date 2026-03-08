#ifndef DATASET_H
#define DATASET_H

#include "Point.h"
#include "Matrix.h"
#include <vector>
#include <string>

// Класс Dataset - это набор точек для обучения нейронки
class Dataset {
private:
    std::vector<Point> points;  // Список всех точек

public:
    Dataset();  // Пустой набор данных

    void addPoint(const Point& point);              // Добавить точку
    void addPoint(double x, double y, int label);   // Добавить точку по координатам
    Point getPoint(int index) const;                // Получить точку по индексу
    int size() const;                               // Количество точек
    void clear();                                   // Очистить все точки

    Matrix toMatrix() const;         // Все координаты в матрицу Nx2
    Matrix labelsToMatrix() const;   // Все метки в матрицу Nx1

};

#endif // DATASET_H