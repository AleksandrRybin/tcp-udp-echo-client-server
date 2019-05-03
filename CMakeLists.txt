cmake_minimum_required (VERSION 3.8)

project(tcp-udp-echo-client-server)

set(CMAKE_CXX_STANDARD 17)

set(CMAKE_CXX_STANDARD_REQUIRED on)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Werror")

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/bin)

include_directories(inc)

add_executable(server src/server.cpp src/digits.cpp)

add_executable(client src/client.cpp)
