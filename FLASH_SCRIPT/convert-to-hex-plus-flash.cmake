macro(convertTo_hex_or_bin_and_flash PROJECT_NAME FLASHING_TOOL FILE_TYPE)
    message(STATUS "FLASHING_TOOL: ${FLASHING_TOOL}")
    message(STATUS "FILE_TYPE: ${FILE_TYPE}")

    if(FLASHING_TOOL STREQUAL "/usr/bin/openocd")
        message(STATUS "Using OpenOCD for flashing")

        if(FILE_TYPE STREQUAL "hex")
            message(STATUS "Creating convert-elf-to-hex target")
            add_custom_target(convert-elf-to-hex 
                ALL
                DEPENDS ${PROJECT_NAME}.elf
                COMMAND ${CMAKE_OBJCOPY} -O ihex ${PROJECT_NAME}.elf ${PROJECT_NAME}.hex
                COMMENT "Generating .hex file from .elf file"
            )
        elseif(FILE_TYPE STREQUAL "bin")
            message(STATUS "Creating convert-elf-to-bin target")
            add_custom_target(convert-elf-to-bin
                DEPENDS ${PROJECT_NAME}.elf
                COMMAND ${CMAKE_OBJCOPY} -O binary ${PROJECT_NAME}.elf ${PROJECT_NAME}.bin
                COMMENT "Generating .bin file from .elf file"
            )
        endif()

        message(STATUS "Creating flash target")
        add_custom_target(flash
            COMMAND openocd -f ${CMAKE_CURRENT_SOURCE_DIR}/../FLASH_TOOL/openocd.cfg -c "program ${PROJECT_NAME}${FILE_TYPE} verify reset exit"
            DEPENDS convert-elf-to-hex
            COMMENT "Flashing ${PROJECT_NAME} to ${MCU}"
        )
    else()
        message(WARNING "Unsupported flashing tool: ${FLASHING_TOOL}")
    endif()
endmacro()