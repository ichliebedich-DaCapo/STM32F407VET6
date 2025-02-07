



# ----------------------------Algorithm层------------------------------
set(ALGORITHM_INC_DIRS ${ALGORITHM_DIR}/inc)
file(GLOB_RECURSE ALGORITHM_SRCS "${ALGORITHM_DIR}/src/*.cpp" "${ALGORITHM_DIR}/src/*.cppm")

add_library(libalgorithm STATIC ${ALGORITHM_SRCS})
target_include_directories(libalgorithm PUBLIC ${ALGORITHM_INC_DIRS})
# 设置静态库的输出目录
set_target_properties(libalgorithm PROPERTIES ARCHIVE_OUTPUT_DIRECTORY ${LIB_DIR})




# -------------------module特性-----------------------
file(GLOB_RECURSE cxx_modules
        "${ALGORITHM_DIR}/inc/*.ixx"
        "${ALGORITHM_DIR}/src/*.cppm"
)