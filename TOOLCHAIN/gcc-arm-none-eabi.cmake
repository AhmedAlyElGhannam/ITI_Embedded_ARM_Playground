# find_package("gcc-arm-none-eabi" REQUIRED)
set(CMAKE_SYSTEM_NAME Generic)
set(CMAKE_SYSTEM_PROCESSOR arm)

# arm-none-eabi- must be part of path environment
set(TOOLCHAIN_PREFIX "/usr/bin/arm-none-eabi-")
# set(C_FLAGS "-fdata-sections -ffunction-sections --specs=nano.specs -Wl,--gc-sections")
# set(CPP_FLAGS "-fno-rtti -fno-exceptions -fno-threadsafe-statics")

# defining toolchain bin tools
set(CMAKE_C_COMPILER ${TOOLCHAIN_PREFIX}gcc ${C_FLAGS})
set(CMAKE_ASM_COMPILER ${CMAKE_C_COMPILER})
set(CMAKE_CXX_COMPILER ${TOOLCHAIN_PREFIX}g++ ${C_FLAGS} ${CPP_FLAGS})
set(CMAKE_OBJCOPY ${TOOLCHAIN_PREFIX}objcopy)
set(CMAKE_SIZE ${TOOLCHAIN_PREFIX}size)
set(CMAKE_AR ${TOOLCHAIN_PREFIX}ar)
set(CMAKE_LINKER ${TOOLCHAIN_PREFIX}ld)

set(CMAKE_TRY_COMPILE_TARGET_TYPE STATIC_LIBRARY)
