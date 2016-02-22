cmake_minimum_required(VERSION 2.8)

project(ct_examples)

include_directories("${PROJECT_SOURCE_DIR}/src")

add_executable(ct_is_function_0 ${PROJECT_SOURCE_DIR}/src/is_function_0.cpp)
add_executable(ct_is_function_1 ${PROJECT_SOURCE_DIR}/src/is_function_1.cpp)
add_executable(ct_is_function_2 ${PROJECT_SOURCE_DIR}/src/is_function_2.cpp)