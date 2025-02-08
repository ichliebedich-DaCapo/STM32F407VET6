cmake_minimum_required(VERSION 3.10)
project(Simulator LANGUAGES C CXX)
include(common_functions.cmake)
# ------------------------ 选项配置 ------------------------
# 选择你的项目（必要的，不能修改）
set(PROJECT_DIR ../../../Projects/driversDevelop)
set(USE_STATIC_LIBRARY OFF)# 使用普通的变量定义即可，不需要使用option

# ------------------------ 路径配置 ------------------------
set(PROJECT_ROOT_DIR "${CMAKE_CURRENT_SOURCE_DIR}/..")
set(THIRD_PARTY_DIR "${CMAKE_CURRENT_SOURCE_DIR}/../../../Middleware/Third_Party")

# 提取最后一个斜杠后的部分（必要的）
string(REGEX REPLACE ".*/" "" APP "${PROJECT_DIR}")
set(UI_DIR ${PROJECT_DIR}/ui)

# SDL2配置
set(SDL2_ROOT "${CMAKE_CURRENT_SOURCE_DIR}/SDL2/i686-w64-mingw32")
set(SDL2_INC_DIR "${SDL2_ROOT}/include")
set(SDL2_LIB_DIR "${SDL2_ROOT}/lib")
set(SDL2_BIN_DIR "${SDL2_ROOT}/bin")


# LVGL配置
set(LVGL_DIR "${THIRD_PARTY_DIR}/LVGL")
set(LVGL_SRC_DIR "${LVGL_DIR}/lvgl/src")
set(LVGL_PORTING_DIR "${LVGL_DIR}/lvgl/examples/porting")
set(LVGL_INC_DIRS "${LVGL_PORTING_DIR}" "${LVGL_DIR}/lvgl")

# 项目组件路径
set(DRIVERS_DIR "${CMAKE_CURRENT_SOURCE_DIR}/Drivers")
set(RENDER_DIR "${PROJECT_ROOT_DIR}/Render")
set(COMPONENT_DIR "${PROJECT_ROOT_DIR}/Component")
set(COMPONENT_INC_DIR "${COMPONENT_DIR}/inc")
set(COMPONENT_SRC_DIR "${COMPONENT_DIR}/src")
set(BASE_DIR "${PROJECT_ROOT_DIR}/Base")

# 输出目录
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin")

# ------------------------ 编译选项 ------------------------
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
add_compile_options(-O2 -g)
# 给C++添加标志
set(${CMAKE_CXX_FLAGS} "${CMAKE_CXX_FLAGS} -fconcepts")

# ------------------------ 库定义 ------------------------
# LVGL库
find_sources(LVGL_SRCS
        "${LVGL_SRC_DIR}"
        "${LVGL_PORTING_DIR}"
)
add_library(lvgl STATIC ${LVGL_SRCS})
target_include_directories(lvgl PUBLIC ${LVGL_INC_DIRS})

# Drivers库
set(DRIVERS_INC_DIRS ${DRIVERS_DIR} ${LVGL_INC_DIRS} ${SDL2_INC_DIR})
find_sources(DRIVERS_SRCS "${DRIVERS_DIR}")
add_library(drivers STATIC ${DRIVERS_SRCS})
# 依赖SDL2
target_link_libraries(drivers PUBLIC SDL2)
target_include_directories(drivers PUBLIC ${DRIVERS_INC_DIRS})


# Render库
set(RENDER_INC_DIRS ${RENDER_DIR})
find_sources(RENDER_SRCS "${RENDER_DIR}")
add_library(render STATIC ${RENDER_SRCS})
target_include_directories(render PUBLIC ${RENDER_INC_DIRS})

# GUI库
find_sources(COMPONENT_SRCS "${COMPONENT_SRC_DIR}")
find_sources(BASE_SRCS "${BASE_DIR}")
find_sources(UI_SRCS "${UI_DIR}")
set(GUI_SRCS "${COMPONENT_SRCS}" "${BASE_SRCS}" "${UI_SRCS}" "test.cpp")
set(GUI_INC_DIRS ${UI_DIR} ${BASE_DIR} ${COMPONENT_INC_DIR})
add_library(gui STATIC ${GUI_SRCS})
target_include_directories(gui PUBLIC ${GUI_INC_DIRS})
target_link_libraries(gui PUBLIC lvgl render drivers)

# ------------------------ 可执行文件 ------------------------
# 主程序源文件
set(MAIN_SRC "${CMAKE_CURRENT_SOURCE_DIR}/main.cpp")
link_directories(${SDL2_LIB_DIR})
if (USE_STATIC_LIBRARY)
    # 静态库模式：链接所有静态库
    add_executable(${PROJECT_NAME} ${MAIN_SRC})
    target_link_libraries(${PROJECT_NAME} PRIVATE gui)
else ()
    # 直接编译模式：包含所有源文件
    set(ALL_SRCS "${LVGL_SRCS}" "${DRIVERS_SRCS}" "${RENDER_SRCS}" "${GUI_SRCS}")

    add_executable(${PROJECT_NAME} ${MAIN_SRC} ${ALL_SRCS})

    # 包含所有头文件目录
    target_include_directories(${PROJECT_NAME} PRIVATE
            ${LVGL_INC_DIRS}
            ${DRIVERS_INC_DIRS}
            ${RENDER_INC_DIRS}
            ${GUI_INC_DIRS}
    )

    target_link_libraries(${PROJECT_NAME} PRIVATE SDL2)
endif ()

# ------------------------ 后期配置 ------------------------
# 复制SDL2动态库
add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy
        "${SDL2_BIN_DIR}/SDL2.dll"
        $<TARGET_FILE_DIR:${PROJECT_NAME}>
)

# 工具链文件提示
if (CMAKE_TOOLCHAIN_FILE)
    message(STATUS "Using toolchain file: ${CMAKE_TOOLCHAIN_FILE}")
endif ()