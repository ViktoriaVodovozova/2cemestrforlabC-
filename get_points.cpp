#include "get_points.h"
#include <cmath>
#include <random>
#include "classes/Point.h"

// статические переменные для генерации случайных чисел
static std::random_device rd;
static std::mt19937 gen(rd());
static std::uniform_real_distribution<double> dist(0.0, 1.0);

double randomDouble(double min, double max) {
    return min + (max - min) * (dist(gen));
}

Dataset get_points(int first_cl_size, int second_cl_size, float k, float b) {
    // создаем объект Dataset для хранения всех точек
    Dataset dataset;

    // center1 и center2 - центры кластеров 1 и 2
    Point center1, center2;

    // задаем центры кластеров в диапазоне от -200 до 200
    center1.x = randomDouble(-200.0, 200.0);
    center1.y = randomDouble(-200.0, 200.0);
    center1.label = 0;

    // для центра второго кластера используются формулы для нахождения точки, симметричной данной относительно заданной прямой
    center2.x = center1.x - 2*k*(k*center1.x - center1.y + b) / (k*k + 1);
    center2.y = center1.y + 2*(k*center1.x - center1.y + b) / (k*k + 1);
    center2.label = 1;

    double radius = sqrt(pow((center1.x - center2.x), 2) + pow((center1.y - center2.y), 2)) * 0.5 * 0.8;

    // генерация точек первого кластера
    for(int i = 0; i < first_cl_size; ++i){
        Point current;

        // генерируем случайный угол и случайное расстояние
        double angle = 2 * M_PI * dist(gen);
        double r = radius * sqrt(dist(gen)); // sqrt для равномерного распределения по площади

        current.x = center1.x + r * cos(angle);
        current.y = center1.y + r * sin(angle);
        current.label = 0;

        dataset.addPoint(current);
    }

    // генерация точек второго кластера
    for(int i = 0; i < second_cl_size; ++i){
        Point current;

        double angle = 2 * M_PI * dist(gen);
        double r = radius * sqrt(dist(gen));

        current.x = center2.x + r * cos(angle);
        current.y = center2.y + r * sin(angle);
        current.label = 1;

        dataset.addPoint(current);
    }
    
    return dataset;
}