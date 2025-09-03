cmake_minimum_required(VERSION 3.12)

project(klangstrom)

if(NOT ${KLST_ENV})
    message(FATAL_ERROR "KLST_ENV is not defined")
endif()

file(GLOB SOURCES
        "${CMAKE_CURRENT_SOURCE_DIR}/src/*.c"
        "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp"
)

add_library(klangstrom ${SOURCES})

target_include_directories(klangstrom PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/src
)

target_compile_definitions(klangstrom PRIVATE KLST_ENV=${KLST_ENV})
