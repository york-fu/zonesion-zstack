/*********************************************************************************************
* 文件：Infrared.h
* 作者：Lixm 2017.10.17
* 说明：空气质量传感器驱动代码头文件  
* 修改：
* 注释：
*********************************************************************************************/

/*********************************************************************************************
* 宏条件编译
*********************************************************************************************/
#ifndef __MP_503_H__
#define __MP_503_H__

/*********************************************************************************************
* 头文件
*********************************************************************************************/
#include <ioCC2540.h>

/*********************************************************************************************
* 内部原型函数
*********************************************************************************************/
void airgas_init(void);                                         //空气质量传感器初始化
unsigned int get_airgas_data(void);                             //获取空气质量传感器状态

#endif /*__MP_503_H__*/

