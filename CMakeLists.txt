#此文件从模板自动生成! 请勿更改!
set(CMAKE_SYSTEM_NAME Generic)
set(CMAKE_SYSTEM_VERSION 1)
cmake_minimum_required(VERSION 3.29)

# specify cross-compilers and tools
set(CMAKE_C_COMPILER arm-none-eabi-gcc)
set(CMAKE_CXX_COMPILER arm-none-eabi-g++)
set(CMAKE_ASM_COMPILER  arm-none-eabi-gcc)
set(CMAKE_AR arm-none-eabi-ar)
set(CMAKE_OBJCOPY arm-none-eabi-objcopy)
set(CMAKE_OBJDUMP arm-none-eabi-objdump)
set(SIZE arm-none-eabi-size)
set(CMAKE_TRY_COMPILE_TARGET_TYPE STATIC_LIBRARY)

# project settings
project(Furina C CXX ASM)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_C_STANDARD 11)

# uncomment to mitigate c++17 absolute addresses warnings
#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-register")

# 添加编译标志
#Uncomment for hardware floating point
add_compile_definitions(ARM_MATH_CM4;ARM_MATH_MATRIX_CHECK;ARM_MATH_ROUNDING)
add_compile_options(-mfloat-abi=hard -mfpu=fpv4-sp-d16)

#Uncomment for software floating point
#add_compile_options(-mfloat-abi=soft)

add_compile_options(-mcpu=cortex-m4 -mthumb -mthumb-interwork)
add_compile_options(-ffunction-sections -fdata-sections -fno-common -fmessage-length=0)

# Enable assembler files preprocessing
add_compile_options($<$<COMPILE_LANGUAGE:ASM>:-x$<SEMICOLON>assembler-with-cpp>)

# 选择优化级别
if ("${CMAKE_BUILD_TYPE}" STREQUAL "Release")
    message(STATUS "Maximum optimization for speed")
    add_compile_options(-Ofast)
elseif ("${CMAKE_BUILD_TYPE}" STREQUAL "RelWithDebInfo")
    message(STATUS "Maximum optimization for speed, debug info included")
    add_compile_options(-Ofast -g)
elseif ("${CMAKE_BUILD_TYPE}" STREQUAL "MinSizeRel")
    message(STATUS "Maximum optimization for size")
    add_compile_options(-Os)
else ()
    message(STATUS "Minimal optimization, debug info included")
    add_compile_options(-Og -g)
endif ()

#添加宏定义
# 使用Debug，使用HAL库。    -DUSE_FULL_LL_DRIVER 使用LL库
add_definitions(-DDEBUG -DUSE_HAL_DRIVER  -DSTM32F407xx )
#添加头文件
include_directories(
        # 官方自带库
        Drivers/STM32F4xx_HAL_Driver/Inc
        Drivers/STM32F4xx_HAL_Driver/Inc/Legacy
        Drivers/CMSIS/Device/ST/STM32F4xx/Include Drivers/CMSIS/Include

        # 应用
        Application
        Application/Base
        Application/Conf

        # 模块
        Module
        # 板级支持包
        BSP/Base
        BSP/Core
        BSP/LCD
        BSP/Key
        BSP/SPI_Flash
        BSP/Timer
        BSP/ADC
        BSP/DAC

        # GUI
        GUI/Base
        GUI/Events
        GUI/Component
        GUI/UI

        # 库
        Library/Data
        Library/Third_Party/
        # FreeRTOS库
        Library/Third_Party/FreeRTOS/Source/include
        Library/Third_Party/FreeRTOS/Source/CMSIS_RTOS_V2
        Library/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM4F
        # LVGL库
        Library/Third_Party/lvgl/src
        Library/Third_Party/lvgl/examples/porting
)


# 添加资源文件
file(GLOB_RECURSE SOURCES
        "Application/*.*"
        "Module/*.*"
        "BSP/*.*"
        "Drivers/*.*"
        "GUI/Base/*.*"
        "GUI/UI/*.*"
        "GUI/Component/*.*"
        "GUI/Events/*.*"

        # 数据库
        "Library/Data/*.*"
        "Library/Third_Party/lvgl/src/*.*"
        "Library/Third_Party/lvgl/lvgl.h"
        "Library/Third_Party/lvgl/lv_conf.h"
        "Library/Third_Party/lvgl/examples/porting/*.*"

        # FreeRTOS库
        "Library/Third_Party/FreeRTOS/*.*"
)

# 设置链接脚本
set(LINKER_SCRIPT ${CMAKE_SOURCE_DIR}/STM32F407VETX_FLASH.ld)

# 设置链接选项
add_link_options(-mfloat-abi=hard -mfpu=fpv4-sp-d16)
add_link_options(-Wl,-gc-sections,--print-memory-usage,-Map=${PROJECT_BINARY_DIR}/${PROJECT_NAME}.map)
add_link_options(-mcpu=cortex-m4 -mthumb -mthumb-interwork)
add_link_options(-T ${LINKER_SCRIPT})

add_executable(${PROJECT_NAME}.elf ${SOURCES} ${LINKER_SCRIPT})

set(HEX_FILE ${PROJECT_BINARY_DIR}/${PROJECT_NAME}.hex)
set(BIN_FILE ${PROJECT_BINARY_DIR}/${PROJECT_NAME}.bin)

# 添加自定义命令：
# 生成十六进制和二进制文件
add_custom_command(TARGET ${PROJECT_NAME}.elf POST_BUILD
        COMMAND ${CMAKE_OBJCOPY} -Oihex $<TARGET_FILE:${PROJECT_NAME}.elf> ${HEX_FILE}
        COMMAND ${CMAKE_OBJCOPY} -Obinary $<TARGET_FILE:${PROJECT_NAME}.elf> ${BIN_FILE}
        COMMENT "Building ${HEX_FILE}
Building ${BIN_FILE}")


# ----------------------------用于启动LVGL模拟器------------------------
# 引入模块,可以启用
#include(ExternalProject)
#ExternalProject_Add(GUI_simulator
#        SOURCE_DIR "${CMAKE_SOURCE_DIR}/GUI/lvgl-simulator"
#        BUILD_IN_SOURCE 1
#        CONFIGURE_COMMAND ""
#        BUILD_COMMAND ""
#        INSTALL_COMMAND ""
#)