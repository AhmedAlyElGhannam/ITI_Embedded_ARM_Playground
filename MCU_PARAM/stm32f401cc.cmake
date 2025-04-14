# mcu-related info/specific compiler flags
set(MCU_FAMILY STM32F4xx)
set(MCU_MODEL STM32F401CC)
set(CPU_PARAMETERS
    -march=armv7e-m
    -mcpu=cortex-m4
    -mthumb
    -mfpu=fpv4-sp-d16
    -mfloat-abi=hard
    -mlittle-endian
    -DF_CPU=84000000UL
)
