cmake_minimum_required(VERSION 3.10)
project(Simulator LANGUAGES C CXX)
include(common_functions.cmake)

# ------------------------ 配置选项 ------------------------
set(PROJECT_DIR ../../../Projects/driversDevelop)  # 项目选择
options_enable_static_lib_ld()

# ------------------------ 路径配置 ------------------------
# 基础路径
set(PROJECT_ROOT_DIR "${CMAKE_CURRENT_SOURCE_DIR}/..")
set(THIRD_PARTY_DIR "${CMAKE_CURRENT_SOURCE_DIR}/../../../Middleware/Third_Party")

# 组件路径配置
set(SDL2_ROOT "${CMAKE_CURRENT_SOURCE_DIR}/SDL2/i686-w64-mingw32")
set(LVGL_DIR "${THIRD_PARTY_DIR}/LVGL/lvgl")
set(UI_DIR "${PROJECT_DIR}/ui")  # 通过PROJECT_DIR自动生成

# 输出目录统一配置
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin")

# ------------------------ 编译选项 ------------------------
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
add_compile_options(-O2 -g)
string(APPEND CMAKE_CXX_FLAGS " -fconcepts")  # 修正flags设置方式

# ------------------------ 依赖配置 ------------------------
# SDL2配置
find_library(SDL2_LIB SDL2 HINTS "${SDL2_ROOT}/lib" REQUIRED)

# ------------------------ 组件库定义 ------------------------
# LVGL库
set(LVGL_INC_DIRS "${LVGL_DIR}" "${LVGL_DIR}/examples/porting")
find_sources(LVGL_SRCS
        "${LVGL_DIR}/src"
        "${LVGL_DIR}/examples/porting"
)
add_library(lvgl STATIC ${LVGL_SRCS})
target_include_directories(lvgl PUBLIC ${LVGL_INC_DIRS})

# Drivers库
set(DRIVERS_INC_DIRS
        "${CMAKE_CURRENT_SOURCE_DIR}/Drivers"
        "${LVGL_DIR}"
        "${SDL2_ROOT}/include")
find_sources(DRIVERS_SRCS "${CMAKE_CURRENT_SOURCE_DIR}/Drivers")
add_library(drivers STATIC ${DRIVERS_SRCS})
target_link_libraries(drivers PUBLIC ${SDL2_LIB})
target_include_directories(drivers PUBLIC ${DRIVERS_INC_DIRS})

# Render库
set(RENDER_INC_DIRS "${PROJECT_ROOT_DIR}/Render")
find_sources(RENDER_SRCS "${PROJECT_ROOT_DIR}/Render")
add_library(render STATIC ${RENDER_SRCS})
target_include_directories(render PUBLIC ${RENDER_INC_DIRS})

# GUI库（聚合组件）
set(GUI_INC_DIRS
        "${UI_DIR}"
        "${PROJECT_ROOT_DIR}/Base"
        "${PROJECT_ROOT_DIR}/Component/inc")
find_sources(GUI_COMPONENTS
        "${PROJECT_ROOT_DIR}/Component/src"
        "${PROJECT_ROOT_DIR}/Base"
        "${UI_DIR}"
)
set(GUI_SRCS ${GUI_COMPONENTS} "test.cpp")
add_library(gui STATIC ${GUI_SRCS})
target_include_directories(gui PUBLIC ${GUI_INC_DIRS})
target_link_libraries(gui PUBLIC lvgl render drivers)

# ------------------------ 可执行目标 ------------------------
add_executable(${PROJECT_NAME} "${CMAKE_CURRENT_SOURCE_DIR}/main.cpp")

if (STATIC_LIB_LD)
    target_link_libraries(${PROJECT_NAME} PRIVATE gui)
else ()
    # 直接编译模式自动继承所有依赖
    target_sources(${PROJECT_NAME} PRIVATE
            ${LVGL_SRCS}
            ${DRIVERS_SRCS}
            ${RENDER_SRCS}
            ${GUI_SRCS}
    )
    include_directories(
            ${GUI_INC_DIRS}
            ${RENDER_INC_DIRS}
            ${DRIVERS_INC_DIRS}
            ${LVGL_INC_DIRS})
    target_link_libraries(${PROJECT_NAME} PRIVATE ${SDL2_LIB})

endif ()

# ------------------------ 后期处理 ------------------------
# 自动复制SDL2.dll
add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy
        "${SDL2_ROOT}/bin/SDL2.dll"
        $<TARGET_FILE_DIR:${PROJECT_NAME}>
)

# 工具链提示
if (CMAKE_TOOLCHAIN_FILE)
    message(STATUS "Using toolchain file: ${CMAKE_TOOLCHAIN_FILE}")
endif ()