# set minimum required version of cmake to avoid errors
cmake_minimum_required(VERSION 3.22)

# to make sure CMake knows that the target will not be executed on this machine
set(CMAKE_SYSTEM_NAME Generic)

# setting target architecture to ARM
set(CMAKE_SYSTEM_PROCESSOR arm)

# set C && Cpp versions
set(CMAKE_C_STANDARD 99)

# define a variable to store linker script name and path for this project
set(LINKER_SCRIPT ${CMAKE_CURRENT_SOURCE_DIR}/linker_script/linker.ld)

# define a variable to store startup file name and path for this project
set(STARTUP_FILE ${CMAKE_CURRENT_SOURCE_DIR}/startup/startup.c)

# project name 
project(ARM_PROJECT)

# define used languages
enable_language(C CXX ASM)

# arm mcu is cortex-m4 clocked @84MHz
set(MCU cortex-m4)
set(F_CPU 84000000UL) 

# programmer type 
set(PROGRAMMER st-link) 

# collect all .c files for app
file(GLOB_RECURSE ${PROJECT_NAME}_SRC_FILES ${CMAKE_CURRENT_SOURCE_DIR}/src/*.c)

# define app include dir
set(${PROJECT_NAME}_INC_DIR "${CMAKE_CURRENT_SOURCE_DIR}/inc/")

# defining a list of MCAL libraries that will be used to build HAL libraries
set(MCAL_LIB
	MRCC
	MGPIO	
)

# looping over all libs to build them
foreach(mcal_lib ${MCAL_LIB})
	add_subdirectory(
	    "${CMAKE_CURRENT_SOURCE_DIR}/../COTS/MCAL/${mcal_lib}/"
	    "${CMAKE_BINARY_DIR}/${mcal_lib}/"
	)
endforeach()

# defining a list of HAL libraries that app depends on
# set(HAL_LIB
# 	HLED
# )

# # looping over all HAL libs to build them
# foreach(hal_lib ${HAL_LIB})
# 	add_subdirectory(
# 	    "${CMAKE_CURRENT_SOURCE_DIR}/../COTS/HAL/${hal_lib}"
# 	    "${CMAKE_BINARY_DIR}/${hal_lib}"
# 	)
# endforeach()

# include .cmake script that contains used toolchain info
include(${CMAKE_CURRENT_SOURCE_DIR}/../TOOLCHAIN/gcc-arm-none-eabi.cmake)

# include .cmake script that contains flashing tool info
include(${CMAKE_CURRENT_SOURCE_DIR}/../FLASH_TOOL/st-flash.cmake)

# include ,cmake script for converting .elf to .hex/.bin and flashing
include(${CMAKE_CURRENT_SOURCE_DIR}/../FLASH_SCRIPT/convert-to-hex-plus-flash.cmake)

# compiling and getting .elf file
add_executable(${PROJECT_NAME}.elf "${${PROJECT_NAME}_SRC_FILES}" ${STARTUP_FILE}) # main target

# compiler options for target
target_compile_options( ${PROJECT_NAME}.elf
    PRIVATE
    -Wall 
    -Wextra 
    -g 
    -Os 
    -ffunction-sections 
    -fdata-sections
    -mcpu=${MCU} 
    -mthumb 
    -mfpu=fpv4-sp-d16 
    -mfloat-abi=hard
    -pedantic
)

# linker options for target
target_link_options( ${PROJECT_NAME}.elf
    PRIVATE
    -T${LINKER_SCRIPT}
    -mcpu=${MCU}
    -mthumb
    -mfpu=fpv4-sp-d16
    -mfloat-abi=hard
    -specs=nano.specs
    -specs=nosys.specs
    --entry=_estack
    --data-sections
)

# libraries to-be-linked with target
target_link_libraries(${PROJECT_NAME}.elf 
    PRIVATE 
    ${MCAL_LIB} 
    ${HAL_LIB}
)

# include directory for target
target_include_directories(${PROJECT_NAME}.elf 
    PRIVATE
    "${${PROJECT_NAME}_INC_DIR}"
)

# calling macro for converting .elf into .hex
# convertTo_hex_or_bin(${PROJECT_NAME} ".hex")

# calling macro for flashing .hex on mcu
convertTo_hex_or_bin_and_flash(${PROJECT_NAME} ${FLASHING_TOOL} "hex")

# modify the clean build rule to delete generated .hex && .bin files
# set_directory_properties(PROPERTIES ADDITIONAL_MAKE_CLEAN_FILES ${PROJECT_NAME}.hex ${PROJECT_NAME}.bin)