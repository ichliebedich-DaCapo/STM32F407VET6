# 引入bsp依赖
include(bsp_drivers.cmake)

# 创建可扩展的BSP库
#add_library(bsp OBJECT)

# -----------根据项目需求选择驱动----------
if (EXISTS "${PROJECT_SOURCE_DIR}/projects/${TARGET_PROJECT}/project_config.cmake")
    include(${PROJECT_SOURCE_DIR}/projects/${TARGET_PROJECT}/project_config.cmake)
else ()
    # 在Projects下对应的项目目录里添加bsp_components.cmake
    message(WARNING "Can not find bsp_components.cmake!!")
    set(BSP_COMPONENTS lcd adc) # 默认组件
endif ()


# 初始化已添加组件列表和源文件列表
set(ADDED_COMPONENTS "" CACHE INTERNAL "List of added components")
set(BSP_SRCS "" CACHE INTERNAL "List of BSP source files")
set(BSP_INC_DIRS "${DRIVERS_INC_DIRS}" CACHE INTERNAL "List of BSP header dirs")
#message(STATUS "BSP:${BSP_INC_DIRS}")


# 递归解析依赖
function(add_driver_component comp)
    # 检查当前组件是否已经添加
    list(FIND ADDED_COMPONENTS ${comp} idx)
    if (idx EQUAL -1)
        # 添加当前组件到已添加列表中
        list(APPEND ADDED_COMPONENTS ${comp})
        set(ADDED_COMPONENTS "${ADDED_COMPONENTS}" CACHE INTERNAL "List of added components")

        # message(STATUS "ADDED_COMPONENTS:${ADDED_COMPONENTS}")

        # 根据组件名称设置对应的变量（HAL或BSP）
        string(TOUPPER ${comp} COMP_UPPER)
        if (comp MATCHES "^hal_")
            # HAL组件：去掉前缀"hal_"，设置HAL_USE_<COMPONENT>
            string(REGEX REPLACE "^hal_" "" HAL_COMP ${comp})
            string(REGEX REPLACE "_ex$" "" HAL_COMP ${HAL_COMP})  # 去除可能的"_ex"后缀
            string(TOUPPER ${HAL_COMP} HAL_COMP_UPPER)
            set(HAL_${HAL_COMP_UPPER}_MODULE_ENABLED ON CACHE INTERNAL "Enable HAL component ${HAL_COMP}")
        elseif (comp MATCHES "^ll_")
            # LL组件（可选）：设置LL_USE_<COMPONENT>
            string(REGEX REPLACE "^ll_" "" LL_COMP ${comp})
            string(TOUPPER ${LL_COMP} LL_COMP_UPPER)
            set(LL_USE_${LL_COMP_UPPER} ON CACHE INTERNAL "Enable LL component ${LL_COMP}")
        else()
            # BSP组件：设置BSP_USE_<COMPONENT>
            set(BSP_USE_${COMP_UPPER} ON CACHE INTERNAL "Enable BSP component ${comp}")
        endif()

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
        if (${comp}_SRC)
            #            target_sources(bsp PRIVATE ${${comp}_SRCS})
            list(APPEND BSP_SRCS ${${comp}_SRC})
            set(BSP_SRCS "${BSP_SRCS}" CACHE INTERNAL "List of BSP source files")
        endif ()
    endif ()
endfunction()


# ------------------------解析BSP依赖-------------------------------
# 添加默认组件
list(APPEND BSP_COMPONENTS "default")
foreach (comp ${BSP_COMPONENTS})
    add_driver_component(${comp})
endforeach ()

# 确保能识别到bsp_config.h
#target_include_directories(bsp PRIVATE ${CMAKE_CURRENT_BINARY_DIR})

# 生成BSP配置头文件
configure_file(
        ${CMAKE_CURRENT_SOURCE_DIR}/bsp_config.h.in
        ${INC_DIR}/bsp_config.h
)

# 生成hal配置头文件
configure_file(
        ${CMAKE_CURRENT_SOURCE_DIR}/stm32f4xx_hal_conf.h.in
        ${DRIVERS_DIR}/stm32f4xx_hal_conf.h
)

message(STATUS "BSP_SRCS--:${BSP_SRCS}")