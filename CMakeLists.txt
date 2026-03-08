cmake_minimum_required(VERSION 3.10)
project(PointGeneration)

set(CMAKE_CXX_STANDARD 17)

# Указываем путь к vcpkg (если ещё не настроен глобально)
set(CMAKE_PREFIX_PATH "/Users/yowlxc/itmo/cpp/vcpkg/installed/arm64-osx" ${CMAKE_PREFIX_PATH})

# Находим библиотеку
find_package(nlohmann_json CONFIG REQUIRED)

add_executable(point_generation
    point_generation.cpp
    classes/Point.cpp
    classes/Matrix.cpp
    classes/Dataset.cpp
)

# Линкуем библиотеку
target_link_libraries(point_generation PRIVATE nlohmann_json::nlohmann_json)