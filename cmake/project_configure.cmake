set(CMAKE_SYSTEM_NAME Generic)
set(CMAKE_SYSTEM_VERSION 1)

#设置编译目标类型为静态，必须放在project语句前面
set(CMAKE_TRY_COMPILE_TARGET_TYPE STATIC_LIBRARY)



# ------设置编译目录-----
set(BUILD_DIR ${CMAKE_CURRENT_BINARY_DIR}/build)
set(LIB_DIR ${BUILD_DIR}/lib)
set(BIN_DIR ${BUILD_DIR}/bin)
ensure_directory(${BUILD_DIR})
ensure_directory(${LIB_DIR})
ensure_directory(${BIN_DIR})


# ------设置各文件目录------
set(ALGORITHM_DIR ${CMAKE_SOURCE_DIR}/Algorithm)
set(CORE_DIR ${CMAKE_SOURCE_DIR}/Core)
set(BSP_DIR ${CMAKE_SOURCE_DIR}/BSP)
set(ADAPTER_DIR ${CMAKE_SOURCE_DIR}/Adapter)
set(MIDDLEWARE_DIR ${CMAKE_SOURCE_DIR}/Middleware)
set(PROJECTS_DIR ${CMAKE_SOURCE_DIR}/Projects)
