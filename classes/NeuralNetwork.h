#ifndef NEURALNETWORK_H
#define NEURALNETWORK_H

#include "Layer.h"
#include "Matrix.h"
#include <vector>
#include <string>
#include <fstream>
#include <nlohmann/json.hpp>

class NeuralNetwork {
private:
    std::vector<Layer> layers;

public:
    // Конструктор: topology = [2, 16, 1], activations = ["relu", "sigmoid"]
    NeuralNetwork(std::vector<int> topology, std::vector<std::string> activations);

    // Прямой проход
    Matrix forward(const Matrix& input);

    // Обратный проход
    void backward(const Matrix& target, double learning_rate);

    // Обучение
    void train(const Matrix& X, const Matrix& y, int epochs, double lr = 0.1);

    // Предсказание (возвращает вероятность)
    double predict_proba(const Matrix& x);

    // Предсказание класса (0 или 1)
    int predict(const Matrix& x);

    // Сохранение весов в JSON
    void save_weights(const std::string& filename);

    // Загрузка весов из JSON
    void load_weights(const std::string& filename);
};

#endif // NEURALNETWORK_H