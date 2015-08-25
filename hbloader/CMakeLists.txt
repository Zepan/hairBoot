cmake_minimum_required(VERSION 2.8)
cmake_policy(SET CMP0015 OLD)

add_executable(hbloader main.c)
include_directories(hbloader ${CMAKE_CURRENT_SOURCE_DIR})

add_definitions(-DDEBUG)
add_definitions(-DDEBUG_LEVEL_ERROR=1 -DDEBUG_LEVEL_WARNING=2 -DDEBUG_LEVEL_INFO=3 -DDEBUG_LEVEL_TRACE=4)
add_definitions(-DDEBUG_SERIAL_DRIVE=5)

