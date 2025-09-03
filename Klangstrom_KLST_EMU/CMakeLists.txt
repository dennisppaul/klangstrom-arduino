cmake_minimum_required(VERSION 3.12)

project(klangstrom_KLST_EMU)

if(NOT DEFINED KLST_ENV)
    message(FATAL_ERROR "KLST_ENV is not defined")
endif()

file(GLOB SOURCE_FILES
        "${CMAKE_CURRENT_SOURCE_DIR}/src/*.c"
        "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp"
)

add_library(klangstrom_KLST_EMU ${SOURCE_FILES})

target_include_directories(klangstrom_KLST_EMU PUBLIC 
    ${CMAKE_CURRENT_SOURCE_DIR}/src
)

target_compile_definitions(klangstrom_KLST_EMU PRIVATE KLST_ENV=${KLST_ENV})
target_link_libraries(klangstrom_KLST_EMU PRIVATE klangstrom)
target_link_libraries(klangstrom_KLST_EMU PRIVATE klangstrom-emulator)
target_link_libraries(klangstrom_KLST_EMU PRIVATE umgebung-lib)
