cmake_minimum_required(VERSION 3.22)

project(rtt)

add_library(rtt INTERFACE)

target_include_directories(rtt INTERFACE
    ${CMAKE_CURRENT_SOURCE_DIR}/repo/RTT/
    ${CMAKE_CURRENT_SOURCE_DIR}/repo/Config/
)

target_sources(rtt INTERFACE
    ${CMAKE_CURRENT_SOURCE_DIR}/repo/RTT/SEGGER_RTT.c
    ${CMAKE_CURRENT_SOURCE_DIR}/repo/RTT/SEGGER_RTT_printf.c
)