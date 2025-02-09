cmake_minimum_required(VERSION 3.29)
include(cmake/common_functions.cmake)
include(cmake/configure_project.cmake)
project(Furina LANGUAGES C CXX ASM)
# --------------------------------------选择你的项目--------------------------------
# 如果你是在CLion里，那么可以直接在斜杆后面输入首字母，会自动弹出相关项目选项
set(PROJECT_DIR Projects/signalGenerator)

# 创建test项目目录，其下有ui和app目录，并且各目录下有相应文件


# --------------------------------------包含子cmake--------------------------------
# 对配置项变量进行预处理，包括获取项目名称
include(cmake/target_project.cmake)
add_subdirectory(BSP)
# 包含编译器的设置及相关标志的传递
include(cmake/arm-gcc.cmake)
# 与stm32相关的一些宏，同时指定链接脚本
include(cmake/stm32f4xx.cmake)
# 包含Core层(不含Drivers）
include(cmake/structure/core.cmake)
# 包含HAL驱动库
include(cmake/structure/bsp.cmake)
# 包含算法
include(cmake/structure/algorithm.cmake)
# 包含中间件
include(cmake/structure/middleware.cmake)
# 包含适配层
include(cmake/structure/adapter.cmake)
# 项目库
include(cmake/structure/projects.cmake)


# 定义一个可执行文件目标，生成了 ELF 文件
add_executable(${PROJECT_NAME}.elf ${SOURCES})


# 导入模块
if (STATIC_LIB_LD)
        target_sources(libalgorithm PUBLIC
                FILE_SET CXX_MODULES FILES
                ${cxx_modules})
else ()
    target_sources(${PROJECT_NAME}.elf PUBLIC
            FILE_SET CXX_MODULES FILES
            ${cxx_modules})
endif ()


# 是否链接静态库
if (STATIC_LIB_LD)
    target_link_libraries(${PROJECT_NAME}.elf PRIVATE libapp)
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
        COMMAND ${CMAKE_SIZE} $<TARGET_FILE:${PROJECT_NAME}.elf>
        COMMENT "Building ${HEX_FILE}   Building ${BIN_FILE}")
