cmake_minimum_required(VERSION 3.11)

project(tiny_server)

add_executable(${PROJECT_NAME} main.c tiny_err.c)

target_include_directories(${PROJECT_NAME} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include)