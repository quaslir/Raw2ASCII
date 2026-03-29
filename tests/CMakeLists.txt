cmake_minimum_required(VERSION 3.20)
project(RAW2ASCII_TESTS)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include(FetchContent)

FetchContent_Declare(
googletest
URL https://github.com/google/googletest/archive/03597a01ee50ed33e9dfd640b249b4be3799d395.zip
)

FetchContent_MakeAvailable(googletest)

add_executable(rgb_test ../src/rgb.cpp ../src/utils.cpp src/test_rgb.cpp)
target_link_libraries(rgb_test PRIVATE gtest_main gmock)
target_compile_options(rgb_test PRIVATE -Wall -Wextra -Wpedantic)
target_include_directories(rgb_test PRIVATE ../include)

add_executable(utils_test ../src/rgb.cpp ../src/utils.cpp src/test_utils.cpp)
target_link_libraries(utils_test PRIVATE gtest_main gmock)
target_compile_options(utils_test PRIVATE -Wall -Wextra -Wpedantic -g3)
target_include_directories(utils_test PRIVATE ../include)

add_executable(eight_bit_test ../src/rgb.cpp ../src/utils.cpp ../src/color8bit.cpp src/test_eight_bits.cpp)
target_link_libraries(eight_bit_test PRIVATE gtest_main gmock)
target_compile_options(eight_bit_test PRIVATE -Wall -Wextra -Wpedantic -g3)
target_include_directories(eight_bit_test PRIVATE ../include)