# ----------------------------------设置你的UI路径（选择项目）------------------------------
set(UI_DIR ../UI/driversDevelop)
# ----------------------------------选项------------------------------
#set(USE_STATIC_LIBRARY True)

# ----------------------------------自动根据选项来配置------------------------------

# ----------------------------------选项------------------------------
# 提取最后一个斜杠后的部分
string(REGEX REPLACE ".*/" "" APP "${UI_DIR}")

cmake_minimum_required(VERSION 3.10)
project(Simulator C CXX)

#set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}  -std=gnu++2a")
# 定义路径
set(PROJECT_SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/..)
set(APP_DIR ${CMAKE_CURRENT_SOURCE_DIR}/../../Application)
set(SIMULATOR_DIR ${CMAKE_CURRENT_SOURCE_DIR})
set(BASE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/../Base)
set(COMPONENT_DIR ${PROJECT_SOURCE_DIR}/Component)
set(RENDER_DIR ${PROJECT_SOURCE_DIR}/Render)
set(DRIVERS_DIR ${CMAKE_CURRENT_SOURCE_DIR}/Drivers)
set(LVGL_DIR ./../../Library/Third_Party/lvgl/ ./../../Library/Third_Party/lvgl/lvgl/src ./../../Library/Third_Party/lvgl/lvgl/examples/porting)
# 生成的目标文件目录
set(BUILD_DIR ${CMAKE_CURRENT_BINARY_DIR}/build)
set(BIN_DIR ${BUILD_DIR}/bin)
set(LIB_DIR ${BUILD_DIR}/lib)
# SDL2库
set(SDL2_INCLUDE_DIR ${SIMULATOR_DIR}/SDL2/i686-w64-mingw32/include)
set(SDL2_BIN_DIR ${SIMULATOR_DIR}/SDL2/i686-w64-mingw32/bin)
set(SDL2_LIB_DIR ${SIMULATOR_DIR}/SDL2/i686-w64-mingw32/lib)


# 编译选项 开-O2和-O3有时候会卡死，但相同情况下真机没有问题
add_compile_options(-O2 -g)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fconcepts")

# 添加链接选项,显示内存分布，但现在不能使用
#add_link_options(-Wl,-gc-sections,--print-memory-usage,-Map=${BIN_DIR}/${PROJECT_NAME}.map)


#------------------------------------------库-------------------------------------------
#---------------lvgl库---------------
file(GLOB_RECURSE LVGL_SRCS "./../../Library/Third_Party/lvgl/lvgl/src/*.c" "./../../Library/Third_Party/lvgl/lvgl/examples/porting/*.c")
add_library(liblvgl STATIC ${LVGL_SRCS})
target_include_directories(liblvgl PUBLIC ${LVGL_DIR})
set_target_properties(liblvgl PROPERTIES ARCHIVE_OUTPUT_DIRECTORY ${LIB_DIR})

#---------------Drivers库---------------
file(GLOB_RECURSE DRIVERS_SRCS "${DRIVERS_DIR}/*.cpp")
add_library(libdrivers STATIC ${DRIVERS_SRCS})
target_include_directories(libdrivers PUBLIC
        ${DRIVERS_DIR}
        ${LVGL_DIR}
        ${SDL2_INCLUDE_DIR}
)
set_target_properties(libdrivers PROPERTIES ARCHIVE_OUTPUT_DIRECTORY ${LIB_DIR})

#---------------Render库---------------
# 渲染，即实现底层绘制算法
file(GLOB_RECURSE RENDER_SRCS "${RENDER_DIR}/*.cpp")
add_library(librender STATIC ${RENDER_SRCS}
        ../Render/WaveCurve.cpp)
target_include_directories(librender PUBLIC
        ${RENDER_DIR}
)
target_link_libraries(librender PUBLIC libsimulator)
set_target_properties(librender PROPERTIES ARCHIVE_OUTPUT_DIRECTORY ${LIB_DIR})


#---------------GUI库---------------
file(GLOB_RECURSE COMPONENT_SRCS "${COMPONENT_DIR}/src/*.cpp" "${COMPONENT_DIR}/inspiration.cpp")
file(GLOB_RECURSE BASE_SRCS "${BASE_DIR}/*.cpp")
file(GLOB_RECURSE UI_SRCS "${UI_DIR}/*.cpp" "${UI_DIR}/*.c" "../UI/test.cpp")
add_library(libgui STATIC
        ${COMPONENT_SRCS}
        ${BASE_SRCS}
        ${UI_SRCS}
        ../UI/StorageOscilloscope/Draw_wave.cpp
)
target_include_directories(libgui PUBLIC
        ${UI_DIR}
        ${BASE_DIR}
        ${COMPONENT_DIR}
)
target_link_libraries(libgui PUBLIC liblvgl librender)
# 设置静态库的输出目录
set_target_properties(libgui PROPERTIES ARCHIVE_OUTPUT_DIRECTORY ${LIB_DIR})


# --------资源文件-------
if (USE_STATIC_LIBRARY)
    file(GLOB_RECURSE SOURCE ${SIMULATOR_DIR}/main.cpp)
else ()
    file(GLOB_RECURSE SOURCE
            ${SIMULATOR_DIR}/main.cpp

            # lvgl
            ${LVGL_SRCS}

            # drivers
            ${DRIVERS_SRCS}

            # GUI
            ${RENDER_SRCS}
            ${COMPONENT_SRCS}
            ${BASE_SRCS}
            ${UI_SRCS}
    )
    include_directories(
            #lvgl
            ${LVGL_DIR}

            # DRIVERS
            ${DRIVERS_DIR}

            #SDL2
            ${SDL2_INCLUDE_DIR}

            # GUI
            ${RENDER_DIR}

            ${UI_DIR}
            ${BASE_DIR}
            ${COMPONENT_DIR}/inc
    )
endif ()


# 全局设置应该链接的库的目录
set(LIBRARIES SDL2)
link_directories(${SDL2_LIB_DIR})

# -----------------创建可执行文件-----------------
add_executable(${PROJECT_NAME} ${SOURCE})
# 动态库或者可执行文件的生成位置
set_target_properties(${PROJECT_NAME} PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${BIN_DIR})

if (USE_STATIC_LIBRARY)
    target_link_libraries(${PROJECT_NAME} PRIVATE libdrivers libgui ${LIBRARIES})
else ()
    target_link_libraries(${PROJECT_NAME} PRIVATE ${LIBRARIES})
endif ()

# 使用 configure_file 只有当源文件发生变化时才会复制（在CMakelists阶段操作）
configure_file(${SDL2_BIN_DIR}/SDL2.dll ${BIN_DIR}/SDL2.dll COPYONLY)


if (CMAKE_TOOLCHAIN_FILE)
    message(STATUS "Using toolchain file: ${CMAKE_TOOLCHAIN_FILE}")
endif ()

