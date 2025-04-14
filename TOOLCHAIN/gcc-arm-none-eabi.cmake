# find_package("gcc-arm-none-eabi" REQUIRED)
# to make sure CMake knows that the target will not be executed on this machine
set(CMAKE_SYSTEM_NAME Generic)

# setting target architecture to ARM
set(CMAKE_SYSTEM_PROCESSOR arm)

# arm-none-eabi- must be part of path environment
set(TOOLCHAIN_PREFIX "/usr/bin/arm-none-eabi-")

# setting up C && Cpp INITIAL flags
set(CMAKE_C_FLAGS_INIT
    "-fdata-sections -ffunction-sections --specs=nano.specs -Wl,--gc-sections")
set(CMAKE_CXX_FLAGS_INIT
    "${CMAKE_C_FLAGS_INIT} -fno-rtti -fno-exceptions -fno-threadsafe-statics")

# defining toolchain binaries
set(CMAKE_C_COMPILER ${TOOLCHAIN_PREFIX}gcc)
set(CMAKE_ASM_COMPILER ${CMAKE_C_COMPILER})
set(CMAKE_CXX_COMPILER ${TOOLCHAIN_PREFIX}g++)
set(CMAKE_OBJCOPY ${TOOLCHAIN_PREFIX}objcopy)
set(CMAKE_SIZE ${TOOLCHAIN_PREFIX}size)
set(CMAKE_AR ${TOOLCHAIN_PREFIX}ar)
set(CMAKE_LINKER ${TOOLCHAIN_PREFIX}ld)

# to avoid issues where linking an executable is problematic due to missing system dependencies.
set(CMAKE_TRY_COMPILE_TARGET_TYPE STATIC_LIBRARY)