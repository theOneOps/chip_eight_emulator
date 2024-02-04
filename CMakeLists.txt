cmake_minimum_required(VERSION 3.26)
project(chip_eight C)

set(CMAKE_C_STANDARD 11)
set(CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/cmake_modules)

set(SDL2_PATH "C:/Development/SDL2/SDL2-devel-2.28.5-mingw/SDL2-2.28.5/x86_64-w64-mingw32")

find_package(SDL2 REQUIRED)


add_executable(chip_eight main.c
        headers/cpu.h
        headers/ram.h
        headers/emulator.h
        bodies/ram.c
        bodies/cpu.c
        bodies/emulator.c)

include_directories(${SDL2_INCLUDE_DIR})
include_directories(${CMAKE_SOURCE_DIR}/libprovided/include)
target_link_libraries(${PROJECT_NAME} ${SDL2_LIBRARY} ${CMAKE_SOURCE_DIR}/libprovided/lib/release/x64/libprovided.a
        -lm)