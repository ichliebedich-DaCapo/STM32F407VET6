# 引入bsp依赖
include(bsp_drivers.cmake)

# 创建可扩展的BSP库
#add_library(bsp OBJECT)

# -----------根据项目需求选择驱动----------
if (EXISTS "${PROJECT_SOURCE_DIR}/projects/${TARGET_PROJECT}/bsp_components.cmake")
    include(${PROJECT_SOURCE_DIR}/projects/${TARGET_PROJECT}/bsp_components.cmake)
else ()
    # 在Projects下对应的项目目录里添加bsp_components.cmake
    message(WARNING "Can not find bsp_components.cmake!!")
    set(BSP_COMPONENTS lcd adc) # 默认组件
endif ()

# 初始化已添加组件列表和源文件列表
set(ADDED_COMPONENTS "" CACHE INTERNAL "List of added components")
set(BSP_SRCS "" CACHE INTERNAL "List of BSP source files")
set(BSP_INC_DIRS "${CMAKE_CURRENT_BINARY_DIR}" "${CORE_INC_DIRS}" CACHE INTERNAL "List of BSP header dirs")


# 递归解析依赖
function(add_driver_component comp)
    # 检查当前组件是否已经添加
    list(FIND ADDED_COMPONENTS ${comp} idx)
    if (idx EQUAL -1)
        # 添加当前组件到已添加列表中
        list(APPEND ADDED_COMPONENTS ${comp})
        set(ADDED_COMPONENTS "${ADDED_COMPONENTS}" CACHE INTERNAL "List of added components")

                message(STATUS "ADDED_COMPONENTS:${ADDED_COMPONENTS}")

        # 查找依赖项
        foreach (driver ${BSP_DRIVERS})
            string(REGEX MATCH "^${comp}:" match ${driver})
            if (match)
                # 提取依赖项部分（去掉驱动名称和冒号）
                string(REPLACE "${comp}:" "" deps_str ${driver})

                # 将依赖项字符串按逗号分割成列表
                string(REPLACE "," ";" deps_list "${deps_str}")

                # 递归处理每个依赖项
                foreach (dep ${deps_list})
                    add_driver_component(${dep})
                endforeach ()
            endif ()
        endforeach ()

        # 添加源文件
        if (${comp}_SRCS)
            #            target_sources(bsp PRIVATE ${${comp}_SRCS})
            list(APPEND BSP_SRCS ${${comp}_SRCS})
            set(BSP_SRCS "${BSP_SRCS}" CACHE INTERNAL "List of BSP source files")
        endif ()
    endif ()
endfunction()



# ------------------------解析BSP依赖-------------------------------
# 添加默认组件
list(APPEND BSP_COMPONENTS "default")
foreach (comp ${BSP_COMPONENTS})
    # 将组件名称转换为大写
    string(TOUPPER ${comp} COMP_UPPER)
    # 设置对应的宏定义为ON
    set(BSP_USE_${COMP_UPPER} ON CACHE INTERNAL "Enable ${comp} driver")
    add_driver_component(${comp})
endforeach ()

# 确保能识别到bsp_config.h
#target_include_directories(bsp PRIVATE ${CMAKE_CURRENT_BINARY_DIR})

# 生成BSP配置头文件
configure_file(
        ${CMAKE_CURRENT_SOURCE_DIR}/inc/bsp_config.h.in
        ${CMAKE_CURRENT_BINARY_DIR}/bsp_config.h
)

message(STATUS "BSP_SRCS--:${BSP_SRCS}")