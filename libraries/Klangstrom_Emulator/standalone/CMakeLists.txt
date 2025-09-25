cmake_minimum_required(VERSION 3.15)

project(klangstrom-emulator-standalone LANGUAGES C CXX)

# -------------------------
# Configurable options
# -------------------------

option(ADD_KLANGWELLEN "Add Klangwellen library" ON)
set(KLANGWELLEN_PATH "$ENV{HOME}/Documents/dennisppaul/projects/klangwellen/git/klangwellen/") # DEV default path
set(ARDUINO_SKETCH_FILE "/Users/dennisppaul/Documents/dennisppaul/projects/klangwellen/git/klangwellen/examples/05.FormantFilter/05.FormantFilter.ino" CACHE PATH "Path to Arduino sketch file (.ino)")

if (ADD_KLANGWELLEN)
    message(STATUS "Klangwellen library will be included from: ${KLANGWELLEN_PATH}")
    if (NOT DEFINED ENV{UMFELD_PATH}) # only set if not already set in environment
        if (NOT DEFINED ENV{KLANGWELLEN_PATH} OR "$ENV{KLANGWELLEN_PATH}" STREQUAL "")
            message(FATAL_ERROR "KLANGWELLEN_PATH environment variable is not set but ADD_KLANGWELLEN is ON. Please set KLANGWELLEN_PATH to the root of your Klangwellen library.")
        endif ()
    endif ()
endif ()

if (NOT ARDUINO_SKETCH_FILE)
    set(ARDUINO_SKETCH_FILE "${CMAKE_CURRENT_SOURCE_DIR}/../../Klangstrom/examples/advanced/AudioCodecKlangwellen/AudioCodecKlangwellen.ino")
endif ()
set(HOME_DIR $ENV{HOME})
set(UMFELD_PATH $ENV{UMFELD_PATH})
if (NOT DEFINED ENV{UMFELD_PATH} OR "$ENV{UMFELD_PATH}" STREQUAL "")
    message(STATUS "UMFELD_PATH environment variable is not set.")
endif ()

#set(UMFELD_ARDUINO_PATH "${UMFELD_PATH}" CACHE PATH "Path to 'umfeld-arduino/umfeld' root")
set(UMFELD_ARDUINO_PATH "${HOME_DIR}/Documents/dev/umfeld/git/umfeld-arduino/umfeld" CACHE PATH "Path to 'umfeld-arduino/umfeld' root") # TODO this needs to handled differently
set(KLST_ENV "0x46" CACHE STRING "Klangstrom environment code")
set(KLST_EMU_AUDIO_BLOCK "2048" CACHE STRING "Audio block size for emulator")
set(VARIANT_H "variant_KLST_Emulator.h" CACHE STRING "Variant header filename")

if (NOT UMFELD_ARDUINO_PATH)
    message(FATAL_ERROR "UMFELD_ARDUINO_PATH is not set. Pass -DUMFELD_ARDUINO_PATH=/path/to/umfeld-arduino/umfeld")
endif ()

set(KLST_EMU_UMFELD_PATH "${UMFELD_ARDUINO_PATH}/cores/sdl/umfeld")
set(KLST_EMU_VARIANT_PATH "${UMFELD_ARDUINO_PATH}/variants/KLST_EMU")

message(STATUS "----------------------------------------")
message(STATUS "Klangstrom Emulator Standalone")
message(STATUS "----------------------------------------")
message(STATUS "UMFELD_PATH          : ${UMFELD_PATH}")
message(STATUS "UMFELD_ARDUINO_PATH  : ${UMFELD_ARDUINO_PATH}")
message(STATUS "KLST_EMU_UMFELD_PATH : ${KLST_EMU_UMFELD_PATH}")
message(STATUS "KLST_EMU_VARIANT_PATH: ${KLST_EMU_VARIANT_PATH}")
message(STATUS "KLST_ENV             : ${KLST_ENV}")
message(STATUS "KLST_EMU_AUDIO_BLOCK : ${KLST_EMU_AUDIO_BLOCK}")
message(STATUS "ARDUINO_SKETCH_FILE  : ${ARDUINO_SKETCH_FILE}")
message(STATUS "----------------------------------------")

# -------------------------
# Language / features
# -------------------------
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)
# Modern alternative to set(CMAKE_CXX_STANDARD ...):
# target_compile_features(target PRIVATE cxx_std_17)

# -------------------------
# Common interface options (DRY)
# -------------------------
add_library(klst_common_options INTERFACE)
target_compile_definitions(klst_common_options INTERFACE
        UMFELD_SET_DEFAULT_CALLBACK=0
        BOARD_NAME_KLST_EMU
        KLST_ENV=${KLST_ENV}
        KLST_EMU_AUDIO_BLOCK=${KLST_EMU_AUDIO_BLOCK}
        VARIANT_H="${VARIANT_H}"
)
target_include_directories(klst_common_options INTERFACE
        "$<BUILD_INTERFACE:${KLST_EMU_VARIANT_PATH}>"
        "$<BUILD_INTERFACE:${KLST_EMU_UMFELD_PATH}/..>"
        "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/src>"
)
# Mark third-party includes as SYSTEM if desired:
# target_include_directories(klst_common_options SYSTEM INTERFACE "${KLST_EMU_UMFELD_PATH}/..")

# -------------------------
# Executable and sources
# -------------------------
add_executable(${PROJECT_NAME})

# Keep GLOB but make it reactive; or replace with explicit list if you prefer stricter control.
#file(GLOB SRC_INO CONFIGURE_DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/src/*.ino")
# set_source_files_properties(${SRC_INO} PROPERTIES LANGUAGE CXX)
set_source_files_properties(${ARDUINO_SKETCH_FILE} PROPERTIES
        LANGUAGE CXX
        COMPILE_FLAGS "-x c++"
)

file(GLOB SRC CONFIGURE_DEPENDS
        "${CMAKE_CURRENT_SOURCE_DIR}/src/*.c"
        "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp"
        "${KLST_EMU_VARIANT_PATH}/*.c"
        "${KLST_EMU_VARIANT_PATH}/*.cpp"
)
target_sources(${PROJECT_NAME} PRIVATE ${SRC} ${ARDUINO_SKETCH_FILE})
target_link_libraries(${PROJECT_NAME} PRIVATE klst_common_options)

# -------------------------
# Dependencies
# -------------------------
# Umfeld
if (EXISTS "${KLST_EMU_UMFELD_PATH}/CMakeLists.txt")
    add_subdirectory("${KLST_EMU_UMFELD_PATH}" ${CMAKE_BINARY_DIR}/umfeld-lib-${PROJECT_NAME} EXCLUDE_FROM_ALL)
    add_umfeld_libs()
    # If you need the same options while compiling the lib itself:
    if (TARGET umfeld-lib)
        target_link_libraries(umfeld-lib PRIVATE klst_common_options)
    endif ()
    # NOTE linking library below is handled in `add_umfeld_libs()` ( as well as a few other things )
    #    if (TARGET umfeld-lib-interface)
    #        target_link_libraries(${PROJECT_NAME} PRIVATE umfeld-lib-interface umfeld-lib)
    #    endif ()
else ()
    message(FATAL_ERROR "Umfeld path not found: ${KLST_EMU_UMFELD_PATH}")
endif ()

# Klangstrom_Emulator (parent dir)
if (EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/../CMakeLists.txt")
    add_subdirectory(".." ${CMAKE_BINARY_DIR}/klangstrom-emulator-lib-${PROJECT_NAME} EXCLUDE_FROM_ALL)
    if (TARGET klangstrom-emulator)
        target_link_libraries(klangstrom-emulator PRIVATE klst_common_options umfeld-lib-interface umfeld-lib)
        target_link_libraries(${PROJECT_NAME} PRIVATE klangstrom-emulator)
    endif ()
else ()
    message(WARNING "Klangstrom_Emulator not found adjacent to this project.")
endif ()

# Klangstrom_KLST_EMU
if (EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/../../Klangstrom_KLST_EMU/CMakeLists.txt")
    add_subdirectory("../../Klangstrom_KLST_EMU" ${CMAKE_BINARY_DIR}/klangstrom_KLST_EMU-lib-${PROJECT_NAME} EXCLUDE_FROM_ALL)
    if (TARGET klangstrom_KLST_EMU)
        target_link_libraries(klangstrom_KLST_EMU PRIVATE klst_common_options umfeld-lib-interface umfeld-lib)
        target_link_libraries(${PROJECT_NAME} PRIVATE klangstrom_KLST_EMU)
    endif ()
else ()
    message(WARNING "Klangstrom_KLST_EMU not found at ../../Klangstrom_KLST_EMU")
endif ()

# Klangstrom
if (EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/../../Klangstrom/CMakeLists.txt")
    add_subdirectory("../../Klangstrom" ${CMAKE_BINARY_DIR}/klangstrom-lib-${PROJECT_NAME} EXCLUDE_FROM_ALL)
    if (TARGET klangstrom)
        target_link_libraries(klangstrom PRIVATE klangstrom_KLST_EMU)
        target_link_libraries(${PROJECT_NAME} PRIVATE klangstrom)
    endif ()
else ()
    message(WARNING "Klangstrom not found at ../../Klangstrom")
endif ()

# KlanWellen (optional)
if (ADD_KLANGWELLEN)
    message(STATUS "trying to include Klangwellen library")
    if (EXISTS "${KLANGWELLEN_PATH}/CMakeLists.txt")
        add_subdirectory("${KLANGWELLEN_PATH}" ${CMAKE_BINARY_DIR}/klangwellen-lib-${PROJECT_NAME} EXCLUDE_FROM_ALL)
        if (TARGET klangwellen)
            target_link_libraries(${PROJECT_NAME} PRIVATE klangwellen)
        endif ()
    else ()
        message(WARNING "Klangwellen not found at ../../Klangwellen")
    endif ()
endif ()
