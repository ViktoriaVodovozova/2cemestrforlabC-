#include "NeuralNetwork.h"
#include <iostream>
#include <cmath>

NeuralNetwork::NeuralNetwork(std::vector<int> topology, std::vector<std::string> activations) {
    if (topology.size() < 2) {
        std::cerr << "Ошибка: топология должна содержать минимум 2 слоя" << std::endl;
        return;
    }
    
    if (activations.size() != topology.size() - 1) {
        std::cerr << "Ошибка: количество активаций должно быть на 1 меньше количества слоёв" << std::endl;
        return;
    }
    
    // Создаём слои
    for (size_t i = 0; i < topology.size() - 1; i++) {
        layers.emplace_back(topology[i], topology[i + 1], activations[i]);
    }
    
    std::cout << " Нейросеть создана: ";
    for (size_t i = 0; i < topology.size(); i++) {
        std::cout << topology[i];
        if (i < topology.size() - 1) std::cout << " → ";
    }
    std::cout << std::endl;
}

Matrix NeuralNetwork::forward(const Matrix& input) {
    Matrix output = input;
    for (auto& layer : layers) {
        output = layer.forward(output);
    }
    return output;
}

void NeuralNetwork::backward(const Matrix& target, double learning_rate) {
    // Начинаем с последнего слоя
    Matrix error = target;  // будет перезаписано
    
    // Проходим слои в обратном порядке
    for (int i = layers.size() - 1; i >= 0; i--) {
        // Для последнего слоя: error = prediction - target
        if (i == (int)layers.size() - 1) {
            // Получаем выход последнего слоя (нужно сохранить в forward)
            // Упрощённо: считаем, что последний forward уже был
            error = error;
        }
        
        error = layers[i].backward(error, learning_rate);
    }
}

void NeuralNetwork::train(const Matrix& X, const Matrix& y, int epochs, double lr) {
    for (int epoch = 0; epoch < epochs; epoch++) {
        double total_loss = 0.0;
        
        for (int i = 0; i < X.getRows(); i++) {
            // Одна строка = один пример
            Matrix x(1, X.getCols());
            for (int j = 0; j < X.getCols(); j++) {
                x.set(0, j, X.get(i, j));
            }
            
            double target = y.get(i, 0);
            
            // Forward
            Matrix prediction = forward(x);
            double pred_val = prediction.get(0, 0);
            
            // Loss (Binary Cross-Entropy)
            double loss = -target * std::log(pred_val + 1e-15) - (1 - target) * std::log(1 - pred_val + 1e-15);
            total_loss += loss;
            
            // Backward
            Matrix error(1, 1);
            error.set(0, 0, pred_val - target);
            
            for (int l = layers.size() - 1; l >= 0; l--) {
                error = layers[l].backward(error, lr);
            }
        }
        
        if (epoch % 100 == 0) {
            std::cout << "Epoch " << epoch << ": loss = " << (total_loss / X.getRows()) << std::endl;
        }
    }
}

double NeuralNetwork::predict_proba(const Matrix& x) {
    Matrix output = forward(x);
    return output.get(0, 0);
}

int NeuralNetwork::predict(const Matrix& x) {
    double prob = predict_proba(x);
    return (prob >= 0.5) ? 1 : 0;
}

void NeuralNetwork::save_weights(const std::string& filename) {
    nlohmann::json j;
    
    for (size_t i = 0; i < layers.size(); i++) {
        std::string layer_key = "layer_" + std::to_string(i);
        
        // Сохраняем веса
        Matrix w = layers[i].getWeights();
        std::vector<std::vector<double>> weights_data;
        for (int row = 0; row < w.getRows(); row++) {
            std::vector<double> row_data;
            for (int col = 0; col < w.getCols(); col++) {
                row_data.push_back(w.get(row, col));
            }
            weights_data.push_back(row_data);
        }
        j[layer_key]["weights"] = weights_data;
        
        // Сохраняем bias
        Matrix b = layers[i].getBias();
        std::vector<double> bias_data;
        for (int col = 0; col < b.getCols(); col++) {
            bias_data.push_back(b.get(0, col));
        }
        j[layer_key]["bias"] = bias_data;
    }
    
    std::ofstream file(filename);
    file << j.dump(4);
    file.close();
    
    std::cout << " веса сохранены в " << filename << std::endl;
}

void NeuralNetwork::load_weights(const std::string& filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << " не удалось открыть файл: " << filename << std::endl;
        return;
    }
    
    nlohmann::json j;
    file >> j;
    file.close();
    
    for (size_t i = 0; i < layers.size(); i++) {
        std::string layer_key = "layer_" + std::to_string(i);
        
        // Загружаем веса
        auto& w_json = j[layer_key]["weights"];
        int rows = w_json.size();
        int cols = w_json[0].size();
        Matrix w(rows, cols);
        for (int row = 0; row < rows; row++) {
            for (int col = 0; col < cols; col++) {
                w.set(row, col, w_json[row][col]);
            }
        }
        layers[i].setWeights(w);
        
        // Загружаем bias
        auto& b_json = j[layer_key]["bias"];
        Matrix b(1, b_json.size());
        for (size_t col = 0; col < b_json.size(); col++) {
            b.set(0, col, b_json[col]);
        }
        layers[i].setBias(b);
    }
    
    std::cout << " веса загружены из " << filename << std::endl;
}