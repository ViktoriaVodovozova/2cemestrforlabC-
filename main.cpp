#include <iostream>
#include <cmath>
#include "classes/Dataset.h"
#include "classes/Matrix.h"
#include "classes/NeuralNetwork.h"
#include <nlohmann/json.hpp>
#include "get_points.h"
std::pair<Matrix, Matrix> loadFromJSON(const std::string& filename);



int main() {
    std::cout << "=== Загрузка данных из JSON ===" << std::endl;

    // загружаем из файла (путь относительно папки, откуда запускаешь)
    auto [X, y] = loadFromJSON("points.json");

    if (X.getRows() == 0) {
        std::cerr << "Не удалось загрузить данные. Выход." << std::endl;
        return 1;
    }

    std::cout << "=== Обучение нейросети ===" << std::endl;
    NeuralNetwork net(6, 0.1);  // 6 нейронов, lr=0.1
    net.train(X, y, 500);       // 500 эпох

    std::cout << "\n=== Результат ===" << std::endl;
    std::cout << "Точность: " << net.accuracy(X, y) * 100 << "%" << std::endl;

    return 0;
}

