# set minimum required version of cmake to avoid errors
cmake_minimum_required(VERSION 3.22)

# include toolchain cmake script
include(${CMAKE_CURRENT_SOURCE_DIR}/../../../TOOLCHAIN/gcc-arm-none-eabi.cmake)

# project name + defining used languages
project(MRCC)

# define used languages
enable_language(C CXX ASM)

# collect library source files
file(GLOB_RECURSE ${PROJECT_NAME}_SRC_FILES ${CMAKE_CURRENT_SOURCE_DIR}/src/*.c)

# collect library header files
set(${PROJECT_NAME}_LIB_INC_DIR "${CMAKE_CURRENT_SOURCE_DIR}/inc/")
set(${PROJECT_NAME}_REG_INC_DIR "${CMAKE_CURRENT_SOURCE_DIR}/inc/reg/")
set(${PROJECT_NAME}_STDLIB_INC_DIR "${CMAKE_CURRENT_SOURCE_DIR}/../../STD_LIB/")
set(${PROJECT_NAME}_SERVICES_INC_DIR "${CMAKE_CURRENT_SOURCE_DIR}/../../SERVICES/")

# combine all header file dir variables into a single variable
set(${PROJECT_NAME}_INC_DIRS "${${PROJECT_NAME}_LIB_INC_DIR}" "${${PROJECT_NAME}_STDLIB_INC_DIR}" "${${PROJECT_NAME}_SERVICES_INC_DIR}")

# create a static library
add_library(${PROJECT_NAME} STATIC "${${PROJECT_NAME}_SRC_FILES}")

# compiler options for target
target_compile_options(${PROJECT_NAME} PRIVATE
    ${CPU_PARAMETERS}
    -Wall
    -Wextra
    -Wpedantic
    -Wshadow
    -Wdouble-promotion
    -Wformat=2 -Wformat-truncation
    -Wundef
    -fno-common
    -Wno-unused-parameter
    $<$<COMPILE_LANGUAGE:CXX>: # apply the following flags only if c++ code is compiled
        -Wconversion
        -Wno-volatile
        -Wold-style-cast
        -Wuseless-cast
        -Wsuggest-override>
    $<$<CONFIG:Debug>:-Og -g3 -ggdb> # apply the following flags only if build type is Debug
    $<$<CONFIG:Release>:-Og -g0>
)

# linker options for target
target_link_options(${PROJECT_NAME} PRIVATE
    -T${LINKER_SCRIPT}
    ${CPU_PARAMETERS}
    -Wl,-Map=${CMAKE_PROJECT_NAME}.map
    $<$<VERSION_GREATER:$<C_COMPILER_VERSION>,10.3.1>:-Wl,--no-warn-rwx-segments>
    -Wl,--start-group
    -lc
    -lm
    -lgcc
    -lstdc++
    -Wl,--end-group
    -Wl,--print-memory-usage
    # -specs=nano.specs
    -specs=nosys.specs
    --entry=_estack
    --data-sections
)

# in order to allow main to use .h & pbcfg.h -> make them public
target_include_directories(${PROJECT_NAME} PUBLIC "${${PROJECT_NAME}_INC_DIRS}")

# registers should ABSOLUTELY BE private
target_include_directories(${PROJECT_NAME} PRIVATE "${${PROJECT_NAME}_REG_INC_DIR}")