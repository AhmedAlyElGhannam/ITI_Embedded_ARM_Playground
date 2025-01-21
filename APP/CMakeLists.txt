# set minimum required version of cmake to avoid errors
cmake_minimum_required(VERSION 3.22)

# to make sure CMake knows that the target will not be executed on this machine
set(CMAKE_SYSTEM_NAME Generic)

# setting target architecture to ARM
set(CMAKE_SYSTEM_PROCESSOR arm)

# set C && Cpp versions
set(CMAKE_C_STANDARD 99)

# project name + defining used languages 
project(ARM_PROJECT C CPP ASM)

# include ,cmake script for converting .elf to .hex and flashing
include(${CMAKE_CURRENT_SOURCE_DIR}/../COTS/convert-to-hex-plus-flash.cmake)

# arm mcu is cortex-m4 clocked @84MHz
set(MCU cortex-m4)
set(F_CPU 84000000) 
set(COMPILE_MCU_NAME atmega32) 	# for compiler
set(FLASH_MCU_NAME m32)    	# for avrdude
#set(FCPU 16000000UL)	# for frequency

# programmer type 
set(PROGRAMMER usbasp) 

# setting avrdude flash flags
set(FLASH_FLAGS "-R .eeprom -R .lock -R .signature")

# collect all .c files
file(GLOB_RECURSE ${PROJECT_NAME}_SRC_FILES ${CMAKE_CURRENT_SOURCE_DIR}/src/*.c)

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

# # defining a list of HAL libraries that app depends on
# set(HAL_LIB
# 	HLED
# 	HSWITCH
# 	H7SEGMENT
# )

# # looping over all HAL libs to build them
# foreach(hal_lib ${HAL_LIB})
# 	add_subdirectory(
# 	    "${CMAKE_CURRENT_SOURCE_DIR}/../COTS/HAL/${hal_lib}"
# 	    "${CMAKE_BINARY_DIR}/${hal_lib}"
# 	)
# endforeach()

# setting default compiler for C in this CMake file
set(CMAKE_C_COMPILER "/usr/bin/arm-none-eabi-gcc")

# setting defaulty compiler for Cpp in this CMake file
set(CMAKE_C_COMPILER "/usr/bin/arm-none-eabi-g++")

# setting defaulty assembler in this CMake file
set(CMAKE_C_COMPILER "/usr/bin/arm-none-eabi-as")

# setting default ar (lib archiving) tool in this CMake file
set(CMAKE_C_COMPILER "/usr/bin/arm-none-eabi-ar")

# setting default linker in this CMake file
set(CMAKE_C_COMPILER "/usr/bin/arm-none-eabi-ld")

# setting compiler flags for C && Cpp
add_compile_options(
    -Wall 
    -Os 
    -g 
    -mmcu=${COMPILE_MCU_NAME}
    )

    # -Wall -Wextra -g -ffunction-sections -fdata-sections
    # -mcpu=cortex-m4 -mthumb -mfpu=fpv4-sp-d16 -mfloat-abi=hard
    # set(LINKER_SCRIPT "${CMAKE_SOURCE_DIR}/stm32f401cc.ld")
    # startup file
    # set(CMAKE_EXE_LINKER_FLAGS "-T${LINKER_SCRIPT} -Wl,--gc-sections -Wl,-Map=${PROJECT_NAME}.map -Wl,--entry=_estack")
    # 

#     target_compile_options(${PROJECT_NAME} PRIVATE
#     -mcpu=${MCU}
#     -mthumb
#     -mfpu=fpv4-sp-d16
#     -mfloat-abi=hard
#     -Wall
#     -Wextra
#     ${DEBUG_FLAGS}
#     -ffunction-sections
#     -fdata-sections
# )

# target_link_options(${PROJECT_NAME} PRIVATE
#     -T${LINKER_SCRIPT}
#     -mcpu=${MCU}
#     -mthumb
#     -mfpu=fpv4-sp-d16
#     -mfloat-abi=hard
#     -specs=nano.specs
#     -specs=nosys.specs
#     -Wl,--gc-sections
#     -Wl,-Map=${PROJECT_NAME}.map
#     ${DEBUG_FLAGS}
# )

## TO DO
# add_compiler_definitions() # to define macros

# setting linker flags
add_link_options(-mmcu=${COMPILE_MCU_NAME} -Wl,--gc-sections)

# compiling and getting .elf file
add_executable(${PROJECT_NAME}.elf "${${PROJECT_NAME}_SRC_FILES}") # main target
target_link_libraries(${PROJECT_NAME}.elf PRIVATE ${MCAL_LIB} ${HAL_LIB})

# calling macro for converting .elf into .hex and flashing it on specified mcu
convert_to_hex_and_flash(${PROJECT_NAME} ${FLASH_MCU_NAME} ${PROGRAMMER})

# modify the clean build rule to delete the .hex file
set_directory_properties(PROPERTIES ADDITIONAL_MAKE_CLEAN_FILES ${PROJECT_NAME}.hex)





set(CMAKE_SYSTEM_NAME Generic)
set(CMAKE_SYSTEM_PROCESSOR arm)



















cmake_minimum_required(VERSION 3.11)
project(test C ASM)

set(CMAKE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/../../COTS/CMAKE")
set(BUILD_TOOLS_DIR "${CMAKE_CURRENT_SOURCE_DIR}/../../COTS/COMMON")
set(APP_DIR "${CMAKE_CURRENT_SOURCE_DIR}/src/main.c")
set(MCAL_DIR ${CMAKE_CURRENT_SOURCE_DIR}/../../COTS/MCAL)

include("${CMAKE_DIR}/cortexm4.cmake")

set(MCU cortex-m4)
set(F_CPU 84000000) 

set(STARTUP_FILE "${BUILD_TOOLS_DIR}/startup.c")
set(LINKER_SCRIPT "${BUILD_TOOLS_DIR}/stm32f401cc.ld")

add_executable(${PROJECT_NAME} ${APP_DIR})

add_subdirectory("${MCAL_DIR}/RCC" "${CMAKE_BINARY_DIR}/RCC")

target_link_libraries(${PROJECT_NAME} PUBLIC RCC)

target_compile_options(${PROJECT_NAME} PRIVATE
    -mcpu=${MCU}
    -mthumb
    -mfpu=fpv4-sp-d16
    -mfloat-abi=hard
    -Wall
    -Wextra
    ${DEBUG_FLAGS}
    -ffunction-sections
    -fdata-sections
)

target_link_options(${PROJECT_NAME} PRIVATE
    -T${LINKER_SCRIPT}
    -mcpu=${MCU}
    -mthumb
    -mfpu=fpv4-sp-d16
    -mfloat-abi=hard
    -specs=nano.specs
    -specs=nosys.specs
    -Wl,--gc-sections
    -Wl,-Map=${PROJECT_NAME}.map
    ${DEBUG_FLAGS}
)

add_executable(${PROJECT_NAME} ${APP_DIR})

add_subdirectory("${MCAL_DIR}/RCC" "${CMAKE_BINARY_DIR}/RCC")

add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND arm-none-eabi-objcopy -O binary ${PROJECT_NAME} ${PROJECT_NAME}.bin
    COMMAND arm-none-eabi-objcopy -O ihex ${PROJECT_NAME} ${PROJECT_NAME}.hex
    COMMENT "Generating binary and hex files"
)

# Optional: Add size information
add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND arm-none-eabi-size ${PROJECT_NAME}
    COMMENT "Displaying size information"
)