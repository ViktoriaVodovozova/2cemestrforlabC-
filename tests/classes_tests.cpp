#include <iostream>
#include <cassert>
#include "../classes/Matrix.h"
#include "../classes/Point.h"
#include "../classes/Dataset.h"

int main() {
    std::cout << "=== Тест Matrix ===" << std::endl;

    // cоздаём матрицу 2x3
    Matrix A(2, 3);
    A.randomize(-1.0, 1.0);

    // cоздаём матрицу 3x1
    Matrix B(3, 1);
    B.randomize(-1.0, 1.0);

    // умножаем матрицы
    Matrix C = A * B;

    // проверяем размеры матрицы, после умножения
    assert(C.getRows() == 2);
    assert(C.getCols() == 1);

    std::cout << "Matrix работает\n";


    std::cout << "\n=== Тест Point ===" << std::endl;

    // создаём точку
    Point p(1.5, 2.3, 1);

    // проверяем значения полей
    assert(p.x == 1.5);
    assert(p.y == 2.3);
    assert(p.label == 1);

    std::cout << "Point работает\n";


    std::cout << "\n=== Тест Dataset ===" << std::endl;

    // проверка пустого датасета
    Dataset empty;
    assert(empty.size() == 0);
    std::cout << "Dataset пуст\n";

    // создаём датасет и добавляем точки
    Dataset data;
    data.addPoint(1.0, 2.0, 1);
    data.addPoint(-1.0, 0.5, 0);

    // проверяем размер
    assert(data.size() == 2);
    std::cout << "Добавление точек работает\n";

    // проверяем получение точки
    Point p0 = data.getPoint(0);
    assert(p0.x == 1.0);
    assert(p0.y == 2.0);
    assert(p0.label == 1);
    std::cout << "getPoint работает\n";

    // преобразуем в матрицы
    Matrix X = data.toMatrix();
    Matrix Y = data.labelsToMatrix();

    // проверяем размеры матриц
    assert(X.getRows() == 2);
    assert(X.getCols() == 2);

    assert(Y.getRows() == 2);
    assert(Y.getCols() == 1);

    std::cout << "Преобразование в матрицы работает\n";

    // проверка очистки
    data.clear();
    assert(data.size() == 0);

    std::cout << "Очистка Dataset работает\n";


    std::cout << "\n=== Все тесты пройдены ===" << std::endl;

    return 0;
}