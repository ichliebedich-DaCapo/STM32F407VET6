set(CMAKE_SYSTEM_NAME Generic)
set(CMAKE_SYSTEM_VERSION 1)
cmake_minimum_required(VERSION 3.29)

# specify cross-compilers and tools
set(CMAKE_C_COMPILER arm-none-eabi-gcc)
set(CMAKE_CXX_COMPILER arm-none-eabi-g++)
set(CMAKE_ASM_COMPILER arm-none-eabi-gcc)
set(CMAKE_AR arm-none-eabi-ar)
set(CMAKE_OBJCOPY arm-none-eabi-objcopy)
set(CMAKE_OBJDUMP arm-none-eabi-objdump)
set(SIZE arm-none-eabi-size)
set(CMAKE_TRY_COMPILE_TARGET_TYPE STATIC_LIBRARY)

# project settings
project(Furina C CXX ASM)
set(CMAKE_CXX_STANDARD 20)
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

# 选择优化级别（真是见鬼！在CMake配置文件改了构建选项，结果没反应，干脆手动操作）
#-O0：无优化，适合调试。
#-O1：基本优化，适合快速迭代开发。
#-O2：中级优化，适合大多数生产环境。
#-O3：高级优化，适合对性能要求较高的应用。
#-Ofast：极端优化，适合对性能要求极高且对标准合规性要求不高的应用。
#-Og：调试优化，适合开发和调试阶段。
#-Os:优化生成的代码大小，而不是性能。
#-Oz:极度优化大小,进一步优化生成的代码大小，比 -Os更激进,可能会牺牲一些性能。
#-flto:在链接阶段进行优化,允许进行跨文件的优化，进一步提高性能，但耗时间（项目本来就小我不在乎）
#-fprofile-generate（生成配置文件） 和 -fprofile-use（基于配置文件的优化）
add_compile_options(-Ofast -g)
# 不调试了再开启
#add_compile_options(-flto )

#添加宏定义
# 使用Debug，使用HAL库。    -DUSE_FULL_LL_DRIVER 使用LL库
add_definitions(-DDEBUG -DUSE_HAL_DRIVER -DSTM32F407xx)

# ----------------------------------设置你的UI路径（选择项目）------------------------------
set(UI_DIR GUI/UI/signalGenerator)
# ----------------------------------设置你的UI路径（选择项目）------------------------------

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
        ${UI_DIR}
        GUI/Base
        GUI/Component
        GUI/Events

        # -----------------库-----------------
        Library/Data
        # --------第三方-------
        Library/Third_Party/
        # FreeRTOS库
        Library/Third_Party/FreeRTOS/Source/include
        Library/Third_Party/FreeRTOS/Source/CMSIS_RTOS_V2
        Library/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM4F
        # LVGL库
        Library/Third_Party/lvgl/src
        Library/Third_Party/lvgl/examples/porting
        # --------CMSIS-------
        # DSP
        Library/CMSIS/DSP/Include
        Library/CMSIS/DSP/PrivateInclude
)


# 添加资源文件
file(GLOB_RECURSE SOURCES
        "BSP/Core/*.s"
        "Drivers/STM32F4xx_HAL_Driver/Src/*.c"

        # 三层划分
        "Application/*.cpp"
        "Module/*.cpp"
        "BSP/*.c"

        # GUI
        "${UI_DIR}/*.*"
        "GUI/Base/GUI.cpp"
        "GUI/Component/*.cpp"
        "GUI/Events/*.cpp"

        # -----------------库-----------------
        # 数据库
        "Library/Data/*.*"
        # --------第三方-------
        # LVGL库
        "Library/Third_Party/lvgl/src/*.c"
        "Library/Third_Party/lvgl/examples/porting/*.c"
        # FreeRTOS库
#        "Library/Third_Party/FreeRTOS/Source/*.c"
#        "Library/Third_Party/FreeRTOS/Source/CMSIS_RTOS_V2/cmsis_os2.c"
#        " Library/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM4F/port.c"

        # --------CMSIS-------
        # DSP
        # 现在CMakeLists还没有用熟练，先凑合
        "Library/CMSIS/DSP/Source/TransformFunctions/arm_cfft_f32.c"
        "Library/CMSIS/DSP/Source/TransformFunctions/arm_cfft_init_f32.c"
        "Library/CMSIS/DSP/Source/ComplexMathFunctions/arm_cmplx_mag_f32.c"
)

# 设置链接脚本
set(LINKER_SCRIPT ${CMAKE_SOURCE_DIR}/STM32F407VETX_FLASH.ld)

add_link_options(-mfloat-abi=hard -mfpu=fpv4-sp-d16)
add_link_options(-mcpu=cortex-m4 -mthumb -mthumb-interwork)
add_link_options(-T ${LINKER_SCRIPT})

# 设置链接选项
#-Wl,--gc-sections：删除未使用的节区。链接器会删除未使用的代码和数据节区，减小二进制文件的大小。
#特别适用于嵌入式系统，可以显著减少代码大小。
#-Wl,--as-needed：按需链接库。链接器只会链接实际使用的库文件，避免不必要的依赖。
#可以减少二进制文件的大小和加载时间。
#-Wl,--relax：优化链接过程。链接器会尝试优化链接过程，减少重定位条目，提高加载性能。
#特别适用于某些架构，如 ARM。
#-Wl,--strip-all：删除所有符号表和调试信息。可以显著减小二进制文件的大小，但会影响调试体验。
#-Wl,--strip-debug：仅删除调试信息。
add_link_options(-Wl,-gc-sections,--print-memory-usage,-Map=${PROJECT_BINARY_DIR}/${PROJECT_NAME}.map)
#add_link_options(-Wl,--strip-all)# 好像没有作用

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