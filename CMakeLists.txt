# --------------------------------------选择你的项目--------------------------------
# 如果你是在CLion里，那么可以直接在斜杆后面输入首字母，会自动弹出相关项目选项
set(APP_DIR Application/StorageOscilloscope)

# --------------------------------------某些可选选项------------------------------
# 是否使用独立静态库链接
# 注释后，可以使用链接优化-flto，并且默认开启链接优化
# ld链接器无法自动创建目录以存放map文件，所以注释下面时需要确保存在bin目录
set(STATIC_MODULE_LD True)
# 使用GUI
set(GUI_ENABLE True)
# 使用AI
#set (AI_ENABLE True)
# 添加编译器选项，使得支持Module特性
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fmodules-ts")

# --------------------------------------某些配置------------------------------
set(CMAKE_SYSTEM_NAME Generic)
set(CMAKE_SYSTEM_VERSION 1)
cmake_minimum_required(VERSION 3.29)
#设置编译目标类型为静态，必须放在project语句前面
set(CMAKE_TRY_COMPILE_TARGET_TYPE STATIC_LIBRARY)

# ----------------------------------设置编译工具-------------------------------
# -------gcc-------
set(CMAKE_C_COMPILER arm-none-eabi-gcc)
set(CMAKE_CXX_COMPILER arm-none-eabi-g++)
set(CMAKE_ASM_COMPILER arm-none-eabi-gcc)
set(CMAKE_AR arm-none-eabi-ar)
set(CMAKE_OBJCOPY arm-none-eabi-objcopy)
set(CMAKE_OBJDUMP arm-none-eabi-objdump)
set(SIZE arm-none-eabi-size)
# -------Clang------
# 我的评价是：缺少各种库。查看了STM32提供的固件包，压根没有clang的启动文件，这一点与TI相比弗如远甚。
# 继续搞下去，只会是浪费时间，并且从目前编译的情况来看，编译速度很慢很慢
#set(CMAKE_C_COMPILER clang)
#set(CMAKE_CXX_COMPILER clang++)
#set(CMAKE_ASM_COMPILER clang)
#set(CMAKE_AR llvm-ar)
#set(CMAKE_OBJCOPY llvm-objcopy)
#set(CMAKE_OBJDUMP llvm-objdump)
#set(SIZE llvm-size)

# -----------------工程设置-----------------

project(Furina C CXX ASM)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_C_STANDARD 11)

# ----------------------判断编译器类别--------------------
if (${CMAKE_C_COMPILER_ID} STREQUAL "GNU")
    message(STATUS "Using GCC compiler")
    set(COMPILER_IS_GCC TRUE)
elseif (${CMAKE_C_COMPILER_ID} STREQUAL "Clang")
    message(STATUS "Using Clang compiler")
    set(COMPILER_IS_CLANG TRUE)
else ()
    message(WARNING "Unsupported compiler: ${CMAKE_C_COMPILER_ID}")
endif ()


# ------------------------添加宏定义--------------------------
add_compile_definitions(ARM_MATH_CM4;ARM_MATH_MATRIX_CHECK;ARM_MATH_ROUNDING)
# 使用Debug，使用HAL库。    -DUSE_FULL_LL_DRIVER 使用LL库
add_definitions(-DDEBUG -DUSE_HAL_DRIVER -DSTM32F407xx)
if(GUI_ENABLE)
    add_definitions(-DGUI_ENABLE)
endif ()

#--------------------------编译选项----------------------------
# 根据编译器类型设置特定编译选项
if (COMPILER_IS_GCC)
    add_compile_options(-mcpu=cortex-m4 -mthumb -mthumb-interwork)
    # 硬件浮点和软件浮点，任选一个
    add_compile_options(-mfloat-abi=hard -mfpu=fpv4-sp-d16)
    #add_compile_options(-mfloat-abi=soft)

elseif (COMPILER_IS_CLANG)
    add_compile_options(-target armv7em-none-eabi)
    #    add_compile_options(-mcpu=cortex-m4 -mthumb )
    # 硬件浮点和软件浮点，任选一个
    add_compile_options(-mfloat-abi=hard -mfpu=fpv4-sp-d16)
    link_directories("E:/Tools/Develop/ToolsKits/ARM/arm-gnu-toolchain_v13_3/lib/gcc/arm-none-eabi/13.3.1")
    link_directories("E:/Tools/Develop/ToolsKits/ARM/arm-gnu-toolchain_v13_3/lib/gcc/arm-none-eabi/13.3.1")
endif ()


#---------------------------优化选项--------------------------
add_compile_options(-ffunction-sections -fdata-sections -fno-common -fmessage-length=0)
# 允许预处理器处理汇编文件
add_compile_options($<$<COMPILE_LANGUAGE:ASM>:-x$<SEMICOLON>assembler-with-cpp>)

# 如果不开优化，使用LVGL时有可能因为栈爆而导致数组越界，进而进入硬件错误中断。至少需要开-Og
if (COMPILER_IS_GCC)
    # 选择优化级别（真是见鬼！在CMake配置文件改了构建选项，结果没反应，干脆手动操作）
    #-O0：无优化，适合调试。
    #-O1：基本优化，适合快速迭代开发。
    #-O2：中级优化，适合大多数生产环境。
    #-O3：高级优化，适合对性能要求较高的应用。
    #-Ofast：极端优化，适合对性能要求极高且对标准合规性要求不高的应用。
    #-Og：调试优化，适合开发和调试阶段。
    #-Os:优化生成的代码大小，而不是性能。
    #-Oz:极度优化大小,进一步优化生成的代码大小，比 -Os更激进,可能会牺牲一些性能。
    #-flto:在链接阶段进行优化,允许进行跨文件的优化，进一步提高性能，但耗时间（项目本来就小我不在乎），并且这个优化容易出问题（这个我很在乎）
    #-fprofile-generate（生成配置文件） 和 -fprofile-use（基于配置文件的优化）
    add_compile_options(-Ofast -g)
    # 必须关闭静态库独立模块编译，才能够使用链接优化
    if (NOT STATIC_MODULE_LD)
#仅使用 add_compile_options(-flto)：
#   结果：编译器生成的中间表示（IR）包含优化所需的信息，但链接器不会进行优化。
#   问题：LTO 的效果不完整，性能提升有限。
#仅使用 add_link_options(-flto)：
#   结果：链接器尝试进行优化，但编译器生成的对象文件可能不包含足够的信息。
#   问题：链接器无法进行有效的优化，可能导致编译错误或优化失败。
#        add_compile_options(-flto )
#        add_link_options(-flto)
    endif ()
elseif (COMPILER_IS_CLANG)
    add_compile_options(-O3 -ffast-math -g3 -Wextra)
    # 禁用依赖扫描
    #    set(CMAKE_CXX_DEPFILE_FORMAT none)
    #    set(CMAKE_CXX_DEPENDS_GENERATOR "")
endif ()

# ----------------对选择的项目进行字符串处理，以匹配对应的UI文件--------------

# 提取 APP_DIR 的末尾部分
string(REGEX REPLACE "^.*/(.+)$" "\\1" APP_DIR_LAST_PART "${APP_DIR}")
# 拼接新的 UI_DIR
set(UI_DIR "GUI/UI/${APP_DIR_LAST_PART}")

# ----设置编译目录----
set(BUILD_DIR ${CMAKE_CURRENT_BINARY_DIR}/build)
set(LIB_DIR ${BUILD_DIR}/lib)
set(BIN_DIR ${BUILD_DIR}/bin)

#-------------------------------------------库-------------------------------------------
# 官方驱动库
file(GLOB_RECURSE DRIVERS_SRC "Drivers/STM32F4xx_HAL_Driver/Src/*.c")
set(DRIVERS_DIR
        Drivers
        Drivers/STM32F4xx_HAL_Driver/Inc
        Drivers/STM32F4xx_HAL_Driver/Inc/Legacy
        Drivers/CMSIS/Device/ST/STM32F4xx/Include
        Drivers/CMSIS/Include
)
add_library(libdrivers STATIC ${DRIVERS_SRC})
target_include_directories(libdrivers PUBLIC ${DRIVERS_DIR})
# 设置静态库的输出目录
set_target_properties(libdrivers PROPERTIES ARCHIVE_OUTPUT_DIRECTORY ${LIB_DIR})


#---------------------第三方库-----------------------
# -----lvgl库-----
file(GLOB_RECURSE LVGL_SRC
        # LVGL库
        "Library/Third_Party/LVGL/lvgl/src/*.c"
        "Library/Third_Party/LVGL/lvgl/examples/porting/*.c"
)
set(LVGL_DIR
        Library/Third_Party/LVGL
        Library/Third_Party/LVGL/lvgl/src
        Library/Third_Party/LVGL/lvgl/examples/porting
)
add_library(liblvgl STATIC ${LVGL_SRC})
target_include_directories(liblvgl PRIVATE BSP/inc)
target_include_directories(liblvgl PUBLIC ${LVGL_DIR})
# 依赖BSP中的lcd库 耦合有些严重，后面慢慢改
target_link_libraries(liblvgl PUBLIC libdrivers)
# 设置静态库的输出目录
set_target_properties(liblvgl PROPERTIES ARCHIVE_OUTPUT_DIRECTORY ${LIB_DIR})


# ------FreeRTOS库------
# 不愧是CubeMX生成的库，和HAL耦合得太严重了
file(GLOB_RECURSE FREERTOS_SRC
        "Library/Third_Party/FreeRTOS/Source/*.c"
        "Library/Third_Party/FreeRTOS/Source/CMSIS_RTOS_V2/cmsis_os2.c"
        "Library/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM4F/port.c"
)
set(FREERTOS_DIR
        Library/Third_Party/FreeRTOS
        Library/Third_Party/FreeRTOS/Source/include
        Library/Third_Party/FreeRTOS/Source/CMSIS_RTOS_V2
        Library/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM4F
)
add_library(libfreertos STATIC ${FREERTOS_SRC})
target_include_directories(libfreertos PRIVATE
        Drivers
        Drivers/STM32F4xx_HAL_Driver/Inc
        Drivers/CMSIS/Include
        Drivers/CMSIS/Device/ST/STM32F4xx/Include)
target_include_directories(libfreertos PUBLIC ${FREERTOS_DIR})
# 设置静态库的输出目录
set_target_properties(libfreertos PROPERTIES ARCHIVE_OUTPUT_DIRECTORY ${LIB_DIR})


# ------DSP库------
file(GLOB_RECURSE DSP_SRC
        "Library/CMSIS/DSP/Source/TransformFunctions/arm_cfft_f32.c"
        "Library/CMSIS/DSP/Source/TransformFunctions/arm_cfft_init_f32.c"
        "Library/CMSIS/DSP/Source/ComplexMathFunctions/arm_cmplx_mag_f32.c"
)
set(DSP_DIR
        Library/CMSIS/DSP/Include
        Library/CMSIS/DSP/PrivateInclude
)
add_library(libdsp STATIC ${DSP_SRC})
target_include_directories(libdsp PRIVATE Drivers/CMSIS/Include)
target_include_directories(libdsp PUBLIC ${DSP_DIR})
# 设置静态库的输出目录
set_target_properties(libdsp PROPERTIES ARCHIVE_OUTPUT_DIRECTORY ${LIB_DIR})


#---------------------用户库-----------------------
# ------BSP库------
file(GLOB_RECURSE BSP_SRC "BSP/src/*.c")
set(BSP_DIR BSP/inc)
add_library(libbsp STATIC ${BSP_SRC})
target_include_directories(libbsp PUBLIC ${BSP_DIR})
target_link_libraries(libbsp PUBLIC libdrivers)
# 设置静态库的输出目录
set_target_properties(libbsp PROPERTIES ARCHIVE_OUTPUT_DIRECTORY ${LIB_DIR})


# ------Data库------
file(GLOB_RECURSE DATA_SRC "Library/Data/*.c")
set(DATA_DIR
        Library/Data
)
add_library(libdata STATIC ${DATA_SRC})
target_include_directories(libdata INTERFACE ${DATA_DIR})
# 设置静态库的输出目录
set_target_properties(libdata PROPERTIES ARCHIVE_OUTPUT_DIRECTORY ${LIB_DIR})


# ------GUI库------
if (GUI_ENABLE)
file(GLOB_RECURSE GUI_SRC
        "${UI_DIR}/*.*"
        "GUI/Base/*.cpp"
        "GUI/Component/*.cpp"
        "GUI/Events/*.cpp"
        "GUI/Render/*.cpp"
)
set(GUI_DIR
        ${UI_DIR}
        GUI/Base
        GUI/Component
        GUI/Events
        GUI/Render
)
add_library(libgui STATIC ${GUI_SRC})
target_include_directories(libgui PUBLIC ${GUI_DIR})
# 依赖lvgl库
target_link_libraries(libgui PUBLIC liblvgl libbsp)
# 设置静态库的输出目录
set_target_properties(libgui PROPERTIES ARCHIVE_OUTPUT_DIRECTORY ${LIB_DIR})
endif ()

# ------Module库------
file(GLOB_RECURSE MODULE_SRC "Module/src/*.cpp")
set(MODULE_DIR
        Module/inc
)
add_library(libmodule STATIC ${MODULE_SRC})
target_include_directories(libmodule PUBLIC ${MODULE_DIR})
target_link_libraries(libmodule PRIVATE libdsp libdata)
# 设置静态库的输出目录
set_target_properties(libmodule PROPERTIES ARCHIVE_OUTPUT_DIRECTORY ${LIB_DIR})

# -------AI库-------
if (AI_ENABLE)
    file(GLOB_RECURSE AI_SRC "AI/${APP_DIR_LAST_PART}/App/*.c")
    set(AI_DIR AI/${APP_DIR_LAST_PART}/App AI/${APP_DIR_LAST_PART})
    add_library(libai STATIC ${AI_SRC})
    target_include_directories(libai PUBLIC ${AI_DIR})
    target_link_libraries(libai INTERFACE :NetworkRuntime910_CM4_GCC.a)
    target_link_libraries(libai PRIVATE libdrivers)
    target_link_directories(libai INTERFACE Library/ST/AI/Lib)

    # 设置静态库的输出目录
    set_target_properties(libai PROPERTIES ARCHIVE_OUTPUT_DIRECTORY ${LIB_DIR})
endif ()

# ------Application库------
file(GLOB_RECURSE APPLICATION_SRC "${APP_DIR}/app.cpp" "Application/Base/main.cpp" "Application/Base/baseInit.cpp")
set(APPLICATION_DIR Application/Base)
add_library(libapp STATIC ${APPLICATION_SRC})
target_include_directories(libapp PUBLIC ${APPLICATION_DIR})
# 需要链接 libdsp libdata libgui libbsp libmodule等，根据需要选
target_link_libraries(libapp PUBLIC libdsp libdata libbsp libmodule )
if(AI_ENABLE)
    target_link_libraries(libapp PUBLIC libai)
endif ()
if(GUI_ENABLE)
    target_link_libraries(libapp PUBLIC libgui)
endif ()
# 设置静态库的输出目录
set_target_properties(libapp PROPERTIES ARCHIVE_OUTPUT_DIRECTORY ${LIB_DIR})

# -------------------module特性测试-----------------------
file(GLOB_RECURSE cxx_modules
    "Module/inc/*.ixx" "Module/src/*.cppm"
)

# ----------------包含公共目录---------------
# 公共目录
include_directories(Library/ST/AI/Inc)
include_directories(Application/Conf ${APP_DIR})

# 定义公共的头文件目录和源文件
set(COMMON_INCLUDE_DIRS
    ${DRIVERS_DIR}
    ${BSP_DIR}
    ${DATA_DIR}
    ${MODULE_DIR}
    ${AI_DIR}
    ${APPLICATION_DIR}
)

set(COMMON_SOURCES
    "Core/*.*"
    "Application/Base/ISR.cpp"
    ${DRIVERS_SRC}
    ${BSP_SRC}
    ${DATA_SRC}
    ${MODULE_SRC}
    ${AI_SRC}
    ${APPLICATION_SRC}
)

if (STATIC_MODULE_LD)
    # 这里面不能直接添加到静态库里 否则链接时会出现找不到定义的现象
    file(GLOB_RECURSE SOURCES "Core/*.*" "Application/Base/ISR.cpp")
else ()
    if (GUI_ENABLE)
        list(APPEND COMMON_INCLUDE_DIRS
            ${LVGL_DIR}
            ${GUI_DIR}
        )
        list(APPEND COMMON_SOURCES
            ${LVGL_SRC}
            ${GUI_SRC}
        )
    endif ()

    # 添加公共的头文件目录、源文件
    include_directories(${COMMON_INCLUDE_DIRS})
    file(GLOB_RECURSE SOURCES ${COMMON_SOURCES})
endif ()


# -------------------编译链接-------------------
# 设置链接脚本
set(LINKER_SCRIPT ${CMAKE_SOURCE_DIR}/STM32F407VETX_FLASH.ld )

# 链接标志
add_link_options(-mfloat-abi=hard -mfpu=fpv4-sp-d16)
if (COMPILER_IS_GCC)
    add_link_options(-mcpu=cortex-m4 -mthumb -mthumb-interwork)
elseif (COMPILER_IS_CLANG)
    add_link_options(-mcpu=cortex-m4)
endif ()
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
add_link_options(-Wl,-gc-sections,--print-memory-usage,-Map=${BIN_DIR}/${PROJECT_NAME}.map)
#add_link_options(-Wl,--strip-all)# 好像没有作用

# 设置链接目标
add_executable(${PROJECT_NAME}.elf ${SOURCES} ${LINKER_SCRIPT})

# 导入模块
target_sources(libapp PUBLIC
        FILE_SET CXX_MODULES FILES
        ${cxx_modules})


# 是否链接静态库
if (STATIC_MODULE_LD)
    target_link_libraries(${PROJECT_NAME}.elf PRIVATE libapp)
    # 设置可执行文件的输出目录
    set_target_properties(${PROJECT_NAME}.elf PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${BIN_DIR})
endif ()
target_link_libraries(${PROJECT_NAME}.elf INTERFACE :NetworkRuntime910_CM4_GCC.a)
target_link_directories(${PROJECT_NAME}.elf INTERFACE Library/ST/AI/Lib)

# 定义hex和bin文件
set(HEX_FILE ${BIN_DIR}/${PROJECT_NAME}.hex)
set(BIN_FILE ${BIN_DIR}/${PROJECT_NAME}.bin)

# 添加自定义命令：
# 生成十六进制和二进制文件
add_custom_command(TARGET ${PROJECT_NAME}.elf POST_BUILD
        COMMAND ${CMAKE_OBJCOPY} -Oihex $<TARGET_FILE:${PROJECT_NAME}.elf> ${HEX_FILE}
        COMMAND ${CMAKE_OBJCOPY} -Obinary $<TARGET_FILE:${PROJECT_NAME}.elf> ${BIN_FILE}
        COMMENT "Building ${HEX_FILE}   Building ${BIN_FILE}")