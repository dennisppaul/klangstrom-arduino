cmake_minimum_required(VERSION 3.12)

project(Klangstrom_KLST_PANDA_STM32)

if(NOT ${KLST_ENV})
    message(FATAL_ERROR "KLST_ENV is not defined")
endif ()

file(GLOB SOURCE_FILES
        "${CMAKE_CURRENT_SOURCE_DIR}/src/*.c"
        "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp"
)
add_library(Klangstrom_KLST_PANDA_STM32 ${SOURCE_FILES})

target_include_directories(Klangstrom_KLST_PANDA_STM32 PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/src
)

target_compile_definitions(Klangstrom_KLST_PANDA_STM32 PRIVATE KLST_ENV=${KLST_ENV})
target_link_libraries(Klangstrom_KLST_PANDA_STM32 PRIVATE klangstrom)
