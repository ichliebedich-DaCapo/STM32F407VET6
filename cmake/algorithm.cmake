set(ALGORITHM_INC_DIRS ${ALGORITHM_DIR}/inc)
file(GLOB_RECURSE ALGORITHM_SRCS "${ALGORITHM_DIR}/src/*.cpp")

add_library(libalgorithm STATIC ${ALGORITHM_SRCS})
target_include_directories(libalgorithm PUBLIC ${ALGORITHM_INC_DIRS})
#target_link_libraries(libmodule PRIVATE libdsp libdata)
# 设置静态库的输出目录
set_target_properties(libalgorithm PROPERTIES ARCHIVE_OUTPUT_DIRECTORY ${LIB_DIR})

# -------AI库-------
if (AI_ENABLE)
    file(GLOB_RECURSE AI_SRC "AI/${APP_DIR_LAST_PART}/App/*.c")
    set(AI_DIR AI/${APP_DIR_LAST_PART}/App AI/${APP_DIR_LAST_PART})
    add_library(libai STATIC ${AI_SRC})
    target_include_directories(libai PUBLIC ${AI_DIR})
    target_link_libraries(libai INTERFACE :NetworkRuntime910_CM4_GCC.a)
    target_link_libraries(libai PRIVATE libdrivers)
    target_link_directories(libai INTERFACE Library/ST/AI/Lib)
    # 设置静态库的输出目录
    set_target_properties(libai PROPERTIES ARCHIVE_OUTPUT_DIRECTORY ${LIB_DIR})
endif ()


# -------------------module特性测试-----------------------
file(GLOB_RECURSE cxx_modules
        "${ALGORITHM_DIR}/inc/*.ixx"
        "${ALGORITHM_DIR}/src/*.cppm"
)