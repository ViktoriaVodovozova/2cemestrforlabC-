#ifndef MATRIX_H
#define MATRIX_H

#include <vector>
#include <string>

// Класс Matrix - это двумерная таблица чисел
// Нужен для всех вычислений в нейронке (X * W + b)
class Matrix {
private:
    int rows;                    // Количество строк
    int cols;                    // Количество столбцов
    std::vector<std::vector<double>> data;  // Сама таблица чисел

public:

    Matrix();                    // Пустая матрица 0×0
    Matrix(int rows, int cols);  // Матрица rows×cols, заполненная нулями
    Matrix(int rows, int cols, double value);  // Матрица с заполнением value

    // ========== ДОСТУП К ЭЛЕМЕНТАМ ==========
    double get(int row, int col) const;   // Получить элемент [row][col]
    void set(int row, int col, double value);  // Установить элемент


    Matrix operator+(const Matrix& other) const;  // Сложение A + B
    Matrix operator-(const Matrix& other) const;  // Вычитание A - B
    Matrix operator*(const Matrix& other) const;  // Умножение A * B
    Matrix operator*(double scalar) const;         // Умножение на число A * 2

    int getRows() const;         // Вернуть количество строк
    int getCols() const;         // Вернуть количество столбцов
    Matrix transpose() const;    // Транспонирование (строки ↔ столбцы)
    void fill(double value);     // Заполнить всю матрицу значением
    void randomize(double min, double max);  // Заполнить случайными числами
    void apply(double (*func)(double));      // Применить функцию к каждому элементу
    void print() const;          // Вывести на экран
    std::string toString() const; // Вернуть как строку
};

#endif // MATRIX_H