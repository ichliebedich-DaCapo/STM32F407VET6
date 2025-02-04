# --------------------------------------自动化更新配置------------------------------
# --------对选择的项目进行字符串处理，以匹配对应的UI文件---------
# 提取 APP_DIR 的末尾部分
string(REGEX REPLACE "^.*/(.+)$" "\\1" APP_DIR_LAST_PART "${PROJECT_DIR}")

# 设置APP和UI目录
set(APP_DIR ${PROJECT_DIR}/app)
set(UI_DIR ${PROJECT_DIR}/ui)

# -------读取对应GUI_DISABLE和FreeRTOS_ENABLE宏----------
# 获取文件中的宏
# 在app.hpp里打注释没用的原因找到了
file(READ "${APP_DIR}/app.hpp" macro_content)
# 使用正则表达式检查 GUI_DISABLE 是否被注释掉
if (macro_content MATCHES "//.*GUI_DISABLE")
    message(STATUS ">>>>>> GUI_DISABLE is commented out.")
    set(GUI_ENABLE True)
elseif (macro_content MATCHES "#define GUI_DISABLE")
    message(STATUS ">>>>>> GUI_DISABLE is defined.")
    set(GUI_ENABLE False)
else ()
    message(STATUS ">>>>>> GUI_DISABLE not found.")
    set(GUI_ENABLE True)
endif ()
# 使用正则表达式检查 FreeRTOS_ENABLE 是否被定义
if (macro_content MATCHES "//.*FreeRTOS_ENABLE")
    message(STATUS ">>>>>> FreeRTOS_ENABLE is commented out.")
    set(FreeRTOS_ENABLE False)
elseif (macro_content MATCHES "#define FreeRTOS_ENABLE")
    message(STATUS ">>>>>> FreeRTOS_ENABLE found.")
    set(FreeRTOS_ENABLE True)
else ()
    message(STATUS ">>>>>> FreeRTOS_ENABLE not found.")
    set(FreeRTOS_ENABLE False)
endif ()
