/*********************************************************************************************
* 文件：MP-4.c
* 作者：Lixm 2017.10.17
* 说明：可燃气体驱动代码
* 修改：
* 注释：
*********************************************************************************************/

/*********************************************************************************************
* 头文件
*********************************************************************************************/
#include "MP-4.h"

/*********************************************************************************************
* 名称：combustiblegas_init()
* 功能：可燃气体传感器初始化
* 参数：无
* 返回：无
* 修改：
* 注释：
*********************************************************************************************/
void combustiblegas_init(void)
{
  GPIO_InitTypeDef  GPIO_InitStructure;
  ADC_CommonInitTypeDef ADC_CommonInitStructure;
  ADC_InitTypeDef       ADC_InitStructure;
  
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);         //使能GPIOC时钟
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE);          //使能ADC1时钟
  
  //先初始化ADC1通道11 IO口
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;                     //PC1 通道11
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AN;                  //模拟输入
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL ;             //不带上下拉
  GPIO_Init(GPIOC, &GPIO_InitStructure);                        //初始化  
  
  RCC_APB2PeriphResetCmd(RCC_APB2Periph_ADC1,ENABLE);	        //ADC1复位
  RCC_APB2PeriphResetCmd(RCC_APB2Periph_ADC1,DISABLE);	        //复位结束	 
  
  ADC_CommonInitStructure.ADC_Mode = ADC_Mode_Independent;      //独立模式
  //两个采样阶段之间的延迟5个时钟
  ADC_CommonInitStructure.ADC_TwoSamplingDelay = ADC_TwoSamplingDelay_5Cycles;
   //DMA失能
  ADC_CommonInitStructure.ADC_DMAAccessMode = ADC_DMAAccessMode_Disabled;
  ADC_CommonInitStructure.ADC_Prescaler = ADC_Prescaler_Div4;   //预分频4分频。ADCCLK=PCLK2/4=84/4=21Mhz,ADC时钟最好不要超过36Mhz 
  ADC_CommonInit(&ADC_CommonInitStructure);                     //初始化
  
  ADC_InitStructure.ADC_Resolution = ADC_Resolution_12b;        //12位模式
  ADC_InitStructure.ADC_ScanConvMode = DISABLE;                 //非扫描模式	
  ADC_InitStructure.ADC_ContinuousConvMode = DISABLE;           //关闭连续转换
  //禁止触发检测，使用软件触发
  ADC_InitStructure.ADC_ExternalTrigConvEdge = ADC_ExternalTrigConvEdge_None;
  ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;        //右对齐	
  ADC_InitStructure.ADC_NbrOfConversion = 1;                    //1个转换在规则序列中 也就是只转换规则序列1 
  ADC_Init(ADC1, &ADC_InitStructure);                           //ADC初始化
  
  ADC_Cmd(ADC1, ENABLE);                                        //开启AD转换器	
}

/*********************************************************************************************
* 名称：unsigned char get_combustiblegas_status(void)
* 功能：获取可燃气体传感器状态
* 参数：无
* 返回：无
* 修改：
* 注释：
*********************************************************************************************/
extern u16 AdcGet(u8 ch);
unsigned int get_combustiblegas_data(void)
{ 
  //ADC1,ADC通道,480个周期,提高采样时间可以提高精确度	
//  ADC_RegularChannelConfig(ADC1, ADC_Channel_11, 1, ADC_SampleTime_480Cycles );			     
//  ADC_SoftwareStartConv(ADC1);		                            //使能指定的ADC1的软件转换启动功能	 
//  while(!ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC ));               //等待转换结束
//  return ADC_GetConversionValue(ADC1);	                        //返回最近一次ADC1规则组的转换结果
  unsigned short value = 0;
  value = AdcGet(2);
  if(value >= 1500)
      return 1;
  else
      return 0;
}