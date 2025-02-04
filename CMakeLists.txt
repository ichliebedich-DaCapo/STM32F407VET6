cmake_minimum_required(VERSION 3.29)
project(Furina LANGUAGES C CXX ASM)
include(cmake/common_function.cmake)
include(cmake/project_configure.cmake)
# --------------------------------------选择你的项目--------------------------------
# 如果你是在CLion里，那么可以直接在斜杆后面输入首字母，会自动弹出相关项目选项
set(PROJECT_DIR Projects/driversDevelop)

# 是否使用独立静态库链接,使用后会额外编译一些不需要的文件,可以使用链接优化-flto
option(STATIC_LIB_LD "Independently compiled into various static libraries" OFF)
# ---------使用AI---------
option(AI_ENABLE "Enable AI" OFF)


# --------------------------------------包含子cmake--------------------------------
# 对配置项变量进行处理
include(cmake/project_handler.cmake)
# 包含编译器的设置及相关标志的传递
include(cmake/arm-gcc.cmake)
# 与stm32相关的一些宏，同时指定链接脚本
include(cmake/stm32f4xx.cmake)
# 包含HAL驱动库
include(cmake/bsp.cmake)
# 包含算法
include(cmake/algorithm.cmake)
# 包含BSP
include(cmake/core.cmake)
# 包含中间件
include(cmake/middleware.cmake)
# 包含适配层
include(cmake/adapter.cmake)
# 项目库
include(cmake/projects.cmake)


# 定义一个可执行文件目标，生成了 ELF 文件
add_executable(${PROJECT_NAME}.elf ${SOURCES} ${LINKER_SCRIPT})

# 导入模块
if (${STATIC_LIB_LD})
    target_sources(libapp PUBLIC
            FILE_SET CXX_MODULES FILES
            ${cxx_modules})
else ()
    target_sources(${PROJECT_NAME}.elf PUBLIC
            FILE_SET CXX_MODULES FILES
            ${cxx_modules})
endif ()


# 是否链接静态库
if (${STATIC_LIB_LD})
    target_link_libraries(${PROJECT_NAME}.elf PRIVATE libapp)
    # 设置可执行文件的输出目录
    set_target_properties(${PROJECT_NAME}.elf PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${BIN_DIR})
endif ()

if (${AI_ENABLE})
    target_link_libraries(${PROJECT_NAME}.elf INTERFACE :NetworkRuntime910_CM4_GCC.a)
    target_link_directories(${PROJECT_NAME}.elf INTERFACE Library/ST/AI/Lib)
endif ()

# 定义hex和bin文件
set(HEX_FILE ${BIN_DIR}/${PROJECT_NAME}.hex)
set(BIN_FILE ${BIN_DIR}/${PROJECT_NAME}.bin)
set(SYMBOL_TABLE ${BIN_DIR}/${PROJECT_NAME}.sym)    # 生成符号表
# 添加自定义命令：
# 生成十六进制和二进制文件
add_custom_command(TARGET ${PROJECT_NAME}.elf POST_BUILD
        COMMAND ${CMAKE_OBJCOPY} -Oihex $<TARGET_FILE:${PROJECT_NAME}.elf> ${HEX_FILE}
        COMMAND ${CMAKE_OBJCOPY} -Obinary $<TARGET_FILE:${PROJECT_NAME}.elf> ${BIN_FILE}
        COMMAND ${CMAKE_NM} -n $<TARGET_FILE:${PROJECT_NAME}.elf> > ${SYMBOL_TABLE}
        COMMAND ${SIZE} $<TARGET_FILE:${PROJECT_NAME}.elf>
        COMMENT "Building ${HEX_FILE}   Building ${BIN_FILE}")
