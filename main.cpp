#include <iostream>
#include <fstream>
#include <cmath>
#include "classes/Dataset.h"
#include "classes/Matrix.h"
#include "classes/Point.h"
#include "get_points.h"

// функция для подсчета ошибки (сколько точек не на своих местах)
double totalLoss(const Dataset& data, double k, double b) {
    int errors = 0;
    for (int i = 0; i < data.size(); i++) {
        Point p = data.getPoint(i);
        // предсказываем кластер по тому, выше или ниже точки прямой
        double lineY = k * p.x + b;
        int predictedCluster = (p.y > lineY) ? 1 : 0;
        
        if (predictedCluster != p.label) {
            errors++;
        }
    }
    return static_cast<double>(errors) / data.size(); // возвращаем долю ошибок
}

int main() {
    std::cout << "=== Генерация кластеров ===" << std::endl;
    
    // генерируем точки (по 100 в каждом кластере, идеальная прямая - y = -2x + 10)
    Dataset data = get_points(100, 100, -2, 10);
    std::cout << "Сгенерировано " << data.size() << " точек" << std::endl;
    
    // Считаем total loss
    double k, b;
    std::cout << "Введите последовательно k и b для проверяемой прямой y = kx + b:" << std::endl << "k = ";
    std::cin >> k;
    std::cout << "b = ";
    std::cin >> b;
    double loss = totalLoss(data, k, b);
    std::cout << "Прямая: y = " << k << "x + " << b << std::endl;
    std::cout << "total loss: " << loss * 100 << "%" << std::endl;
    
    // Сохраняем в JSON
    std::ofstream fout("points.json");
    fout << "{\n    \"points\": [\n";
    for (int i = 0; i < data.size(); i++) {
        Point p = data.getPoint(i);
        fout << "        {\"x\": " << p.x << ", \"y\": " << p.y << ", \"label\": " << p.label << "}";
        if (i < data.size() - 1) fout << ",";
        fout << "\n";
    }
    fout << "    ]\n}\n";
    fout.close();
    
    std::cout << "Данные сохранены в points.json" << std::endl;
    std::cout << "Запустите python3 visualization.py для просмотра" << std::endl;
    
    return 0;
}