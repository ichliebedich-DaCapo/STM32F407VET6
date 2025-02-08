
# -------------------------------Projects层--------------------------------
# ---------------------App--------------------
file(GLOB_RECURSE APP_SRCS "${APP_DIR}/*.cpp" "${PROJECTS_DIR}/shared/*.cpp")
set(APP_INC_DIRS ${APP_DIR} ${PROJECTS_DIR}/shared)


# 生成公共宏定义配置头文件
configure_file(
        ${PROJECTS_DIR}/shared/common_config.h.in
        ${INC_DIR}/common_config.h
)
# 生成PROJECT配置头文件
configure_file(
        ${PROJECTS_DIR}/shared/project_config.h.in
        ${INC_DIR}/project_config.h
)


# ----------------------------------app库-------------------------
add_library(libapp STATIC ${APP_SRCS})
target_include_directories(libapp PUBLIC ${APP_INC_DIRS})
# ----------------Adapter层-----------------
if (GUI_ENABLE)
    target_link_libraries(libapp PUBLIC libgui)
endif ()
if (AI_ENABLE)
    target_link_libraries(libapp PUBLIC libai)
endif ()
# ------------------Middle层------------------
# 部分算法库不需要给Adapter作适配
target_link_libraries(libapp PUBLIC libdata libdsp)
# -----------------Algorithm层-----------------
target_link_libraries(libapp PUBLIC libbsp libalgorithm)
set_target_properties(libapp PROPERTIES ARCHIVE_OUTPUT_DIRECTORY ${LIB_DIR})


# ----------------------------包含所有目录和资源文件--------------------------
# Core层包含HAL层和CMSIS层，均合并到BSP层

set(ALL_INC_DIRS
        # 公共配置层
        ${INC_DIR}
        # Algorithm层
        ${ALGORITHM_INC_DIRS}
        # Adapter层
        ${ADAPTER_INC_DIRS}
        # BSP层
        ${BSP_INC_DIRS}
        # Middleware层
        ${MIDDLEWARE_INC_DIRS}
        # Projects层
        ${APP_INC_DIRS}
)

set(ALL_SRCS
        # BSP层
        ${BSP_SRCS}
        # Algorithm层
        ${ALGORITHM_SRCS}
        # Adapter层
        ${ADAPTER_SRCS}
        # Core层
        ${CORE_SRCS}
        # Middleware层
        ${MIDDLEWARE_SRCS}
        # Projects层
        ${APP_SRCS}
)

# 静态库的设计思路应是不常改的给封装为库，其他文件全部添加到ALL_SRCS
if (STATIC_LIB_LD)
    set(SOURCES ${CORE_SRCS})
    include_directories(${INC_DIR})
else ()
    # 添加公共的头文件目录、源文件
    include_directories(${ALL_INC_DIRS})
    set(SOURCES ${ALL_SRCS})
endif ()
message(STATUS "SOURCES: ${SOURCES}")

