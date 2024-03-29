/*********************************************************************************************
* 文件：stadiometry.h
* 作者：Lixm 2017.10.17
* 说明：红外测距传感器驱动代码头文件  
* 修改：
* 注释：
*********************************************************************************************/

/*********************************************************************************************
* 宏条件编译
*********************************************************************************************/
#ifndef __STADIOMETRY_H__
#define __STADIOMETRY_H__

/*********************************************************************************************
* 头文件
*********************************************************************************************/
#include "hw_ints.h"
#include "hw_types.h"
#include "hw_memmap.h"
#include "hw_common_reg.h"
#include "prcm.h"
#include "pinmux.h"
#include "rom.h"
#include "rom_map.h"
#include "gpio.h"
#include "pin.h"
#include <math.h>
#include <stdio.h>

/*********************************************************************************************
* 内部原型函数
*********************************************************************************************/
void stadiometry_init(void);                                    //红外测距传感器初始化
float get_stadiometry_data(void);                               //获取红外测距传感器数据

#endif /*__STADIOMETRY_H__*/

