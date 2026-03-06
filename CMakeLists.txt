cmake_minimum_required(VERSION 4.0)
project(Lab1_)

set(CMAKE_CXX_STANDARD 20)

add_executable(Lab1_
        main.cpp
        my_part_classes/Matrix.cpp
        my_part_classes/Point.cpp
        my_part_classes/Dataset.cpp
)

target_include_directories(Lab1_ PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/my_part_classes)