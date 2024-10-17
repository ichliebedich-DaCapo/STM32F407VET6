cmake_minimum_required(VERSION 3.10)
project(Simulator)

# 设置编译器
set(CMAKE_C_COMPILER gcc)

# 定义路径
set(LIBFILE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/../lib/native)
set(LVGL_DIR ${CMAKE_CURRENT_SOURCE_DIR}/..)
set(SIMULATOR_DIR ${CMAKE_CURRENT_SOURCE_DIR})
set(PROJECT_SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/..)

# 生成的目标文件目录
set(BUILD_DIR ${CMAKE_CURRENT_BINARY_DIR}/build)
set(OBJ_DIR ${BUILD_DIR}/object)
set(GEN_OBJ_DIR ${OBJ_DIR}/generated)
set(BIN_DIR ${BUILD_DIR}/bin)

# 编译选项
add_compile_options(-O2 -g0 -DLV_CONF_INCLUDE_SIMPLE=1)

# 链接选项
set(LIBRARIES decoder openh264 rlottie pthread stdc++ jansson curl m SDL2)
link_directories(${LIBFILE_PATH} ${SIMULATOR_DIR}/SDL2/i686-w64-mingw32/lib ${GEN_OBJ_DIR})

# 源文件
file(GLOB_RECURSE LVGL_SRCS "${LVGL_DIR}/lvgl/*.c")
file(GLOB_RECURSE DRIVER_SRCS "${SIMULATOR_DIR}/lv_drivers/*.c")
file(GLOB_RECURSE CUSTOM_SRCS "${PROJECT_SOURCE_DIR}/custom/*.c")
file(GLOB_RECURSE GENERATED_SRCS "${PROJECT_SOURCE_DIR}/generated/*.c")

# 定义静态库
add_library(libgenerated STATIC
        ${LVGL_SRCS}
        ${DRIVER_SRCS}
        ${CUSTOM_SRCS}
        ${GENERATED_SRCS}
)
target_include_directories(libgenerated PUBLIC
        ${LVGL_DIR}
        ${SIMULATOR_DIR}
        ${LVGL_DIR}/lvgl
        ${SIMULATOR_DIR}/lv_drivers
        ${PROJECT_SOURCE_DIR}/custom
        ${PROJECT_SOURCE_DIR}/generated
        ${SIMULATOR_DIR}/SDL2/i686-w64-mingw32/include
)
set_target_properties(libgenerated PROPERTIES ARCHIVE_OUTPUT_DIRECTORY ${GEN_OBJ_DIR})

# 创建可执行文件
add_executable(simulator ${SIMULATOR_DIR}/main.c)
target_link_libraries(simulator PRIVATE libgenerated ${LIBRARIES})
target_include_directories(simulator PRIVATE
        ${SIMULATOR_DIR}
)
set_target_properties(simulator PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${BIN_DIR})

# 创建DLL
add_library(simulator_dll SHARED ${SIMULATOR_DIR}/main.c)
target_link_libraries(simulator_dll PRIVATE libgenerated ${LIBRARIES})
target_include_directories(simulator_dll PRIVATE
        ${SIMULATOR_DIR}
)
set_target_properties(simulator_dll PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY ${BIN_DIR}
        LINK_FLAGS "--entry=_DllMainCRTStartup@12"
)

# 复制所需的库文件
add_custom_target(copy_libs
        COMMAND ${CMAKE_COMMAND} -E copy_if_different ${SIMULATOR_DIR}/SDL2/lib/SDL2.dll ${BIN_DIR}/SDL2.dll
        COMMAND ${CMAKE_COMMAND} -E copy_if_different ${SIMULATOR_DIR}/multi_thread/libgcc_s_dw2-1.dll ${BIN_DIR}/libgcc_s_dw2-1.dll
        COMMAND ${CMAKE_COMMAND} -E copy_if_different ${SIMULATOR_DIR}/multi_thread/pthreadGC-3.dll ${BIN_DIR}/pthreadGC-3.dll
        COMMAND ${CMAKE_COMMAND} -E copy_if_different ${SIMULATOR_DIR}/multi_thread/libjansson-4.dll ${BIN_DIR}/libjansson-4.dll
        COMMAND ${CMAKE_COMMAND} -E copy_if_different ${SIMULATOR_DIR}/multi_thread/libcurl.dll ${BIN_DIR}/libcurl.dll
        COMMAND ${CMAKE_COMMAND} -E copy_if_different ${SIMULATOR_DIR}/SDL2/lib/libopenh264.dll ${BIN_DIR}/libopenh264.dll
)
add_dependencies(simulator copy_libs)

# 运行模拟器
add_custom_target(run
        COMMAND ${CMAKE_COMMAND} -E echo "Running simulator..."
        COMMAND ${BIN_DIR}/simulator
)