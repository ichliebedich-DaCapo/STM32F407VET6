module;
#include<project_config.h>
export module gui;


// ======== 导出所有分区 =========
#ifdef GUI_ENABLE
export import :ui;
#endif
export import :render;






