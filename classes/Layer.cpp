#include "Layer.h"
#include <iostream>

Layer::Layer(int in_features, int out_features, std::string activation)
    : in_features(in_features), out_features(out_features), activation(activation) {

    // Инициализация весов (Xavier initialization)
    weights = Matrix(in_features, out_features);
    double scale = std::sqrt(2.0 / (in_features + out_features));
    weights.randomize(-scale, scale);

    // Инициализация смещений нулями
    bias = Matrix(1, out_features, 0.0);
}

double Layer::relu(double x) {
    return (x > 0) ? x : 0.0;
}

double Layer::sigmoid(double x) {
    return 1.0 / (1.0 + std::exp(-x));
}

double Layer::reluDerivative(double x) {
    return (x > 0) ? 1.0 : 0.0;
}

double Layer::sigmoidDerivative(double x) {
    double s = sigmoid(x);
    return s * (1 - s);
}

Matrix Layer::applyActivation(const Matrix& m) {
    Matrix result(m.getRows(), m.getCols());
    for (int i = 0; i < m.getRows(); i++) {
        for (int j = 0; j < m.getCols(); j++) {
            double val = m.get(i, j);
            if (activation == "relu") {
                result.set(i, j, relu(val));
            } else if (activation == "sigmoid") {
                result.set(i, j, sigmoid(val));
            } else {
                result.set(i, j, val); // linear
            }
        }
    }
    return result;
}

Matrix Layer::applyActivationDerivative(const Matrix& m) {
    Matrix result(m.getRows(), m.getCols());
    for (int i = 0; i < m.getRows(); i++) {
        for (int j = 0; j < m.getCols(); j++) {
            double val = m.get(i, j);
            if (activation == "relu") {
                result.set(i, j, reluDerivative(val));
            } else if (activation == "sigmoid") {
                result.set(i, j, sigmoidDerivative(val));
            } else {
                result.set(i, j, 1.0); // linear
            }
        }
    }
    return result;
}

Matrix Layer::forward(const Matrix& input) {
    // Сохраняем вход для backprop
    last_input = input;

    // Z = X * W + b (с broadcasting для bias)
    Matrix Z = input * weights;

    // Добавляем bias к каждой строке (broadcasting)
    for (int i = 0; i < Z.getRows(); i++) {
        for (int j = 0; j < Z.getCols(); j++) {
            double b = bias.get(0, j);
            Z.set(i, j, Z.get(i, j) + b);
        }
    }

    // Сохраняем Z для backprop
    last_Z = Z;

    // Применяем функцию активации
    Matrix A = applyActivation(Z);

    return A;
}

Matrix Layer::backward(const Matrix& upstream_gradient, double learning_rate) {
    // 1. Локальный градиент: delta = upstream_gradient ⊙ f'(Z)
    Matrix dZ = Matrix(upstream_gradient.getRows(), upstream_gradient.getCols());
    Matrix activation_deriv = applyActivationDerivative(last_Z);

    // Поэлементное умножение (Hadamard product)
    for (int i = 0; i < upstream_gradient.getRows(); i++) {
        for (int j = 0; j < upstream_gradient.getCols(); j++) {
            double grad = upstream_gradient.get(i, j);
            double deriv = activation_deriv.get(i, j);
            dZ.set(i, j, grad * deriv);
        }
    }

    // 2. Градиент по весам: dW = X^T * delta
    Matrix dW = last_input.transpose() * dZ;

    // 3. Градиент по bias: db = sum(delta по строкам)
    Matrix db(1, out_features, 0.0);
    for (int j = 0; j < dZ.getCols(); j++) {
        double sum = 0.0;
        for (int i = 0; i < dZ.getRows(); i++) {
            sum += dZ.get(i, j);
        }
        db.set(0, j, sum);
    }

    // 4. Обновление весов: W = W - lr * dW
    for (int i = 0; i < weights.getRows(); i++) {
        for (int j = 0; j < weights.getCols(); j++) {
            double w = weights.get(i, j);
            double dw = dW.get(i, j);
            weights.set(i, j, w - learning_rate * dw);
        }
    }

    // 5. Обновление bias: b = b - lr * db
    for (int j = 0; j < bias.getCols(); j++) {
        double b = bias.get(0, j);
        double db_val = db.get(0, j);
        bias.set(0, j, b - learning_rate * db_val);
    }

    // 6. Градиент для предыдущего слоя: dX = delta * W^T
    Matrix downstream_gradient = dZ * weights.transpose();

    return downstream_gradient;
}