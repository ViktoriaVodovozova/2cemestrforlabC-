#include "Point.h"


Point::Point() : x(0.0), y(0.0), label(0) {
    // точка в начале координат с классом 0
}

Point::Point(double x, double y, int label) : x(x), y(y), label(label) {
    // точка с заданными координатами и классом
}


std::vector<double> Point::toVector() const {
    // возвращаем координаты как вектор из 2 элементов
    std::vector<double> vec;
    vec.push_back(x);
    vec.push_back(y);
    return vec;
}