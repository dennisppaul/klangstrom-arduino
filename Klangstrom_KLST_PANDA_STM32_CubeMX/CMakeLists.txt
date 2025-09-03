cmake_minimum_required(VERSION 3.12)

project(Klangstrom_KLST_PANDA_STM32_CubeMX)

if(NOT ${KLST_ENV})
    message(FATAL_ERROR "KLST_ENV is not defined")
endif ()

file(GLOB SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/src/*.c")
add_library(Klangstrom_KLST_PANDA_STM32_CubeMX ${SOURCES})

target_include_directories(Klangstrom_KLST_PANDA_STM32_CubeMX PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/src
)

target_compile_definitions(Klangstrom_KLST_PANDA_STM32_CubeMX PRIVATE KLST_ENV=${KLST_ENV})
target_link_libraries(Klangstrom_KLST_PANDA_STM32_CubeMX PRIVATE klangstrom)
