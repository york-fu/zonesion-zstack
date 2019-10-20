/*********************************************************************************************
* 文件：grating.h
* 作者：Lixm 2017.10.17
* 说明：红外光栅传感器驱动代码头文件  
* 修改：
* 注释：
*********************************************************************************************/

/*********************************************************************************************
* 宏条件编译
*********************************************************************************************/
#ifndef __GRATING_H__
#define __GRATING_H__

/*********************************************************************************************
* 头文件
*********************************************************************************************/
#include <ioCC2540.h>

/*********************************************************************************************
* 内部原型函数
*********************************************************************************************/
void grating_init(void);                                        //红外光栅传感器初始化
unsigned char get_grating_status(void);                         //获取红外光栅传感器状态

#endif /*__GRATING_H__*/

