macro(convert_to_hex_and_flash)
    add_custom_target(convert-elf-to-hex 
        ALL
        DEPENDS ${PROJECT_NAME}.elf
        COMMAND ${CMAKE_OBJCOPY} -O ihex ${PROJECT_NAME}.elf ${PROJECT_NAME}.hex
        COMMENT "Generating .hex file from .elf file"
    )

    add_custom_target(flash
        DEPENDS convert-elf-to-hex 
        COMMAND ${OCD} -d2 -s ${OCD_FIND_SCRIPT_PATH} -f ${OCD_FLASHER_CFG} -c "transport select hla_swd" -f ${OCD_TARGET_CFG} -c "program {${CMAKE_BINARY_DIR}/${PROJECT_NAME}.hex}  verify reset; shutdown;"
        COMMENT "Flashing .hex to target MCU"
    )
endmacro()
