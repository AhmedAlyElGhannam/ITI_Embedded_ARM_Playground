macro(convert_to_hex_and_flash)
    add_custom_target(convert-elf-to-hex 
        ALL
        DEPENDS ${PROJECT_NAME}.elf
        COMMAND ${CMAKE_OBJCOPY} -O ihex ${PROJECT_NAME}.elf ${PROJECT_NAME}.hex
        COMMENT "Generating .hex file from .elf file"
    )

    add_custom_target(flash
        DEPENDS convert-elf-to-hex 
        COMMAND "/usr/bin/openocd" -d2 -f /usr/share/openocd/scripts/interface/stlink.cfg -c "transport select hla_swd" -f /usr/share/openocd/scripts/target/stm32f4x.cfg -c "program ${CMAKE_BINARY_DIR}/${PROJECT_NAME}.elf verify reset; shutdown;"
        COMMENT "Flashing .hex to target MCU"
    )
endmacro()
