# ----------------------------------设置你的UI路径（选择项目）------------------------------
set(UI_DIR ../UI/signalGenerator)
# ----------------------------------设置你的UI路径（选择项目）------------------------------

# 提取最后一个斜杠后的部分
string(REGEX REPLACE ".*/" "" APP "${UI_DIR}")

cmake_minimum_required(VERSION 3.10)
project(Simulator)

# 设置编译器
set(CMAKE_C_COMPILER gcc)
set(CMAKE_CXX_STANDARD 20)

# 定义路径
set(PROJECT_SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/..)
set(APP_DIR ${CMAKE_CURRENT_SOURCE_DIR}/../../Application)
set(BASE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/../Base)
set(LVGL_DIR ${CMAKE_CURRENT_SOURCE_DIR}/resource)
set(SIMULATOR_DIR ${CMAKE_CURRENT_SOURCE_DIR})
set(COMPONENT_DIR ${PROJECT_SOURCE_DIR}/Component)
set(EVENTS_DIR ${PROJECT_SOURCE_DIR}/Events)
set(RENDER_DIR ${PROJECT_SOURCE_DIR}/Render)
# 生成的目标文件目录
set(BUILD_DIR ${CMAKE_CURRENT_BINARY_DIR}/build)
set(BIN_DIR ${BUILD_DIR}/bin)
set(LIB_DIR ${BUILD_DIR}/lib)

# 编译选项
# 开-O2和-O3有时候会卡死，但相同情况下真机没有问题
add_compile_options(-O2 -g -DLV_CONF_INCLUDE_SIMPLE=1)

# 添加链接选项,显示内存分布，但现在不能使用
#add_link_options(-Wl,-gc-sections,--print-memory-usage,-Map=${BIN_DIR}/${PROJECT_NAME}.map)


#------------------------------------------库-------------------------------------------
# 添加公共目录
include_directories(${APP_DIR}/Conf ${APP_DIR}/${APP})

#---------------lvgl库---------------
file(GLOB_RECURSE LVGL_SRCS "resource/lvgl/*.c")
add_library(liblvgl STATIC ${LVGL_SRCS})
target_include_directories(liblvgl PUBLIC
        ${LVGL_DIR}
        ${LVGL_DIR}/lvgl

        ${SIMULATOR_DIR}
)
# 设置静态库的输出目录
set_target_properties(liblvgl PROPERTIES ARCHIVE_OUTPUT_DIRECTORY ${LIB_DIR})

#---------------Driver库---------------
file(GLOB_RECURSE DRIVER_SRCS "${SIMULATOR_DIR}/lv_drivers/*.c")
add_library(libdriver STATIC ${DRIVER_SRCS})
target_include_directories(libdriver PUBLIC
        ${SIMULATOR_DIR}/lv_drivers
        ${SIMULATOR_DIR}
        ${LVGL_DIR}
        ${SIMULATOR_DIR}/SDL2/i686-w64-mingw32/include
)
# 设置静态库的输出目录
set_target_properties(libdriver PROPERTIES ARCHIVE_OUTPUT_DIRECTORY ${LIB_DIR})

#---------------Render库---------------
# 渲染，即实现底层绘制算法
file(GLOB_RECURSE RENDER_SRCS "${RENDER_DIR}/*.cpp")
add_library(librender STATIC ${RENDER_SRCS})
target_include_directories(librender PUBLIC
        ${RENDER_DIR}
)
target_link_libraries(librender PUBLIC libdriver)
# 设置静态库的输出目录
set_target_properties(librender PROPERTIES ARCHIVE_OUTPUT_DIRECTORY ${LIB_DIR})


#---------------GUI库---------------
file(GLOB_RECURSE COMPONENT_SRCS "${COMPONENT_DIR}/*.*")
file(GLOB_RECURSE EVENTS_SRCS "${EVENTS_DIR}/*.*")
file(GLOB_RECURSE BASE_SRCS "${BASE_DIR}/*.*")
file(GLOB_RECURSE UI_SRCS "${UI_DIR}/*.*" "../UI/test.cpp")
add_library(libgui STATIC
        ${COMPONENT_SRCS}
        ${BASE_SRCS}
        ${EVENTS_SRCS}
        ${UI_SRCS}
)
target_include_directories(libgui PUBLIC
        ${UI_DIR}
        ${BASE_DIR}
        ${COMPONENT_DIR}
        ${EVENTS_DIR}
)
target_link_libraries(libgui PUBLIC liblvgl librender)
# 设置静态库的输出目录
set_target_properties(libgui PROPERTIES ARCHIVE_OUTPUT_DIRECTORY ${LIB_DIR})


# 链接外部库
set(LIBRARIES pthread stdc++ m SDL2 decoder openh264 rlottie jansson curl)
set(LIBFILE_PATH resource/lib/native)
# 全局设置应该链接的库的目录
link_directories(${LIBFILE_PATH} ${SIMULATOR_DIR}/SDL2/i686-w64-mingw32/lib)

# 创建可执行文件
add_executable(${PROJECT_NAME} ${SIMULATOR_DIR}/main.cpp)
target_link_libraries(${PROJECT_NAME} PRIVATE libdriver libgui ${LIBRARIES})
# 动态库或者可执行文件的生成位置
set_target_properties(${PROJECT_NAME} PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${BIN_DIR})

# 复制程序运行所必需的库文件
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
#
### 获取当前日期和时间
#string(TIMESTAMP CURRENT_DATE_TIME "%Y-%m-%d %H:%M:%S")
#set(SIZE_OUTPUT_FILE "size_history.txt")
#
## 显示exe文件
#add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
#        COMMAND echo.
#        COMMAND echo ${CURRENT_DATE_TIME} >> ${SIZE_OUTPUT_FILE}
#        COMMAND ${CMAKE_SIZE_UTIL} ${BIN_DIR}/${PROJECT_NAME}.exe
#        COMMAND ${CMAKE_SIZE_UTIL} ${BIN_DIR}/${PROJECT_NAME}.exe >> ${SIZE_OUTPUT_FILE}
#        COMMAND echo "--------------------------------------------------------------------------------------------" >> ${SIZE_OUTPUT_FILE}
#        COMMENT "Displaying size of the executable"
#)
