

# -----------------------UI---------------------
file(GLOB_RECURSE UI_SRCS "${UI_DIR}/*.cpp" "${UI_DIR}/*.c")
set(UI_INC_DIRS ${UI_DIR})


# -----------------------App---------------------
file(GLOB_RECURSE APP_SRCS "${APP_DIR}/*.cpp" "${PROJECTS_DIR}/shared/*.cpp")
set(APP_INC_DIRS ${APP_DIR} ${PROJECTS_DIR}/shared)
add_library(libapp STATIC ${APP_SRCS})
target_include_directories(libapp PUBLIC ${APP_INC_DIRS})
set_target_properties(libapp PROPERTIES ARCHIVE_OUTPUT_DIRECTORY ${LIB_DIR})




# -------------------------------Projects层--------------------------------
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

set(PROJECTS_INC_DIRS
        ${APP_INC_DIRS}

)
set(PROJECTS_SRCS
        ${APP_SRCS}

)


# 控制是否添加UI
if (GUI_ENABLE)
    list(APPEND PROJECTS_INC_DIRS
            ${UI_INC_DIRS}
    )
    list(APPEND PROJECTS_SRCS
            ${UI_SRCS}
    )

    add_library(libui STATIC ${UI_SRCS})
    target_include_directories(libui PUBLIC ${UI_INC_DIRS})
    target_link_libraries(libui PUBLIC libgui)
    set_target_properties(libui PROPERTIES ARCHIVE_OUTPUT_DIRECTORY ${LIB_DIR})
endif ()


# --------------------合并------------------------
add_library(ProjectsObjects OBJECT ${PROJECTS_SRCS})
add_library(libprojects STATIC $<TARGET_OBJECTS:ProjectsObjects>)
target_link_libraries(libprojects PUBLIC  libui libapp)
set_target_properties(libprojects PROPERTIES ARCHIVE_OUTPUT_DIRECTORY ${LIB_DIR})


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
        ${PROJECTS_INC_DIRS}
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
        ${PROJECTS_SRCS}
)


if (STATIC_LIB_LD)
    include_directories(${INC_DIR})
    set(SOURCES ${CORE_SRCS})
else ()
    # 添加公共的头文件目录、源文件
    include_directories(${ALL_INC_DIRS})
    set(SOURCES ${ALL_SRCS})
endif ()
    message(STATUS "SOURCES: ${SOURCES}")

