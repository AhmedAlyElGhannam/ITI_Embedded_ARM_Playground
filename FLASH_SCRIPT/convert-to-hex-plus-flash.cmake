macro(convertToHex PROJECT_NAME)
    # adding a custom target/build rule for creating .hex file
    add_custom_target(convert-elf-to-hex 
    ALL
    DEPENDS ${PROJECT_NAME}.elf
	COMMAND ${CMAKE_OBJCOPY} -O ihex ${PROJECT_NAME}.elf ${PROJECT_NAME}.hex
    COMMENT "Generating .hex file from .elf file"
    )
endmacro()

macro(convertToBin PROJECT_NAME)
    # adding a custom target/build rule for creating .bin file
    add_custom_target(convert-elf-to-bin
    ALL
    DEPENDS ${PROJECT_NAME}.elf
	COMMAND ${CMAKE_OBJCOPY} -O binary ${PROJECT_NAME}.elf ${PROJECT_NAME}.bin
    COMMENT "Generating .bin file from .elf file"
    )
endmacro()

macro(flashHEX PROJECT_NAME)
    # flashing .hex on mcu using flashing tool
    add_custom_target(flash-hex
    COMMAND ${FLASHING_TOOL} ${FLASHING_COMMAND} ${PROJECT_NAME}.hex 0x8000000
    DEPENDS convert-elf-to-hex
    COMMENT "Flashing ${PROJECT_NAME} to ${MCU_MODEL}"
    )
endmacro()


macro(flashBin PROJECT_NAME)
    # flashing .bin on mcu using flashing tool
    add_custom_target(flash-bin
    COMMAND ${FLASHING_TOOL} ${FLASHING_COMMAND} ${PROJECT_NAME}.bin 0x8000000
    DEPENDS convert-elf-to-bin
    COMMENT "Flashing ${PROJECT_NAME} to ${MCU_MODEL}"
    )
endmacro()
