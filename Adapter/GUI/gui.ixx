module;
#ifdef STM32F407xx
#include<project_config.h>
#endif
export module gui;


// ======== 导出所有分区 =========
#ifdef GUI_ENABLE
export import :ui;
#endif
export import :render;






