set(GUI_DIR ${ADAPTER_DIR}/GUI)


# -------------------GUI库-------------------
if (GUI_ENABLE)
    file(GLOB_RECURSE GUI_SRCS
            "${GUI_DIR}/Base/*.cpp"
            "${GUI_DIR}/Component/src/*.cpp"
            "${GUI_DIR}/Render/*.cpp"
    )
    set(GUI_INC_DIRS
            ${GUI_DIR}/Base
            ${GUI_DIR}/Component/inc
            ${GUI_DIR}/Render
    )
    add_library(libgui STATIC ${GUI_SRCS})
    target_include_directories(libgui PUBLIC ${GUI_INC_DIRS})
    # 依赖lvgl库
    target_link_libraries(libgui PRIVATE liblvgl libbsp)
    # 设置静态库的输出目录
    set_target_properties(libgui PROPERTIES ARCHIVE_OUTPUT_DIRECTORY ${LIB_DIR})
endif ()


# ----------------------Adapter层------------------------
set(ADAPTER_INC_DIRS

)

set(ADAPTER_SRCS

)

# 控制GUI的编译
if (GUI_ENABLE)
    list(APPEND ADAPTER_INC_DIRS
            ${GUI_INC_DIRS}
    )
    list(APPEND ADAPTER_SRCS
            ${GUI_SRCS}
    )
endif ()