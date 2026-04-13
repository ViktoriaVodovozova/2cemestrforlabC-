#include <iostream>
#include <nlohmann/json.hpp>
#include <fstream>           // для std::ifstream
#include <nlohmann/json.hpp> // для JSON
#include <utility>           // для std::pair
#include "classes/Matrix.h"  // для Matrix
using json = nlohmann::json;

// Загружает данные из JSON и возвращает матрицы признаков и меток
std::pair<Matrix, Matrix> loadFromJSON(const std::string& filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Ошибка: не удалось открыть файл " << filename << std::endl;
        return {Matrix(0, 2), Matrix(0, 1)};
    }

    json j;
    file >> j;
    file.close();

    int n = j["points"].size();
    Matrix X(n, 2);  // признаки: x, y
    Matrix y(n, 1);  // метки: 0 или 1

    for (int i = 0; i < n; i++) {
        double x = j["points"][i]["x"];
        double yy = j["points"][i]["y"];  // yy, чтобы не путать с меткой y
        int label = j["points"][i]["label"];

        // Нормализация: делим на 100, чтобы числа были ~[-2, 2]
        X.set(i, 0, x / 100.0);
        X.set(i, 1, yy / 100.0);
        y.set(i, 0, label);
    }

    std::cout << "Загружено " << n << " точек из " << filename << std::endl;
    return {X, y};
}