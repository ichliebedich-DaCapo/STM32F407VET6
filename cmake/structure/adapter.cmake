set(GUI_DIR ${ADAPTER_DIR}/GUI)

# -------------------GUI库-------------------
file(GLOB_RECURSE GUI_SRCS
        # GUI衍生物
        "${UI_DIR}/*.cpp"
        "${UI_DIR}/*.c"
)

set(GUI_INC_DIRS
        # GUI组件
        ${GUI_DIR}/Component
        # GUI衍生物
        ${UI_DIR}
)


# ----------------------Adapter层------------------------
set(ADAPTER_INC_DIRS )

set(ADAPTER_SRCS )

# -------控制GUI的编译------
if (GUI_ENABLE)
    list(APPEND ADAPTER_INC_DIRS ${GUI_INC_DIRS})
    list(APPEND ADAPTER_SRCS ${GUI_SRCS})

    file(GLOB_RECURSE adapter_modules
            "${GUI_DIR}/Compose/*.ixx"
            "${GUI_DIR}/*.ixx"
            "${UI_DIR}/*.ixx"
    )
endif ()



