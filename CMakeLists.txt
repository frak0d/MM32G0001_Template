cmake_minimum_required(VERSION 3.20)

set(CMAKE_SYSTEM_NAME Generic)
set(CMAKE_SYSTEM_PROCESSOR arm)
set(CMAKE_TRY_COMPILE_TARGET_TYPE STATIC_LIBRARY)

set(TOOLCHAIN_PREFIX arm-none-eabi)
set(CMAKE_C_COMPILER ${TOOLCHAIN_PREFIX}-gcc)
set(CMAKE_CXX_COMPILER ${TOOLCHAIN_PREFIX}-g++)
set(CMAKE_ASM_COMPILER ${TOOLCHAIN_PREFIX}-gcc)
set(CMAKE_OBJCOPY ${TOOLCHAIN_PREFIX}-objcopy)

project(example-project C CXX ASM)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(COMMON_FLAGS "-mcpu=cortex-m0plus -Os -flto -Wall -Wextra -fdata-sections -ffunction-sections -fno-builtin -DUSE_STDPERIPH_DRIVER")
set(CMAKE_C_FLAGS "${COMMON_FLAGS} -std=c23")
set(CMAKE_CXX_FLAGS "${COMMON_FLAGS} -std=c++26 -fno-exceptions -fno-rtti -Wno-register")

set(LINKER_SCRIPT ${CMAKE_CURRENT_SOURCE_DIR}/ldscript.ld)
set(CMAKE_EXE_LINKER_FLAGS "-T${LINKER_SCRIPT} --specs=nano.specs --specs=nosys.specs -Wl,--gc-sections,--build-id=none,--no-warn-rwx-segments,--print-memory-usage")

include_directories(
    "Device/CMSIS/Core/Include"
    "Device/MM32G0001/Include"
    "Device/MM32G0001/HAL_Lib/inc"
    "include"
)
set(DEVICE_SOURCES
    "Device/MM32G0001/Source/system_mm32g0001.c"
    "Device/MM32G0001/Source/startup_mm32g0001_gcc.s"
)
file(GLOB HAL_SOURCES "Device/MM32G0001/HAL_Lib/src/*.c")
file(GLOB_RECURSE USER_SOURCES CONFIGURE_DEPENDS "source/*.c" "source/*.cpp")

add_executable(${PROJECT_NAME}.elf ${DEVICE_SOURCES} ${HAL_SOURCES} ${USER_SOURCES})

add_custom_command(
    TARGET ${PROJECT_NAME}.elf POST_BUILD
    COMMAND ${CMAKE_OBJCOPY} -O binary $<TARGET_FILE:${PROJECT_NAME}.elf> ${CMAKE_BINARY_DIR}/${PROJECT_NAME}.bin
    COMMENT "Building ${PROJECT_NAME}.bin"
)

add_custom_target(flash
    COMMAND pyocd load ${CMAKE_BINARY_DIR}/${PROJECT_NAME}.bin -t mm32g0001a1tc --pack ${CMAKE_CURRENT_SOURCE_DIR}/MM32G0001_DFP_1.0.1.pack
    DEPENDS ${PROJECT_NAME}.elf
    COMMENT "Uploading to MCU..."
)
