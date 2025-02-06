# 确定目录是否存在
function(ensure_directory dir_path)
    if (NOT EXISTS ${dir_path})
        # 如果目录不存在，则创建它
        file(MAKE_DIRECTORY ${dir_path})
        message(STATUS "Directory created: ${dir_path}")
    endif ()
endfunction()


# 定义一个宏来设置BSP组件
macro(set_bsp_components)
    # 遍历所有传入的参数并添加到BSP_COMPONENTS列表中
    foreach (comp ${ARGN})
        list(APPEND BSP_COMPONENTS ${comp})
    endforeach ()
endmacro()

# -----------------------------项目宏----------------------------------
# 在项目下的bsp_components.cmake中使用
# 不使用option，是因为这些变量不需要缓存
# 定义的这些变量最后会在在配置头文件里定义相关宏
set(GUI_ENABLE ON CACHE INTERNAL "GUI option")
set(FREERTOS_ENABLE OFF CACHE INTERNAL "FreeRTOS option")
set(FREERTOS_DEBUG OFF CACHE INTERNAL "FreeRTOS debug option")
set(AI_ENABLE OFF CACHE INTERNAL "AI option")
set(STATIC_LIB_LD OFF CACHE INTERNAL "Independent static library link option")


# 关闭GUI
macro(options_disable_GUI)
    set(GUI_ENABLE OFF CACHE INTERNAL "GUI option")
endmacro()

# 开启FreeRTOS
macro(options_enable_FreeRTOS)
    set(FREERTOS_ENABLE ON CACHE INTERNAL "FreeRTOS option")
endmacro()

# 开启FreeRTOS调试
macro(options_enable_FreeRTOS_debug)
    set(FREERTOS_DEBUG ON CACHE INTERNAL "FreeRTOS debug option")
endmacro()

# ------------不在project_config.h.in中---------------
# 开启AI
macro(options_enable_AI)
    set(AI_ENABLE ON)
endmacro()

# 开启静态库链接
macro(options_enable_static_lib_ld)
    set(STATIC_LIB_LD ON CACHE INTERNAL "Independent static library link option")
endmacro()

# -----------------------------脚本宏----------------------------------
# 创建项目
function(create_project project_name)
    if (EXISTS ${CMAKE_SOURCE_DIR}/Projects/${project_name})
        # 项目已经存在，请在CMAkeLists里清除该宏的调用
        message(WARNING "Project component already exists: ${project_name}")
        return()
    endif ()

    # 创建 Projects 目录结构
    file(MAKE_DIRECTORY ${CMAKE_SOURCE_DIR}/Projects/${project_name}/app)
    file(MAKE_DIRECTORY ${CMAKE_SOURCE_DIR}/Projects/${project_name}/ui)

    # 生成 App 模板文件
    configure_file(
            ${CMAKE_SOURCE_DIR}/Tools/templates/app.cpp.in
            ${CMAKE_SOURCE_DIR}/Projects/${project_name}/app/app.cpp
    )

    # 生成 UI 模板文件
    configure_file(
            ${CMAKE_SOURCE_DIR}/Tools/templates/ui.hpp.in
            ${CMAKE_SOURCE_DIR}/Projects/${project_name}/ui/ui.hpp
    )
    configure_file(
            ${CMAKE_SOURCE_DIR}/Tools/templates/ui.cpp.in
            ${CMAKE_SOURCE_DIR}/Projects/${project_name}/ui/ui.cpp
    )

    message(STATUS "Templates generated for component: ${project_name}")
endfunction()


# 创建BSP
function(create_bsp bsp_name)
    if (EXISTS ${CMAKE_SOURCE_DIR}/BSP/inc/${bsp_name}.h OR EXISTS ${CMAKE_SOURCE_DIR}/BSP/src/${bsp_name}.c)
        # BSP文件已经存在，请在CMAkeLists里清除该宏的调用
        message(WARNING "BSP component already exists: ${bsp_name}")
        return()
    endif ()

    set(COMPONENT_NAME ${bsp_name})
    string(TOUPPER ${bsp_name} COMPONENT_NAME_UPPER)

    # 生成 BSP 头文件和源文件
    configure_file(
            ${CMAKE_SOURCE_DIR}/Tools/templates/bsp_component.h.in
            ${CMAKE_SOURCE_DIR}/BSP/inc/${bsp_name}.h
    )
    configure_file(
            ${CMAKE_SOURCE_DIR}/Tools/templates/bsp_component.c.in
            ${CMAKE_SOURCE_DIR}/BSP/src/${bsp_name}.c
    )
    message(STATUS "${CMAKE_SOURCE_DIR}")
    message(STATUS "BSP files generated for component: ${bsp_name}")
endfunction()