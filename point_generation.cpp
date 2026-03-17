#include <iostream>
#include <fstream>
#include <cmath>
#include <ctime>
#include <random>
#include <nlohmann/json.hpp>

#include "classes/Dataset.h"
#include "classes/Matrix.h"
#include "classes/Point.h"

using json = nlohmann::json;

static std::random_device rd;
static std::mt19937 gen(rd());
static std::uniform_real_distribution<double> dist(0.0, 1.0);

double randomDouble(double min, double max) {
    return min + (max - min) * (dist(gen));
}

Dataset get_points (int first_cl_size, int second_cl_size, float k, float b) {
    
    // создаем объект Dataset для хранения всех точек
    Dataset dataset;

    // center1 и center2 - центры кластеров 1 и 2
    Point center1, center2;

    // задаем центры кластеров в диапазоне от -10 000 до 10 000
    center1.x = randomDouble(-200.0, 200.0);
    center1.y = randomDouble(-200.0, 200.0);
    center1.label = 0;

    // для центра второго кластера используются формулы для нахождения точки, симметричной данной отн-но заданной прямой.
    center2.x = center1.x - 2*k*(k*center1.x - center1.y + b) / (k*k + 1);
    center2.y = center1.y + 2*(k*center1.x - center1.y + b) / (k*k + 1);
    center2.label = 1;

    double radius = sqrt(pow((center1.x - center2.x), 2) + pow((center1.y - center2.y), 2)) * 0.5 * 0.8;

    // генерация точек

    // первый кластер
    for(int i = 0; i < first_cl_size; ++i){
        Point current;

        // генерируем случайный угол и случайное расстояние
        double angle = 2 * M_PI * dist(gen);
        // sqrt(dist(gen)) для равномерного распределения по площади
        double r = radius * sqrt(dist(gen));

        current.x = center1.x + r * cos(angle);
        current.y = center1.y + r * sin(angle);
        current.label = 0;

        // добавляем точку в Dataset
        dataset.addPoint(current);
    }

    // второй кластер
    for(int i = 0; i < second_cl_size; ++i){
        Point current;

        double angle = 2 * M_PI * dist(gen);
        double r = radius * sqrt(dist(gen));

        current.x = center2.x + r * cos(angle);
        current.y = center2.y + r * sin(angle);
        current.label = 1;

        // добавляем точку в Dataset
        dataset.addPoint(current);
    }
    return dataset;
}
    
int main() {

    std::cout << "Сколько точек должно быть в первом и втором кластерах? Последовательно введите с клавиатуры:";
    int first_cl_size, second_cl_size;
    std::cin >> first_cl_size >> second_cl_size;

    std::cout << "Относительно какой прямой будут строиться кластеры? Последовательно введите с клавиатуры коэфициенты k и b для прямой y = kx + b:";
    double k, b;
    std::cin >> k >> b;

    Dataset data = get_points(first_cl_size, second_cl_size, k, b);

    // выводим информацию о созданном датасете
    std::cout << "Создан датасет с " << data.size() << " точками относительно прямой y = ";
    if (k == 1) std:: cout << "x + ";
        else if (k != 0) std:: cout << k << "x + ";
    std::cout << b << std::endl;

    // запись в JSON из Dataset
    std::ofstream fout("points.json");

    fout << "{\n";
    fout << "    \"points\": [\n";
    
    for (unsigned int i = 0; i < data.size(); i++) {
        Point p = data.getPoint(i);
        fout << "        {\"x\": " << p.x << ", \"y\": " << p.y << ", \"label\": " << p.label << "}";
        if (i < data.size() - 1) {
            fout << ",";
        }
        fout << "\n";
    }
    
    fout << "    ]\n";  
    fout << "}\n";
    
    fout.close();
    
    std::cout << "Точки успешно сохранены в points.json" << std::endl;

    return 0;
}