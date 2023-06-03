cmake_minimum_required(VERSION 3.0)
project(Tic-Tac-Toe)

set(CMAKE_CXX_STANDARD 11)

add_executable(Tic-Tac-Toe main.cpp ../src/client.cpp ../src/host.cpp ../src/game.cpp)

target_include_directories(Tic-Tac-Toe PRIVATE ../include)

add_custom_target(CLEAN COMMAND ${CMAKE_COMMAND} -E remove_directory ${CMAKE_BINARY_DIR}/CMakeFiles ${CMAKE_BINARY_DIR}/Makefile ${CMAKE_BINARY_DIR}/CMakeCache.txt)
