/*********************************************************************************************
* 文件：rgb.h
* 作者：Meixin 2017.09.15
* 说明：rgb灯驱动代码头文件  
* 修改：liutong 20171027 修改了rgb控制引脚初始化函数名称、优化了注释
* 注释：
*********************************************************************************************/

/*********************************************************************************************
* 宏条件编译
*********************************************************************************************/
#ifndef __rgb_H__
#define __rgb_H__

/*********************************************************************************************
* 头文件
*********************************************************************************************/
/*********************************************************************************************
* 宏定义
*********************************************************************************************/
#define RGB_R                      GPIO_Pin_1                   //宏定义RGB_R灯控制引脚P0_0
#define RGB_G                      GPIO_Pin_0                   //宏定义RGB_G灯控制引脚P0_1
#define RGB_B                      GPIO_Pin_3                   //宏定义RGB_B灯控制引脚P0_2

#define RGB_R_port                 GPIOB                        //宏定义RGB_R灯控制引脚P0_0
#define RGB_G_port                 GPIOB                        //宏定义RGB_G灯控制引脚P0_1
#define RGB_B_port                 GPIOA                        //宏定义RGB_B灯控制引脚P0_2

       
/*********************************************************************************************
* 函数声明
*********************************************************************************************/
void rgb_init(void);                                            //rgb控制引脚初始化函数
signed char rgb_on(unsigned char rgb);                          //rgb打开控制函数
signed char rgb_off(unsigned char rgb);                         //rgb关闭控制函数

#endif /*__rgb_H_*/

