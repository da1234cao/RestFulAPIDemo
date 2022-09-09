cmake_minimum_required(VERSION 3.11)

project(tiny_server)

set(SOURCE_FILE
    ${CMAKE_CURRENT_SOURCE_DIR}/main.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/tiny_err.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/rio.cpp)

add_executable(${PROJECT_NAME}  ${SOURCE_FILE})

target_include_directories(${PROJECT_NAME} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include)