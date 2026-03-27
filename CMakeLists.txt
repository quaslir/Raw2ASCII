cmake_minimum_required(VERSION 3.20)
project(RAW2ASCII)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if(NOT CMAKE_BUILD_TYPE)
set(CMAKE_BUILD_TYPE Release CACHE STRING "Choose the type of build" FORCE)
endif()
set(CMAKE_CXX_FLAGS_RELEASE "-O3 -march=native -DNDEBUG")


file(GLOB_RECURSE SOURCES "src/*.cpp")
file(GLOB_RECURSE HEADERS "include/*.hpp")
file(GLOB_RECURSE EXTERNAL_SOURCES "external/*.cpp")


add_library(external_libs STATIC ${EXTERNAL_SOURCES})
target_include_directories(external_libs PRIVATE external)


add_executable(${PROJECT_NAME} ${SOURCES})

target_link_libraries(${PROJECT_NAME} PRIVATE external_libs)

if(CMAKE_BUILD_TYPE STREQUAL "Debug")

message(STATUS "Debug mode: Enabling static analysis tools")


find_program(CPPCHECK_EXE NAMES "cppcheck")

if(CPPCHECK_EXE)

set(CPPCHECK_COMMAND
${CPPCHECK_EXE}
"--enable=all"
"--suppress=missingIncludeSystem"
)

set_target_properties(${PROJECT_NAME} PROPERTIES CXX_CPPCHECK "${CPPCHECK_COMMAND}")

endif()

find_program(CLANG_TIDY_EXE NAMES "clang-tidy")

if(CLANG_TIDY_EXE)



endif()

endif()






set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)



target_compile_options(${PROJECT_NAME} PRIVATE -Wall -Wextra -Wpedantic -g3 -fsanitize=address)
target_link_libraries(${PROJECT_NAME} PRIVATE -fsanitize=address)


target_include_directories(${PROJECT_NAME} PRIVATE include external)

if(APPLE)
    target_link_libraries(${PROJECT_NAME} PRIVATE "-framework CoreFoundation")
    target_link_libraries(${PROJECT_NAME} PRIVATE "-framework CoreAudio")
    target_link_libraries(${PROJECT_NAME} PRIVATE "-framework AudioToolbox")
endif()

find_package(PkgConfig REQUIRED)

set(FFMPEG_MODULES libavcodec libavformat libavutil libswscale libswresample)

foreach(module ${FFMPEG_MODULES})
    pkg_check_modules(${module} REQUIRED ${module})
    target_include_directories(${PROJECT_NAME} PRIVATE ${${module}_INCLUDE_DIRS})

    target_link_libraries(${PROJECT_NAME} PRIVATE ${${module}_LIBRARIES})

    target_link_directories(${PROJECT_NAME} PRIVATE ${${module}_LIBRARY_DIRS})
endforeach()

file(GLOB_RECURSE ALL_FILES *.cpp *.hpp)

find_program(CLANG_FORMAT_EXE NAMES "clang-format")

if(CLANG_FORMAT_EXE)

add_custom_target(format
COMMAND ${CLANG_FORMAT_EXE} -i -style=LLVM ${ALL_FILES}
COMMENT "Formatting all source files with clang format..."
)



endif()




