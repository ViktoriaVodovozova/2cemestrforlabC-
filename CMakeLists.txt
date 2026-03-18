cmake_minimum_required(VERSION 3.10)
project(PointGeneration)

set(CMAKE_CXX_STANDARD 17)

# указываем путь к vcpkg 
set(CMAKE_PREFIX_PATH "/Users/yowlxc/itmo/cpp/vcpkg/installed/arm64-osx" ${CMAKE_PREFIX_PATH})

# находим библиотеку
find_package(nlohmann_json CONFIG REQUIRED)

add_executable(main
    main.cpp
    get_points.cpp
    classes/Point.cpp
    classes/Matrix.cpp
    classes/Dataset.cpp
)

# линкуем библиотеку
target_link_libraries(main PRIVATE nlohmann_json::nlohmann_json)