cmake_minimum_required(VERSION 3.5.0)

project(socket_sample)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall")
set(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -ggdb3 -O0")
set(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -ggdb3 -O3")
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -ggdb3 -O0")
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -ggdb3 -O3")


include_directories(
    ${CMAKE_CURRENT_SOURCE_DIR}
)

add_executable(
    tcp_server
    ${CMAKE_CURRENT_SOURCE_DIR}/tcp_server.c
)

add_executable(
    tcp_client
    ${CMAKE_CURRENT_SOURCE_DIR}/tcp_client.c
)

