cmake_minimum_required(VERSION 3.22)

project(asl)

add_library(asl INTERFACE)

add_subdirectory(libs/rtt)
add_subdirectory(libs/align)

add_custom_command(POST_BUILD
    COMMAND ${CMAKE_CURRENT_SOURCE_DIR}/programs/strtt/windows_bin_64/strtt.exe -tcp -v 4
)

target_link_libraries(asl INTERFACE
    rtt
    align
)
