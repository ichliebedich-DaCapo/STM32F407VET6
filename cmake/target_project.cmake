# --------------------------------------自动化更新配置------------------------------
# --------对选择的项目进行字符串处理，以匹配对应的UI文件---------
# 提取 APP_DIR 的末尾部分
string(REGEX REPLACE "^.*/(.+)$" "\\1" APP_DIR_LAST_PART "${TARGET_PROJECT_DIR}")
set(TARGET_PROJECT ${APP_DIR_LAST_PART})
# 设置APP和UI目录
set(APP_DIR ${TARGET_PROJECT_DIR}/app)
set(UI_DIR ${TARGET_PROJECT_DIR}/ui)
set(AI_DIR ${TARGET_PROJECT_DIR}/ai)
include(${TARGET_PROJECT_DIR}/project_config.cmake)

