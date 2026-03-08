#include <iostream>
#include "Matrix.h"
#include "Point.h"
#include "Dataset.h"

int main() {
    std::cout << "=== Тест Matrix ===" << std::endl;

    // Создаём матрицу 2x3
    Matrix A(2, 3);
    A.randomize(-1.0, 1.0);

    std::cout << "Матрица A (2x3):" << std::endl;
    A.print();

    // Создаём матрицу 3x1
    Matrix B(3, 1);
    B.randomize(-1.0, 1.0);

    std::cout << "\nМатрица B (3x1):" << std::endl;
    B.print();

    // Умножаем
    Matrix C = A * B;

    std::cout << "\nРезультат A * B (2x1):" << std::endl;
    C.print();

    // Тестируем Point
    std::cout << "\n=== Тест Point ===" << std::endl;
    Point p(1.5, 2.3, 1);
    std::cout << "Точка: x=" << p.x << ", y=" << p.y << ", label=" << p.label << std::endl;

    // Тестируем Dataset
    std::cout << "\n=== Тест Dataset ===" << std::endl;
    Dataset data;

    // Добавляем точки вручную
    data.addPoint(1.0, 2.0, 1);
    data.addPoint(-1.0, 0.5, 0);
    data.addPoint(2.0, 3.0, 1);
    data.addPoint(-0.5, -1.0, 0);

    std::cout << "Количество точек: " << data.size() << std::endl;

    // Преобразуем в матрицы
    Matrix X = data.toMatrix();
    Matrix Y = data.labelsToMatrix();

    std::cout << "\nМатрица координат X:" << std::endl;
    X.print();

    std::cout << "\nМатрица меток Y:" << std::endl;
    Y.print();

    return 0;
}