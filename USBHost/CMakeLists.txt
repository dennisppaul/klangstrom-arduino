cmake_minimum_required(VERSION 3.12)

project(USBHost)

if(NOT ${KLST_ENV})
    message(FATAL_ERROR "KLST_ENV is not defined")
endif()

file(GLOB SOURCES
        "${CMAKE_CURRENT_SOURCE_DIR}/src/*.c"
        "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp"
)

add_library(USBHost ${SOURCES})

target_include_directories(USBHost PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/src
)

target_compile_definitions(USBHost PRIVATE KLST_ENV=${KLST_ENV})
