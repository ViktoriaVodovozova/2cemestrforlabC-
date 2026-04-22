#include <iostream>
#include <fstream>
#include <cmath>
#include "classes/Dataset.h"
#include "classes/Matrix.h"
#include "classes/NeuralNetwork.h"
#include <nlohmann/json.hpp>
#include "utils/get_points.h"
std::pair<Matrix, Matrix> loadFromJSON(const std::string& filename);



int main() {
    std::cout << "=== Генерация кластеров ===" << std::endl;
    
    // генерируем точки (по 100 в каждом кластере, идеальная прямая - y = -2x + 10)
    Dataset data = get_points(200, 200, -2, 10, randomDouble);
    std::cout << "Сгенерировано " << data.size() << " точек" << std::endl;
    
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

    std::cout << "=== Загрузка данных из JSON ===" << std::endl;

    // загружаем из файла 
    auto [X, y] = loadFromJSON("points.json");

    if (X.getRows() == 0) {
        std::cerr << "Не удалось загрузить данные. Выход." << std::endl;
        return 1;
    }
    int train_size = X.getRows() * 0.8;
    Matrix X_train = X.submatrix(0, 0, train_size, X.getCols());
    Matrix y_train = y.submatrix(0, 0, train_size, y.getCols());
    Matrix X_test = X.submatrix(train_size, 0, X.getRows() - train_size, X.getCols());
    Matrix y_test = y.submatrix(train_size, 0, y.getRows() - train_size, y.getCols());

    std::cout << "=== Обучение нейросети ===" << std::endl;
    NeuralNetwork net(6, 0.1);              // 6 нейронов, lr=0.1
    net.train(X_train, y_train, 500);       // 500 эпох

    std::cout << "\n=== Результат ===" << std::endl;
    std::cout << "Точность: " << net.accuracy(X_test, y_test) * 100 << "%" << std::endl;

    return 0;
}

