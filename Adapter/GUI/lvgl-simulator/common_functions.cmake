# -------------------工具函数-------------------
# 打印信息
function(print info)
    message(STATUS "-->[Info]:${info}")
endfunction()

# 定义查找源文件的函数
function(find_sources variable)
    set(files)
    foreach(dir IN LISTS ARGN)
        file(GLOB_RECURSE found_files "${dir}/*.cpp" "${dir}/*.c")
        list(APPEND files ${found_files})
    endforeach()
    set(${variable} ${files} PARENT_SCOPE)
endfunction()

# ------------------配置项-----------------
# 变量定义
set(STATIC_LIB_LD OFF CACHE INTERNAL "Independent static library link option")

# 开启静态库链接
macro(options_enable_static_lib_ld)
    set(STATIC_LIB_LD ON CACHE INTERNAL "Independent static library link option")
endmacro()
