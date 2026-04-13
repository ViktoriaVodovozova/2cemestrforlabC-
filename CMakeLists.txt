cmake_minimum_required(VERSION 3.10)
project(PointGeneration)

set(CMAKE_CXX_STANDARD 17)

include(FetchContent)

FetchContent_Declare(
        json
        GIT_REPOSITORY https://github.com/nlohmann/json.git
        GIT_TAG v3.11.3
)

FetchContent_MakeAvailable(json)

# --- основная программа ---
add_executable(main
        main.cpp
        get_points.cpp
        classes/Point.cpp
        classes/Matrix.cpp
        classes/Dataset.cpp
)

target_link_libraries(main PRIVATE nlohmann_json::nlohmann_json)

# --- тесты ---
add_executable(classes_tests
        classes_tests.cpp
        classes/Point.cpp
        classes/Matrix.cpp
        classes/Dataset.cpp
)

target_link_libraries(classes_tests PRIVATE nlohmann_json::nlohmann_json)