#include "Dataset.h"
#include <iostream>

Dataset::Dataset() {
    // пустой набор данных
}

void Dataset::addPoint(const Point& point) {
    // добавляем точку в конец списка
    points.push_back(point);
}

void Dataset::addPoint(double x, double y, int label) {
    // создаём точку и добавляем в список
    points.push_back(Point(x, y, label));
}

Point Dataset::getPoint(int index) const {
    // проверка на выход за границы
    if (index < 0 || index >= points.size()) {
        std::cerr << "Ошибка: индекс точки вне диапазона!" << std::endl;
        return Point();
    }
    return points[index];
}

int Dataset::size() const {
    // возвращаем количество точек
    return points.size();
}

void Dataset::clear() {
    // очищаем все точки
    points.clear();
}


Matrix Dataset::toMatrix() const {
    // создаём матрицу: строки = количество точек, столбцы = 2 (x и y)
    Matrix result(points.size(), 2);
    
    for (int i = 0; i < points.size(); i++) {
        result.set(i, 0, points[i].x);  // Столбец 0 = x
        result.set(i, 1, points[i].y);  // Столбец 1 = y
    }
    
    return result;
}

Matrix Dataset::labelsToMatrix() const {
    // создаём матрицу: строки = количество точек, столбцы = 1 (label)
    Matrix result(points.size(), 1);
    
    for (int i = 0; i < points.size(); i++) {
        result.set(i, 0, points[i].label);  // столбец 0 = label
    }
    
    return result;
}