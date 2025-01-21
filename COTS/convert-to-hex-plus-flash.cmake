macro(convert_to_hex_and_flash PROJECT_NAME)
    # Convert ELF to binary for flashing
    add_custom_command(TARGET ${PROJECT_NAME}.elf POST_BUILD
    COMMAND arm-none-eabi-objcopy -O binary ${PROJECT_NAME}.elf ${PROJECT_NAME}.bin
    COMMENT "Converting ELF to binary..."
    )

    # Flash target using OpenOCD
    add_custom_target(flash
    COMMAND openocd -f interface/stlink.cfg -f target/stm32f4x.cfg \
    -c "program ${PROJECT_NAME}.bin verify reset exit"
    DEPENDS ${PROJECT_NAME}.bin
    COMMENT "Flashing the target with OpenOCD..."
    )  
endmacro()

