cmake_minimum_required(VERSION 3.10)
project(PointGeneration)

set(CMAKE_CXX_STANDARD 17)

# Находим pybind11 и JSON
find_package(pybind11 CONFIG REQUIRED)
find_package(nlohmann_json CONFIG REQUIRED)

# Собираем Python-модуль (вместо executable)
pybind11_add_module(anaconda_nn
        classes/Matrix.cpp
        classes/Point.cpp
        classes/Dataset.cpp
        classes/Layer.cpp
        classes/NeuralNetwork.cpp
        classes/pybind_wrapper.cpp   # ← этот файл создадим ниже
        utils/json_loader.cpp
)

# Линкуем JSON (если используется в NeuralNetwork)
target_link_libraries(anaconda_nn PRIVATE nlohmann_json::nlohmann_json)