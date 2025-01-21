# set minimum required version of cmake to avoid errors
cmake_minimum_required(VERSION 3.22)

# project name + defining used languages
project(MGPIO C CPP ASM) 

# setting default compiler for C
set(CMAKE_C_COMPILER "/usr/bin/arm-none-eabi-gcc")

# setting default compiler for Cpp
set(CMAKE_CXX_COMPILER "/usr/bin/arm-none-eabi-g++")

# setting default compiler for ASM
set(CMAKE_ASM_COMPILER "/usr/bin/arm-none-eabi-as")

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

# in order to allow main to use .h & pbcfg.h -> make them public
target_include_directories(${PROJECT_NAME} PUBLIC "${${PROJECT_NAME}_INC_DIRS}")

# registers should ABSOLUTELY BE private
target_include_directories(${PROJECT_NAME} PRIVATE "${${PROJECT_NAME}_REG_INC_DIR}")