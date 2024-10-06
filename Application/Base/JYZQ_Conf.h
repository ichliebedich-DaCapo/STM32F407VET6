//
// Created by 34753 on 2024/9/22.
//
/**
 * @brief JY与ZQ协同开发の配置文件
 * @details 若有不同意见再共同磋商，其余且看且行
 * @note
 * ① 接口兼容性：BSP统一使用C接口。而模块级与应用级则使用C++接口，方便开发
 * ② 文件命名规范：BSP和模块级小写为主，不同属性用下划线隔开，方便调用接口；应用级采用驼峰命名法
 * ③ 函数：
 *      BSP：为了提高驱动性能，实现简单功能的函数尽量内敛，其内局部变量尽量使用字长单位
 * ④ 变量：尽量不使用全局变量
 *      BSP：尽量使用函数式编程，减少副作用
 * ⑤ 调试：统一开-O2级别优化，遇到问题再开-Og。在CLion里面体现为使用Release选项。同时要注意volatile变量的使用
 */


#ifndef FURINA_JYZQ_CONF_H
#define FURINA_JYZQ_CONF_H


/* 应用级*/
#define ENABLE_VOICE_STORAGE_AND_PLAY 1

/* 模块级*/
#define USE_

/* 板级*/
#define USE_



/*其他*/
// 是否开启FreeRTOS调试模式
// 开了之后在负荷不大的情况下，也并不怎么影响实时性
#define FreeRTOS_DebugMode  1





#endif //FURINA_JYZQ_CONF_H
