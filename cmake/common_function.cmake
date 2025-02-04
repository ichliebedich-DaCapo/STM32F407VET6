function(ensure_directory dir_path)
    if(EXISTS ${dir_path})
        # 如果目录存在，使用 STATUS 类型的消息
        message(STATUS "Directory already exists: ${dir_path}")
    else()
        # 如果目录不存在，则创建它，并使用 WARNING 类型的消息以引起注意
        file(MAKE_DIRECTORY ${dir_path})
        message(STATUS "Directory created: ${dir_path}")
    endif()
endfunction()

