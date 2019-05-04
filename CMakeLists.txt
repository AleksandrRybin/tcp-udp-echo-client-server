cmake_minimum_required (VERSION 3.8)

project(tcp-udp-echo-client-server)

set(CMAKE_CXX_STANDARD 11)

set(CMAKE_CXX_STANDARD_REQUIRED on)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall")

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/bin)

include_directories(inc)

add_library(sockets STATIC src/sockets.cpp)

add_library(numbers STATIC src/numbers.cpp)

# server program
add_executable(server src/server.cpp)

# client program
add_executable(client src/client.cpp)

target_link_libraries(server sockets numbers)

target_link_libraries(client sockets)