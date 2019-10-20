/*********************************************************************************************
* 文件：beep.h
* 作者：Lixm 2017.10.17
* 说明：蜂鸣器驱动代码头文件  
* 修改：
* 注释：
*********************************************************************************************/

/*********************************************************************************************
* 宏条件编译
*********************************************************************************************/
#ifndef __BEEP_H__
#define __BEEP_H__

/*********************************************************************************************
* 头文件
*********************************************************************************************/
#include <ioCC2540.h>

#define BEEPIO                   P0_3                            //定义蜂鸣器控制引脚

/*********************************************************************************************
* 内部原型函数
*********************************************************************************************/
void beep_init(void);                                            //继电器初始化

#endif /*__BEEP_H__*/

