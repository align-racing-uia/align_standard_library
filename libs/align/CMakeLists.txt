cmake_minimum_required(VERSION 3.22)

project(align)

add_library(align INTERFACE)

target_include_directories(align INTERFACE
    ${CMAKE_CURRENT_SOURCE_DIR}/inc/
    ${CMAKE_CURRENT_SOURCE_DIR}/inc/stm32g4/
)

target_sources(align INTERFACE
    ${CMAKE_CURRENT_SOURCE_DIR}/src/align.cpp
)