#include <iostream>
#include <fstream>
#include <cmath>
#include <ctime>
#include <nlohmann/json.hpp>

#include "classes/Dataset.h"
#include "classes/Matrix.h"
#include "classes/Point.h"

using json = nlohmann::json;

double randomDouble(double min, double max) {
    return min + (max - min) * (rand() / (double)RAND_MAX);
}

Dataset get_points (int first_cl_size, int second_cl_size) {
    srand(time(nullptr));
    
    // Создаем объект Dataset для хранения всех точек
    Dataset dataset;

    // center1 и center2 - центры кластеров 1 и 2
    Point center1, center2;

    // задаем центры кластеров в диапазоне от -10 000 до 10 000
    center1.x = randomDouble(100.0, 200.0);
    center1.y = randomDouble(100.0, 200.0);
    center1.label = 0;
    center2.x = randomDouble(100.0, 200.0);
    center2.y = randomDouble(100.0, 200.0);
    center2.label = 1;

    float rad = sqrt(pow((center1.x - center2.x), 2) + pow((center1.y - center2.y), 2)) * 0.5 * 0.3;

    // Генерация точек
    // Первый кластер
    for(int i = 0; i < first_cl_size; ++i){
        Point current;

        current.x = rand() % (int)(2*rad) + (center1.x - rad);
        current.y = rand() % (int)(2*rad) + (center1.y - rad);
        current.label = 0;

        // Добавляем точку в Dataset
        dataset.addPoint(current);
    }

    // Второй кластер
    for(int i = 0; i < second_cl_size; ++i){
        Point current;

        current.x = rand() % (int)(2*rad) + (center2.x - rad);
        current.y = rand() % (int)(2*rad) + (center2.y - rad);
        current.label = 1;

        // Добавляем точку в Dataset
        dataset.addPoint(current);
    }
    return dataset;
}
    
int main() {

    std::cout << "Сколько точек должно быть в первом и втором кластерах? Последовательно введите с клавиатуры:";
    int first_cl_size, second_cl_size;
    std::cin >> first_cl_size >> second_cl_size;

    Dataset data = get_points(first_cl_size, second_cl_size);

    // Выводим информацию о созданном датасете
    std::cout << "Создан датасет с " << data.size() << " точками" << std::endl;

    // Запись в JSON из Dataset
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
    system("python3 ../visualization.py");
    std::cout << 'a';

    return 0;
}