set(GUI_DIR ${ADAPTER_DIR}/GUI)
set(AI_DIR ${ADAPTER_DIR}/AI)

# -------------------GUI库-------------------
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

# ----------------AI库----------------
file(GLOB_RECURSE AI_SRCS "${AI_DIR}/${APP_DIR_LAST_PART}/App/*.c")
set(AI_INC_DIRS ${AI_DIR}/${APP_DIR_LAST_PART}/App ${AI_DIR}/${APP_DIR_LAST_PART})


# ----------------------Adapter层------------------------
set(ADAPTER_INC_DIRS

)

set(ADAPTER_SRCS

)

# -------控制GUI的编译------
if (GUI_ENABLE)
    list(APPEND ADAPTER_INC_DIRS ${GUI_INC_DIRS})
    list(APPEND ADAPTER_SRCS ${GUI_SRCS})

    add_library(libgui STATIC ${GUI_SRCS})
    target_include_directories(libgui PUBLIC ${GUI_INC_DIRS})
    # 依赖lvgl库
    target_link_libraries(libgui PRIVATE liblvgl libbsp)
    # 设置静态库的输出目录
    set_target_properties(libgui PROPERTIES ARCHIVE_OUTPUT_DIRECTORY ${LIB_DIR})
endif ()

# -------控制AI的编译------
if (AI_ENABLE)
    list(APPEND ADAPTER_INC_DIRS ${AI_INC_DIRS})
    list(APPEND ADAPTER_SRCS ${AI_SRCS})

    add_library(libai STATIC ${AI_SRC})
    target_include_directories(libai PUBLIC ${AI_DIR})
    target_link_libraries(libai INTERFACE :NetworkRuntime910_CM4_GCC.a)
    target_link_libraries(libai PRIVATE libdrivers)
    target_link_directories(libai INTERFACE Library/ST/AI/Lib)
    # 设置静态库的输出目录
    set_target_properties(libai PROPERTIES ARCHIVE_OUTPUT_DIRECTORY ${LIB_DIR})
endif ()

# -----------------合并后的库---------------
if(ADAPTER_SRCS)
# 使用 OBJECT 库来避免重复编译
add_library(AdapterObjects OBJECT ${ADAPTER_SRCS})

# 创建合并库
add_library(libadapter STATIC $<TARGET_OBJECTS:AdapterObjects>)
target_include_directories(libadapter PUBLIC ${ADAPTER_INC_DIRS})
set_target_properties(libadapter PROPERTIES ARCHIVE_OUTPUT_DIRECTORY ${LIB_DIR})
endif ()