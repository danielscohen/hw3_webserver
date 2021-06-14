cmake_minimum_required(VERSION 3.10)
project(hw3_webserver C)

set(CMAKE_C_STANDARD 99)

set(CMAKE_THREAD_PREFER_PTHREAD TRUE)
set(THREADS_PREFER_PTHREAD_FLAG TRUE)
find_package(Threads REQUIRED)
add_executable(hw3_webserver request.c segel.c server.c queue.c)
target_link_libraries(hw3_webserver Threads::Threads)