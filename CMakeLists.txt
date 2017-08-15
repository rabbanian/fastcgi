cmake_minimum_required(VERSION 3.8)
project(fastcgi)

set(CMAKE_CXX_STANDARD 14)
find_package (Threads)

set(SOURCE_FILES main.cpp connection.cpp request.cpp server.cpp responder.cpp)
add_executable(fastcgi ${SOURCE_FILES})

target_link_libraries (fastcgi ${CMAKE_THREAD_LIBS_INIT})