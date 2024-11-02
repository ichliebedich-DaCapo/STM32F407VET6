cmake_minimum_required(VERSION 3.10)
project(Simulator)

# 设置编译器
set(CMAKE_C_COMPILER gcc)
set(CMAKE_CXX_STANDARD 17)

# 定义路径
set(LIBFILE_PATH resource/lib/native)
set(PROJECT_SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/..)
set(APP_DIR ${CMAKE_CURRENT_SOURCE_DIR}/../../Application)
set(BASE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/../Base)
set(LVGL_DIR ${CMAKE_CURRENT_SOURCE_DIR}/resource)
set(SIMULATOR_DIR ${CMAKE_CURRENT_SOURCE_DIR})
set(COMPONENT_DIR ${PROJECT_SOURCE_DIR}/Component)
set(EVENTS_DIR ${PROJECT_SOURCE_DIR}/Events)
# ----------------------------------设置你的UI路径（选择项目）------------------------------
set(UI_DIR ../UI/signalGenerator)
# ----------------------------------设置你的UI路径（选择项目）------------------------------
# 生成的目标文件目录
set(BUILD_DIR ${CMAKE_CURRENT_BINARY_DIR}/build)
set(OBJ_DIR ${BUILD_DIR}/object)
set(GEN_OBJ_DIR ${OBJ_DIR}/generated)
set(BIN_DIR ${BUILD_DIR}/bin)

# 编译选项
add_compile_options(-O2 -g -DLV_CONF_INCLUDE_SIMPLE=1)

# 添加链接选项,显示内存分布，但现在不能使用
#add_link_options(-Wl,-gc-sections,--print-memory-usage,-Map=${BIN_DIR}/${PROJECT_NAME}.map)

# 链接选项
set(LIBRARIES decoder openh264 rlottie pthread stdc++ jansson curl m SDL2)
link_directories(${LIBFILE_PATH} ${SIMULATOR_DIR}/SDL2/i686-w64-mingw32/lib ${GEN_OBJ_DIR})

# 添加控制目录
include_directories(${APP_DIR}/Conf  ${APP_DIR})

# 源文件
file(GLOB_RECURSE LVGL_SRCS "${LVGL_DIR}/lvgl/*.c")
file(GLOB_RECURSE DRIVER_SRCS "${SIMULATOR_DIR}/lv_drivers/*.c")
file(GLOB_RECURSE COMPONENT_SRCS "${COMPONENT_DIR}/*.*")
file(GLOB_RECURSE EVENTS_SRCS "${EVENTS_DIR}/*.*")
file(GLOB_RECURSE BASE_SRCS "${BASE_DIR}/*.*")
file(GLOB_RECURSE UI_SRCS "${UI_DIR}/*.*")
# 定义静态库
add_library(libgenerated STATIC
        ${LVGL_SRCS}
        ${DRIVER_SRCS}
        ${COMPONENT_SRCS}
        ${BASE_SRCS}
        ${EVENTS_SRCS}
        ${UI_SRCS}
)
# 头文件目录
target_include_directories(libgenerated PUBLIC
        ${LVGL_DIR}
        ${BASE_DIR}
        ${COMPONENT_DIR}
        ${EVENTS_DIR}
        ${SIMULATOR_DIR}
        ${LVGL_DIR}/lvgl
        ${SIMULATOR_DIR}/lv_drivers
        ${SIMULATOR_DIR}/SDL2/i686-w64-mingw32/include
        ${UI_DIR}
)
set_target_properties(libgenerated PROPERTIES ARCHIVE_OUTPUT_DIRECTORY ${GEN_OBJ_DIR})

# 创建可执行文件
add_executable(${PROJECT_NAME} ${SIMULATOR_DIR}/main.cpp)
target_link_libraries(${PROJECT_NAME} PRIVATE libgenerated ${LIBRARIES})
target_include_directories(${PROJECT_NAME} PRIVATE ${SIMULATOR_DIR})
set_target_properties(${PROJECT_NAME} PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${BIN_DIR})

# 创建DLL
add_library(${PROJECT_NAME}_dll SHARED ${SIMULATOR_DIR}/main.cpp)
target_link_libraries(${PROJECT_NAME}_dll PRIVATE libgenerated ${LIBRARIES})
target_include_directories(${PROJECT_NAME}_dll PRIVATE
        ${SIMULATOR_DIR}
)
set_target_properties(${PROJECT_NAME}_dll PROPERTIES
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
add_dependencies(${PROJECT_NAME} copy_libs)

# 自定义命令，构建后执行

## 获取当前日期和时间
string(TIMESTAMP CURRENT_DATE_TIME "%Y-%m-%d %H:%M:%S")
set(SIZE_OUTPUT_FILE "size_history.txt")

# 显示exe文件
add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
        COMMAND echo.
        COMMAND echo ${CURRENT_DATE_TIME} >>  ${SIZE_OUTPUT_FILE}
        COMMAND ${CMAKE_SIZE_UTIL} ${BIN_DIR}/${PROJECT_NAME}.exe
        COMMAND ${CMAKE_SIZE_UTIL} ${BIN_DIR}/${PROJECT_NAME}.exe  >> ${SIZE_OUTPUT_FILE}
        COMMAND echo "--------------------------------------------------------------------------------------------" >> ${SIZE_OUTPUT_FILE}
        COMMENT "Displaying size of the executable"
)
