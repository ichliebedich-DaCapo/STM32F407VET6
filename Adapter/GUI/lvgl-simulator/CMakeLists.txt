cmake_minimum_required(VERSION 3.29)
project(Simulator LANGUAGES C CXX)
include(common_functions.cmake)

# ------------------------ 配置选项 ------------------------
set(PROJECT_DIR ../../../Projects/driversDevelop)  # 项目选择
#options_enable_static_lib_ld()
# 引入命名空间后，这该死的静态库链接容易出问题
#message(WARNING "static_lib_ld has some problems!")
# ------------------------ 路径配置 ------------------------
# 基础路径
set(PROJECT_ROOT_DIR "${CMAKE_CURRENT_SOURCE_DIR}/..")
set(THIRD_PARTY_DIR "${CMAKE_CURRENT_SOURCE_DIR}/../../../Middleware/Third_Party")

# 组件路径配置
#set(SDL2_ROOT "${CMAKE_CURRENT_SOURCE_DIR}/SDL2/i686-w64-mingw32")
set(SDL2_ROOT "${CMAKE_CURRENT_SOURCE_DIR}/SDL2/x86_64-w64-mingw32")
set(LVGL_DIR "${THIRD_PARTY_DIR}/LVGL/lvgl")
set(UI_DIR "${PROJECT_DIR}/ui")  # 通过PROJECT_DIR自动生成
set(GUI_DIR "${CMAKE_CURRENT_SOURCE_DIR}/../Compose")
# 输出目录统一配置
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin")

# ------------------------ 编译选项 ------------------------
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
if (CMAKE_BUILD_TYPE STREQUAL "Release")
    add_compile_options(/O2)
    string(APPEND CMAKE_CXX_FLAGS " /RTC0")  # 显式关闭运行时检查
else()
    add_compile_options(/Og)  # Debug 模式禁用优化
endif()
#string(APPEND CMAKE_CXX_FLAGS " -fconcepts -fmodules-ts")  # 修正flags设置方式
string(APPEND CMAKE_CXX_FLAGS " /std:c++latest")

# ------------------------ 依赖配置 ------------------------
# SDL2配置
find_library(SDL2_LIB SDL2 HINTS "${SDL2_ROOT}/lib" REQUIRED)

# ------------------------ 组件库定义 ------------------------
# LVGL库
set(LVGL_INC_DIRS "${LVGL_DIR}" "${LVGL_DIR}/examples/porting")
file(GLOB_RECURSE LVGL_SRCS "${LVGL_DIR}/src/*.c" "${LVGL_DIR}/examples/porting/*.c")

# Drivers库
set(DRIVERS_INC_DIRS "${CMAKE_CURRENT_SOURCE_DIR}/Drivers" "${LVGL_DIR}" "${SDL2_ROOT}/include")
file(GLOB_RECURSE DRIVERS_SRCS "${CMAKE_CURRENT_SOURCE_DIR}/Drivers/*.c" "${CMAKE_CURRENT_SOURCE_DIR}/Drivers/*.cpp")

# Render库
set(RENDER_INC_DIRS "${PROJECT_ROOT_DIR}/Render")
file(GLOB_RECURSE RENDER_SRCS "${PROJECT_ROOT_DIR}/Render/*.c"  "${PROJECT_ROOT_DIR}/Render/*.cpp")

# GUI库（聚合组件）
set(UI_INC_DIRS "${UI_DIR}")
file(GLOB_RECURSE UI_SRCS "test.cpp" "${UI_DIR}/*.c" "${UI_DIR}/*.cpp")
file(GLOB_RECURSE cxx_modules "${GUI_DIR}/*.ixx" "${UI_DIR}/*.ixx")

# ------------------------ 可执行目标 ------------------------
add_compile_options(-Winline)
add_link_options(-Wl,-gc-sections,--relax,--as-needed,-Map=${CMAKE_BINARY_DIR}/${PROJECT_NAME}.map)
add_executable(${PROJECT_NAME} "${CMAKE_CURRENT_SOURCE_DIR}/main.cpp" )
target_sources(${PROJECT_NAME} PUBLIC
        FILE_SET CXX_MODULES
        BASE_DIRS
        ${GUI_DIR}
        ${UI_DIR}
        FILES
        ${cxx_modules}
)
target_include_directories(${PROJECT_NAME} PUBLIC ${GUI_DIR} ${UI_DIR})

# 直接编译模式自动继承所有依赖
target_sources(${PROJECT_NAME} PRIVATE
        ${LVGL_SRCS}
        ${DRIVERS_SRCS}
        ${RENDER_SRCS}
        ${UI_SRCS}
)
target_include_directories(${PROJECT_NAME} PRIVATE
        ${LVGL_INC_DIRS}
        ${DRIVERS_INC_DIRS}
        ${RENDER_INC_DIRS}
        ${UI_INC_DIRS}
)

target_link_libraries(${PROJECT_NAME} PRIVATE ${SDL2_LIB})


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

# 因为32位编译比64位快，所以没有使用64位编译工具链