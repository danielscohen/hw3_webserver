cmake_minimum_required(VERSION 3.10)
project(hw3_webserver C)

set(CMAKE_C_STANDARD 99)

add_executable(hw3_webserver main.c request.c segel.c server.c output.c)