# 定义查找源文件的函数
function(find_sources variable)
    set(files)
    foreach(dir IN LISTS ARGN)
        file(GLOB_RECURSE found_files "${dir}/*.cpp" "${dir}/*.c")
        list(APPEND files ${found_files})
    endforeach()
    set(${variable} ${files} PARENT_SCOPE)
endfunction()



# 打印信息
function(print info)
    message(STATUS "-->[Info]:${info}")
endfunction()