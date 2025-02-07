

# ------------------------BSP库-----------------------------
# BSP_SRCS有BSP下的CMakeLists传递进来
list(APPEND BSP_INC_DIRS "${BSP_DIR}/inc")
#message(STATUS "${BSP_INC_DIRS}")
add_library(libbsp STATIC ${BSP_SRCS})
target_include_directories(libbsp PUBLIC ${BSP_INC_DIRS})
# 设置静态库的输出目录
set_target_properties(libbsp PROPERTIES ARCHIVE_OUTPUT_DIRECTORY ${LIB_DIR})