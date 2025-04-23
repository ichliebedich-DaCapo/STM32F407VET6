cmake_minimum_required(VERSION 3.29)
project(Simulator LANGUAGES C CXX)
include(common_functions.cmake)

# ------------------------ 配置选项 ------------------------
set(PROJECT_DIR ../../../Projects/driversDevelop)
set(PROJECT_ROOT_DIR "${CMAKE_CURRENT_SOURCE_DIR}/..")
set(THIRD_PARTY_DIR "${CMAKE_CURRENT_SOURCE_DIR}/../../../Middleware/Third_Party")

# 自动识别工具链类型 ！！切换工具链时需要把cmake产物删除干净！！
if(MSVC)
    # Visual Studio 工具链
    set(SDL2_ROOT "${CMAKE_CURRENT_SOURCE_DIR}/SDL2/VC")
    set(SDL2_LIB_DIR "${SDL2_ROOT}/lib/x64")
    message(STATUS "[Config] Using MSVC toolchain")
elseif(CMAKE_CXX_COMPILER_ID MATCHES "GNU")
    # 区分MinGW架构
    message(WARNING "[Config] MinGW64 GCC 14.2 对module和inline特性的支持不完善，会出问题")
    if(CMAKE_SYSTEM_PROCESSOR MATCHES "x86_64" OR CMAKE_SIZEOF_VOID_P EQUAL 8)
        set(SDL2_ROOT "${CMAKE_CURRENT_SOURCE_DIR}/SDL2/x86_64-w64-mingw32")
        message(STATUS "[Config] Using 64-bit MinGW")
    else()
        set(SDL2_ROOT "${CMAKE_CURRENT_SOURCE_DIR}/SDL2/i686-w64-mingw32")
        message(STATUS "[Config] Using 32-bit MinGW")
    endif()
    set(SDL2_LIB_DIR "${SDL2_ROOT}/lib")
else()
    message(FATAL_ERROR "[Config] Unsupported toolchain: ${CMAKE_CXX_COMPILER_ID}")
endif()

set(LVGL_DIR "${THIRD_PARTY_DIR}/LVGL/lvgl")
set(UI_DIR "${PROJECT_DIR}/ui")
set(GUI_DIR "${CMAKE_CURRENT_SOURCE_DIR}/..")

# 输出目录
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin")

# ------------------------ 编译选项 ------------------------
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# MSVC 专用选项
if (MSVC)
    # C++20 模块和概念支持
    string(APPEND CMAKE_CXX_FLAGS " /std:c++latest")

    # 根据构建类型设置优化选项
    if (CMAKE_BUILD_TYPE STREQUAL "Release")
        add_compile_options(/O2)
        string(APPEND CMAKE_CXX_FLAGS " /RTC0")  # Release 禁用运行时检查
    else()
        add_compile_options(/Od)  # Debug 禁用优化
    endif()

    # 调试符号生成
    add_compile_options(/Zi)
else()
    # GCC/Clang 选项
    add_compile_options(-O2 -g -fmodules-ts)
endif()



# ------------------------ 组件库定义 ------------------------
file(GLOB_RECURSE LVGL_SRCS "${LVGL_DIR}/src/*.c" "${LVGL_DIR}/examples/porting/*.c")
file(GLOB_RECURSE DRIVERS_SRCS "${CMAKE_CURRENT_SOURCE_DIR}/Drivers/*.c" "${CMAKE_CURRENT_SOURCE_DIR}/Drivers/*.cpp")
file(GLOB_RECURSE RENDER_SRCS "${PROJECT_ROOT_DIR}/Render/*.c"  "${PROJECT_ROOT_DIR}/Render/*.cpp")
file(GLOB_RECURSE UI_SRCS "test.cpp" "${UI_DIR}/*.c" "${UI_DIR}/*.cpp")
file(GLOB cxx_modules "${GUI_DIR}/*.ixx" "${GUI_DIR}/Compose/*.ixx" "${UI_DIR}/*.ixx")
message(STATUS ${cxx_modules})
# ------------------------ 可执行目标 ------------------------
add_executable(${PROJECT_NAME} "${CMAKE_CURRENT_SOURCE_DIR}/main.cpp")

# 添加模块文件
target_sources(${PROJECT_NAME} PUBLIC
        FILE_SET CXX_MODULES
        BASE_DIRS ${GUI_DIR} ${UI_DIR}
        FILES ${cxx_modules}
)

# 添加普通源文件
target_sources(${PROJECT_NAME} PRIVATE
        ${LVGL_SRCS}
        ${DRIVERS_SRCS}
        ${RENDER_SRCS}
        ${UI_SRCS}
)

# 包含目录
target_include_directories(${PROJECT_NAME} PRIVATE
        ${GUI_DIR}
        ${UI_DIR}
        ${LVGL_DIR}
        ${LVGL_DIR}/examples/porting
        ${CMAKE_CURRENT_SOURCE_DIR}/Drivers
        ${SDL2_ROOT}/include
        ${PROJECT_ROOT_DIR}/Render
)

# ------------------------ 依赖配置 ------------------------
find_library(SDL2_LIB SDL2 HINTS "${SDL2_LIB_DIR}" REQUIRED)# 查找库
target_link_libraries(${PROJECT_NAME} PRIVATE ${SDL2_LIB})# 链接库

# 复制 SDL2.dll
add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy
        "${SDL2_LIB_DIR}/SDL2.dll"
        $<TARGET_FILE_DIR:${PROJECT_NAME}>
)