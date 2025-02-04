

# ------------------------BSP库-----------------------------
file(GLOB_RECURSE BSP_SRCS "${BSP_DIR}/src/*.c")
set(BSP_INC_DIRS ${BSP_DIR}/inc)
add_library(libbsp STATIC ${BSP_SRCS})
target_include_directories(libbsp PUBLIC ${BSP_INC_DIRS})
target_link_libraries(libbsp PUBLIC libdrivers)
# 设置静态库的输出目录
set_target_properties(libbsp PROPERTIES ARCHIVE_OUTPUT_DIRECTORY ${LIB_DIR})