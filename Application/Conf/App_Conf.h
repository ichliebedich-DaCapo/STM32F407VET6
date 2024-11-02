//
// Created by fairy on 2024/10/14 21:04.
//
#ifndef FURINA_APP_CONF_H
#define FURINA_APP_CONF_H

/**
 * @brief 应用级配置
 * @note 决定了哪些项目启用，哪些项目关闭
 *      选择你需要的项目,然后把对应的宏设置为1，就可以启用了
*/
/************************应用级***************************/
#define ENABLE_VOICE_STORAGE_AND_PLAY 0 // 语音存储和播放
#define ENABLE_SIGNAL_GENERATOR 1   // 信号发生器（简单的双音频）

/**************************常用应用级*****************************/
#define ENABLE_KEY_TASK_HANDLE 1


#endif //FURINA_APP_CONF_H