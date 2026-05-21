#include <pybind11/pybind11.h>
#include <pybind11/numpy.h>
#include <pybind11/stl.h>
#include "NeuralNetwork.h"

namespace py = pybind11;

//  Вспомогательная функция: конвертация numpy array -> Matrix
Matrix numpy_to_matrix(py::array_t<double> arr) {
    auto buf = arr.request();
    if (buf.ndim != 2) throw std::runtime_error("Ожидается 2D массив (NxM)");
    int rows = buf.shape[0];
    int cols = buf.shape[1];
    Matrix m(rows, cols);
    auto ptr = static_cast<double*>(buf.ptr);
    // Копируем данные построчно (предполагаем C-contiguous layout)
    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < cols; ++j) {
            m.set(i, j, ptr[i * cols + j]);
        }
    }
    return m;
}

PYBIND11_MODULE(anaconda_nn, m) {
    py::class_<NeuralNetwork>(m, "NeuralNetwork")
        .def(py::init<std::vector<int>, std::vector<std::string>>())

        // train принимает numpy массивы, конвертирует в Matrix и вызывает C++ train
        .def("train", [](NeuralNetwork& net,
                         py::array_t<double> X,
                         py::array_t<double> y,
                         int epochs,
                         double lr) {
            net.train(numpy_to_matrix(X), numpy_to_matrix(y), epochs, lr);
        })

        // predict принимает numpy массив (Nx2), возвращает список классов [0, 1, 0, ...]
        .def("predict", [](NeuralNetwork& net, py::array_t<double> X) {
            Matrix X_mat = numpy_to_matrix(X);
            std::vector<int> results(X_mat.getRows());
            for (int i = 0; i < X_mat.getRows(); ++i) {
                Matrix row(1, X_mat.getCols());
                for (int j = 0; j < X_mat.getCols(); ++j)
                    row.set(0, j, X_mat.get(i, j));
                results[i] = net.predict(row);
            }
            return results;
        })
        
        .def("save_weights", &NeuralNetwork::save_weights)
        .def("load_weights", &NeuralNetwork::load_weights);
}