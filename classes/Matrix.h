#ifndef MATRIX_H
#define MATRIX_H

#include <vector>
#include <string>

// класс Matrix - это двумерная таблица чисел
// нужен для всех вычислений в нейронке (X * W + b)
class Matrix {
private:
    int rows;                    // количество строк
    int cols;                    // количество столбцов
    std::vector<std::vector<double>> data;  // Сама таблица чисел

public:

    Matrix();                    // пустая матрица 0×0
    Matrix(int rows, int cols);  // матрица rows×cols, заполненная нулями
    Matrix(int rows, int cols, double value);  // матрица с заполнением value

    // ========== ДОСТУП К ЭЛЕМЕНТАМ ==========
    double get(int row, int col) const;   // получить элемент [row][col]
    void set(int row, int col, double value);  // установить элемент


    Matrix operator+(const Matrix& other) const;  // сложение A + B
    Matrix operator-(const Matrix& other) const;  // вычитание A - B
    Matrix operator*(const Matrix& other) const;  // умножение A * B
    Matrix operator*(double scalar) const;         // умножение на число A * 2

    int getRows() const;         // вернуть количество строк
    int getCols() const;         // вернуть количество столбцов
    Matrix transpose() const;    // транспонирование (строки ↔ столбцы)
    void fill(double value);     // заполнить всю матрицу значением
    void randomize(double min, double max);  // заполнить случайными числами
    void apply(double (*func)(double));      // применить функцию к каждому элементу
    void print() const;          // вывести на экран
    std::string toString() const; // вернуть как строку
};

#endif // MATRIX_H