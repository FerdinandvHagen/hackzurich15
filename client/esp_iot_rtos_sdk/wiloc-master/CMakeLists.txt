# This file is not used for building!
# Required by CLion IDE.

cmake_minimum_required(VERSION 3.2)
project(uart_tcp_bridge)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(SOURCE_FILES
    driver/uart.c
    user/user_main.c
    user/gpio.c
    user/itstr.c
    user/user_wifi.c
    user/ringbuf.c
    )

include_directories(include)
include_directories(../wiloc-common/include)
include_directories(../extra_include)
include_directories(../include)
include_directories(../include/espressif)
include_directories(../include/lwip)
include_directories(../include/lwip/ipv4)
include_directories(../include/lwip/ipv6)

add_executable(uart_tcp_bridge ${SOURCE_FILES})