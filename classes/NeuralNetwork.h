#ifndef NEURALNETWORK_H
#define NEURALNETWORK_H

#include "Matrix.h"
#include <cmath>
#include <iostream>

// Вспомогательные функции активации
inline double sigmoid(double x) {
    return 1.0 / (1.0 + exp(-x));
}

inline double sigmoidDerivative(double x) {
    double s = sigmoid(x);
    return s * (1 - s);
}

// Простой слой для 2-слойной сети
class Layer {
private:
    Matrix weights;  // (inSize+1) × outSize
    int inSize, outSize;

public:
    Layer(int in, int out) : inSize(in), outSize(out) {
        weights = Matrix(in + 1, out);  // +1 для bias
        weights.randomize(-0.5, 0.5);   // инициализация
    }

    // Forward: input (N×in) → output (N×out)
    Matrix forward(const Matrix& input) {
        int n = input.getRows();
        // Добавляем bias-столбец (единицы)
        Matrix inputWithBias(n, inSize + 1, 1.0);
        for (int i = 0; i < n; i++)
            for (int j = 0; j < inSize; j++)
                inputWithBias.set(i, j, input.get(i, j));

        Matrix z = inputWithBias * weights;  // линейная часть
        // Применяем sigmoid поэлементно
        for (int i = 0; i < z.getRows(); i++)
            for (int j = 0; j < z.getCols(); j++)
                z.set(i, j, sigmoid(z.get(i, j)));
        return z;
    }

    // Для backprop: вернуть веса (без bias-строки)
    Matrix getWeightsNoBias() const {
        Matrix w(inSize, outSize);
        for (int i = 0; i < inSize; i++)
            for (int j = 0; j < outSize; j++)
                w.set(i, j, weights.get(i, j));
        return w;
    }

    // Обновление весов: dW (in+1)×out, db (1×out)
    void update(const Matrix& dW, const Matrix& db, double lr) {
        for (int i = 0; i < weights.getRows(); i++)
            for (int j = 0; j < weights.getCols(); j++) {
                double dw = (i < dW.getRows()) ? dW.get(i, j) : 0;
                double updateVal = lr * (dw + db.get(0, j));
                weights.set(i, j, weights.get(i, j) - updateVal);
            }
    }

    Matrix getFullWeights() const { return weights; }
    int getInSize() const { return inSize; }
    int getOutSize() const { return outSize; }
};

// ====== НЕЙРОСЕТЬ (твоя часть) ======
class NeuralNetwork {
private:
    Layer hidden, output;
    double lr;

    // Для backprop: сохраняем промежуточные значения
    Matrix lastInput, lastZ1, lastA1, lastZ2, lastA2;

public:
    NeuralNetwork(int hiddenNeurons, double learningRate = 0.1)
        : hidden(2, hiddenNeurons), output(hiddenNeurons, 1), lr(learningRate) {}

    // Прямой проход (для одного примера или батча)
    Matrix forward(const Matrix& X) {
        lastInput = X;
        lastZ1 = addBias(X) * hidden.getFullWeights();
        lastA1 = applySigmoid(lastZ1);

        lastZ2 = addBias(lastA1) * output.getFullWeights();
        lastA2 = applySigmoid(lastZ2);  // выход сети
        return lastA2;
    }

    // Обучение на одном примере (SGD)
    void trainStep(const Matrix& x, double yTrue) {
        // Forward
        Matrix pred = forward(x);  // x: 1×2, pred: 1×1

        // Backward: выходной слой
        Matrix dz2(1, 1);
        dz2.set(0, 0, pred.get(0, 0) - yTrue);  // производная BCE+sigmoid

        Matrix a1WithBias = addBias(lastA1);  // 1×(H+1)
        Matrix dW2 = a1WithBias.transpose() * dz2;  // (H+1)×1

        // Скрытый слой
        Matrix w2NoBias = output.getWeightsNoBias();  // H×1
        Matrix dz1 = dz2 * w2NoBias.transpose();  // 1×H
        // Применяем производную sigmoid
        for (int j = 0; j < dz1.getCols(); j++) {
            double a = lastA1.get(0, j);
            dz1.set(0, j, dz1.get(0, j) * a * (1 - a));
        }
        Matrix xWithBias = addBias(x);  // 1×3
        Matrix dW1 = xWithBias.transpose() * dz1;  // 3×H

        // Обновление
        hidden.update(dW1, Matrix(1, hidden.getOutSize()), lr);
        output.update(dW2, dz2, lr);
    }

    // Полный цикл обучения
    void train(const Matrix& X, const Matrix& y, int epochs) {
        for (int epoch = 0; epoch < epochs; epoch++) {
            double loss = 0;
            for (int i = 0; i < X.getRows(); i++) {
                // Одна строка = один пример
                Matrix x(1, 2);
                x.set(0, 0, X.get(i, 0));
                x.set(0, 1, X.get(i, 1));
                double target = y.get(i, 0);

                trainStep(x, target);

                // Loss для мониторинга
                double pred = forward(x).get(0, 0);
                loss -= target * log(pred + 1e-15) + (1 - target) * log(1 - pred + 1e-15);
            }
            if (epoch % 100 == 0)
                std::cout << "Epoch " << epoch << ": loss = " << loss / X.getRows() << std::endl;
        }
    }

    // Предсказание класса (0 или 1)
    int predict(const Matrix& x) {
        Matrix out = forward(x);
        return (out.get(0, 0) >= 0.5) ? 1 : 0;
    }

    // Точность на наборе данных
    double accuracy(const Matrix& X, const Matrix& y) {
        int correct = 0;
        for (int i = 0; i < X.getRows(); i++) {
            Matrix x(1, 2);
            x.set(0, 0, X.get(i, 0));
            x.set(0, 1, X.get(i, 1));
            if (predict(x) == (int)y.get(i, 0)) correct++;
        }
        return (double)correct / X.getRows();
    }

private:
    // Добавить столбец единиц (bias) к матрице
    Matrix addBias(const Matrix& m) {
        Matrix res(m.getRows(), m.getCols() + 1, 1.0);
        for (int i = 0; i < m.getRows(); i++)
            for (int j = 0; j < m.getCols(); j++)
                res.set(i, j, m.get(i, j));
        return res;
    }

    // Применить sigmoid ко всей матрице
    Matrix applySigmoid(const Matrix& m) {
        Matrix res(m.getRows(), m.getCols());
        for (int i = 0; i < m.getRows(); i++)
            for (int j = 0; j < m.getCols(); j++)
                res.set(i, j, sigmoid(m.get(i, j)));
        return res;
    }
};

#endif // NEURALNETWORK_H