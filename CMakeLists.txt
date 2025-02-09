cmake_minimum_required(VERSION 3.22)
project(http_server)

set(CMAKE_CXX_STANDARD 20)

add_executable(http_server main.cpp
        src/TCPServer.cpp
        src/TCPServer.h
        src/HTTPServer.cpp
        src/HTTPServer.h)
