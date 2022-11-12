cmake_minimum_required (VERSION 3.8)

project ("TinyWebServer")

set(SOURCE_FILE "main.cpp"
                "config.cpp"
                "server.hpp"
                "thread_pool.hpp"
                "../http/http_parser.c")

include_directories(${CMAKE_CURRENT_SOURCE_DIR} 
                    ${CMAKE_CURRENT_SOURCE_DIR}/../common 
                    ${CMAKE_CURRENT_SOURCE_DIR}/../http)

add_executable(${PROJECT_NAME} ${SOURCE_FILE})

target_link_libraries(${PROJECT_NAME} PRIVATE tomlplusplus::tomlplusplus spdlog::spdlog boost_filesystem dl boost_system)