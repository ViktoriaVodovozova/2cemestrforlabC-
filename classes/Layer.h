#ifndef LAYER_H
#define LAYER_H

#include "Matrix.h"
#include <string>
#include <cmath>

class Layer {
private:
    Matrix weights;        // W: in_features × out_features
    Matrix bias;           // b: 1 × out_features
    std::string activation; // "relu" или "sigmoid"

    // Для backprop: сохраняем вход и линейную комбинацию
    Matrix last_input;
    Matrix last_Z;

    int in_features;
    int out_features;

public:
    // Конструктор
    Layer(int in_features, int out_features, std::string activation = "sigmoid");

    // Прямой проход
    Matrix forward(const Matrix& input);

    // Обратный проход (возвращает градиент для предыдущего слоя)
    Matrix backward(const Matrix& upstream_gradient, double learning_rate);

    // Геттеры для весов (нужны для сохранения)
    Matrix getWeights() const { return weights; }
    Matrix getBias() const { return bias; }

    // Сеттеры для весов (нужны для загрузки)
    void setWeights(const Matrix& w) { weights = w; }
    void setBias(const Matrix& b) { bias = b; }

    // Размерности
    int getInFeatures() const { return in_features; }
    int getOutFeatures() const { return out_features; }

private:
    // Функции активации
    double relu(double x);
    double sigmoid(double x);
    double reluDerivative(double x);
    double sigmoidDerivative(double x);

    // Применение активации к матрице
    Matrix applyActivation(const Matrix& m);
    Matrix applyActivationDerivative(const Matrix& m);
};

#endif // LAYER_H