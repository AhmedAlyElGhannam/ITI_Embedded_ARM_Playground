# set minimum required version of cmake to avoid errors
cmake_minimum_required(VERSION 3.22)

# include toolchain cmake script
include(${CMAKE_CURRENT_SOURCE_DIR}/../TOOLCHAIN/gcc-arm-none-eabi.cmake)

# include mcu model and family name + compiler parameters
include(${CMAKE_CURRENT_SOURCE_DIR}/../MCU_PARAM/stm32f401cc.cmake)

# include .cmake script that contains flashing tool info
include(${CMAKE_CURRENT_SOURCE_DIR}/../OCD/ocd.cmake)

# include ,cmake script for converting .elf to .hex/.bin and flashing
include(${CMAKE_CURRENT_SOURCE_DIR}/../FLASH_SCRIPT/convert-to-hex-plus-flash.cmake)

# project name 
project(ARM_PROJECT)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# set debug mode as default (will handle other values once it is release-ready)
if ("${CMAKE_BUILD_TYPE}" STREQUAL "")
    set(CMAKE_BUILD_TYPE Debug)
endif()

# define used languages
enable_language(C CXX ASM)

# c-related settings + used standard
set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS ON)

# cpp-related settings + used standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_STANDARD_INCLUDE_DIRECTORIES ${CMAKE_CXX_IMPLICIT_INCLUDE_DIRECTORIES})
set(CMAKE_CXX_EXTENSIONS ON)

# define a variable to store linker script name and path for this project
set(LINKER_SCRIPT ${CMAKE_CURRENT_SOURCE_DIR}/linker_script/STM32F401CCFX_FLASH.ld)

# define a variable to store startup file name and path for this project
set(STARTUP_FILE ${CMAKE_CURRENT_SOURCE_DIR}/startup/startup_stm32f401xc.S)

# collect all .c files for app
file(GLOB_RECURSE ${PROJECT_NAME}_SRC_FILES ${CMAKE_CURRENT_SOURCE_DIR}/src/*.c)

# define app include dir
set(${PROJECT_NAME}_INC_DIR "${CMAKE_CURRENT_SOURCE_DIR}/inc/")

# defining a list of MCAL libraries that will be used to build HAL libraries
set(MCAL_LIB
	MRCC
	MGPIO	
    # MNVIC
)


# defining a list of HAL libraries that app depends on
set(HAL_LIB
	HLED
)

# looping over all libs to build them
foreach(mcal_lib ${MCAL_LIB})
	add_subdirectory(
	    "${CMAKE_CURRENT_SOURCE_DIR}/../COTS/MCAL/${mcal_lib}/"
	    "${CMAKE_BINARY_DIR}/${mcal_lib}/"
	)
endforeach()


# # looping over all HAL libs to build them
# foreach(hal_lib ${HAL_LIB})
# 	add_subdirectory(
# 	    "${CMAKE_CURRENT_SOURCE_DIR}/../COTS/HAL/${hal_lib}"
# 	    "${CMAKE_BINARY_DIR}/${hal_lib}"
# 	)
# endforeach()

# compiling and getting .elf file
add_executable(${PROJECT_NAME}.elf "${${PROJECT_NAME}_SRC_FILES}" ${STARTUP_FILE}) # main target

# compiler options for target
target_compile_options(${PROJECT_NAME}.elf PRIVATE
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
target_link_options(${PROJECT_NAME}.elf  PRIVATE
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
    -specs=nano.specs
    -specs=nosys.specs
    --entry=_estack
    --data-sections
)

# libraries to-be-linked with target
target_link_libraries(${PROJECT_NAME}.elf 
    PRIVATE 
    ${MCAL_LIB} 
    # ${HAL_LIB}
)

# include directory for target
target_include_directories(${PROJECT_NAME}.elf 
    PRIVATE
    "${${PROJECT_NAME}_INC_DIR}"
)

# calling macro for converting .elf into .hex & flashing it to target
convert_to_hex_and_flash()



## openocd command
# openocd -d2 -s /home/nemesis/.platformio/packages/tool-openocd/openocd/scripts -f interface/stlink.cfg -c "transport select hla_swd" -f target/stm32f4x.cfg -c "program {.pio/build/genericSTM32F401CC/firmware.elf}  verify reset; shutdown;"
