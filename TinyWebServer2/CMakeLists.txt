cmake_minimum_required (VERSION 3.8)

project ("TinyWebServer")

SET(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)
# if (CMAKE_CXX_COMPILER_ID MATCHES "MSVC")
#   message(STATUS "compile with MSVC.set the VS_DEBUGGER_WORKING_DIRECTORY")
#   set_target_properties(${PROJECT_NAME} PROPERTIES VS_DEBUGGER_WORKING_DIRECTORY  "${EXECUTABLE_OUTPUT_PATH}")
# endif()

# boost
find_package(Boost REQUIRED
             COMPONENTS json)
include_directories(${Boost_INCLUDE_DIRS})
link_directories(${Boost_LIBRARY_DIRS})

include(FetchContent)

#spdlog
## github访问较慢，使用gitee代替
FetchContent_Declare(
        spdlog
        GIT_REPOSITORY https://gitee.com/moondaisy/spdlog
        GIT_TAG        v1.x
)
FetchContent_MakeAvailable(spdlog)
include_directories(${spdlog_SOURCE_DIR}/include)
message("fetch spdlog from gitee instead of github")

# toml: 
FetchContent_Declare(
    tomlplusplus
    GIT_REPOSITORY https://github.com/marzer/tomlplusplus.git
    GIT_TAG        v3.1.0
)
FetchContent_MakeAvailable(tomlplusplus)
include_directories(${tomlplusplus_SOURCE_DIR}/include)
message("fetch toml from gitee instead of github")

include_directories(${CMAKE_CURRENT_SOURCE_DIR})

ADD_SUBDIRECTORY(src)