cmake_minimum_required(VERSION 3.20)

if(APPLE)
    set(CMAKE_C_COMPILER "/opt/homebrew/bin/gcc-15")
    set(CMAKE_CXX_COMPILER "/opt/homebrew/bin/g++-15")
endif()

project(RAW2ASCII)

file(GLOB_RECURSE SOURCES "src/*.cpp")
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
add_definitions(-D_LIBCPP_HAS_NO_INCOMPLETE_FORMAT)
add_executable(${PROJECT_NAME} ${SOURCES})

target_compile_options(${PROJECT_NAME} PRIVATE -Wall -Wextra -Wpedantic)

target_compile_features(${PROJECT_NAME} PRIVATE cxx_std_20)

target_include_directories(${PROJECT_NAME} PRIVATE include external /opt/homebrew/include)