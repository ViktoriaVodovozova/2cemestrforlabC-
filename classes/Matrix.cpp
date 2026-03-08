#include "Matrix.h"
#include <iostream>
#include <cmath>
#include <cstdlib>
#include <ctime>
#include <sstream>


Matrix::Matrix() : rows(0), cols(0) {
    // Пустая матрица 0×0
}

Matrix::Matrix(int rows, int cols) : rows(rows), cols(cols) {
    // Создаём таблицу rows×cols и заполняем нулями
    data.resize(rows, std::vector<double>(cols, 0.0));
}

Matrix::Matrix(int rows, int cols, double value) : rows(rows), cols(cols) {
    // Создаём таблицу rows×cols и заполняем значением value
    data.resize(rows, std::vector<double>(cols, value));
}


double Matrix::get(int row, int col) const {
    // Проверка на выход за границы
    if (row < 0 || row >= rows || col < 0 || col >= cols) {
        std::cerr << "Ошибка: выход за границы матрицы!" << std::endl;
        return 0.0;
    }
    return data[row][col];
}

void Matrix::set(int row, int col, double value) {
    // Проверка на выход за границы
    if (row < 0 || row >= rows || col < 0 || col >= cols) {
        std::cerr << "Ошибка: выход за границы матрицы!" << std::endl;
        return;
    }
    data[row][col] = value;
}


Matrix Matrix::operator+(const Matrix& other) const {
    // Сложение возможно только если размеры совпадают
    if (rows != other.rows || cols != other.cols) {
        std::cerr << "Ошибка: размеры матриц не совпадают для сложения!" << std::endl;
        return Matrix();
    }

    Matrix result(rows, cols);
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            // Складываем поэлементно
            result.set(i, j, data[i][j] + other.get(i, j));
        }
    }
    return result;
}

Matrix Matrix::operator-(const Matrix& other) const {
    // Вычитание возможно только если размеры совпадают
    if (rows != other.rows || cols != other.cols) {
        std::cerr << "Ошибка: размеры матриц не совпадают для вычитания!" << std::endl;
        return Matrix();
    }

    Matrix result(rows, cols);
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            result.set(i, j, data[i][j] - other.get(i, j));
        }
    }
    return result;
}

Matrix Matrix::operator*(const Matrix& other) const {
    // Умножение матриц: cols первой == rows второй
    // Пример: (2×3) * (3×1) = (2×1)
    if (cols != other.rows) {
        std::cerr << "Ошибка: размеры матриц не подходят для умножения!" << std::endl;
        return Matrix();
    }

    Matrix result(rows, other.cols);
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < other.cols; j++) {
            double sum = 0.0;
            // Скалярное произведение строки на столбец
            for (int k = 0; k < cols; k++) {
                sum += data[i][k] * other.get(k, j);
            }
            result.set(i, j, sum);
        }
    }
    return result;
}

Matrix Matrix::operator*(double scalar) const {
    // Умножение каждого элемента на число
    Matrix result(rows, cols);
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            result.set(i, j, data[i][j] * scalar);
        }
    }
    return result;
}



int Matrix::getRows() const {
    return rows;
}

int Matrix::getCols() const {
    return cols;
}

Matrix Matrix::transpose() const {
    // Меняем строки и столбцы местами
    Matrix result(cols, rows);
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            result.set(j, i, data[i][j]);
        }
    }
    return result;
}

void Matrix::fill(double value) {
    // Заполняем всю матрицу одним значением
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            data[i][j] = value;
        }
    }
}

void Matrix::randomize(double min, double max) {
    // Заполняем случайными числами от min до max
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            double random = min + (max - min) * (double)rand() / RAND_MAX;
            data[i][j] = random;
        }
    }
}

void Matrix::apply(double (*func)(double)) {
    // Применяем функцию к каждому элементу (например, sigmoid)
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            data[i][j] = func(data[i][j]);
        }
    }
}

void Matrix::print() const {
    // Выводим матрицу на экран
    std::cout << "Matrix " << rows << "x" << cols << ":" << std::endl;
    for (int i = 0; i < rows; i++) {
        std::cout << "[ ";
        for (int j = 0; j < cols; j++) {
            std::cout << data[i][j] << " ";
        }
        std::cout << "]" << std::endl;
    }
}

