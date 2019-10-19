/*********************************************************************************************
* 文件：photoresistance_sensor.h
* 作者：shench 2015.3.27
* 说明: 商业光照传感器，采用RS485通讯方式
* 修改：
* 注释：商业光照传感器photoresistance_sensor,以下简称phs
*********************************************************************************************/
#ifndef Noise
#define Noise

/*********************************************************************************************
* 头文件
*********************************************************************************************/
#include "hal_types.h"

/*********************************************************************************************
* 外部函数原型
*********************************************************************************************/
void Noise_init(void);
void Noise_update(void);
float get_Noise_val(void);

#endif // PHOTORESISTANCE_SENSOR_H