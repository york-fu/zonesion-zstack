/*********************************************************************************************
* 文件：sensor.c
* 作者：Zhoucj 2018.6.25
* 说明：火焰传感器
*       节点具备三个属性：定时上报火焰报警值，远程控制火焰传感器开关
*       变量A0表示火焰报警值
*       V0表示火焰报警值主动上报时间间隔
*       D0(Bit0)分别表示A0主动上报使能
*       D1表示燃气阀门继电器开关状态，OD1/CD1进行控制
*       默认值：A0=0,V0=30,D0=1,D1=1
* 修改：
* 注释：
*********************************************************************************************/

/*********************************************************************************************
* 头文件
*********************************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "sapi.h"
#include "osal_nv.h"
#include "addrmgr.h"
#include "mt.h"
#include "hal_led.h"
#include "hal_adc.h"
#include "hal_uart.h"
#include "sensor.h"
#include "zxbee.h"
#include "zxbee-inf.h"
/*********************************************************************************************
* 宏定义
*********************************************************************************************/
#define RELAY1                  P0_6                            // 定义继电器控制引脚
#define RELAY2                  P0_7                            // 定义继电器控制引脚
#define ON                      0                               // 宏定义打开状态控制为ON
#define OFF                     1                               // 宏定义关闭状态控制为OFF

#define D_FLAME_BET             P0_4                            // 火焰传感器检测引脚
/*********************************************************************************************
* 全局变量
*********************************************************************************************/
static uint8 D0 = 1;                                            // 默认打开主动上报功能
static uint8 D1 = 1;                                            // 继电器初始状态为打开继电器1
static uint8 A0 = 0;                                            // A0存储火焰报警值
static uint16 V0 = 30;                                          // V0设置为上报时间间隔，默认为30s
/*********************************************************************************************
* 名称：updateV0()
* 功能：更新V0的值
* 参数：*val -- 待更新的变量
* 返回：
* 修改：
* 注释：
*********************************************************************************************/
void updateV0(char *val)
{
  //将字符串变量val解析转换为整型变量赋值
  V0 = atoi(val);                                               // 获取数据上报时间更改值
}
/*********************************************************************************************
* 名称：updateA0()
* 功能：更新A0的值
* 参数：
* 返回：
* 修改：
* 注释：
*********************************************************************************************/
void updateA0(void)
{
  if (D_FLAME_BET){                                             // 判断管脚的电平状态值
    A0 = 0;                                                     // 检测为高电平时没有检测到火焰A0=0
  }else{
    A0 = 1;                                                     // 否则检测到火焰A0=1        
  }
}
/*********************************************************************************************
* 名称：updateA1()
* 功能：更新A1的值
* 参数：
* 返回：
* 修改：
* 注释：
*********************************************************************************************/
void updateA1(void)
{

}
/*********************************************************************************************
* 名称：sensorInit()
* 功能：传感器硬件初始化
* 参数：无
* 返回：无
* 修改：
* 注释：
*********************************************************************************************/
void sensorInit(void)
{
  // 初始化火焰传感器代码
  P0SEL &= ~0x10;
  P0DIR &= ~0x10;
  
  //P0_4上拉
  P0INP &= ~(1 << 4);
  P2INP &= ~(1 << 5);                                           

  // 初始化继电器代码
  P0SEL &= ~0xC0;                                               // 配置管脚为通用IO模式
  P0DIR |= 0xC0;                                                // 配置控制管脚为输出模式
  
  sensorControl(D1);                                            // 初始化继电器状态

  // 启动定时器，触发传感器上报数据事件：MY_REPORT_EVT
  osal_start_timerEx(sapi_TaskID, MY_REPORT_EVT, (uint16)((osal_rand()%10) * 1000));
  // 启动定时器，触发传感器监测事件：MY_CHECK_EVT
  osal_start_timerEx(sapi_TaskID, MY_CHECK_EVT, 100);
}
/*********************************************************************************************
* 名称：sensorLinkOn()
* 功能：传感器节点入网成功调用函数
* 参数：无
* 返回：无
* 修改：
* 注释：
*********************************************************************************************/
void sensorLinkOn(void)
{
  sensorUpdate();
}
/*********************************************************************************************
* 名称：sensorUpdate()
* 功能：处理主动上报的数据
* 参数：无
* 返回：无
* 修改：
* 注释：
*********************************************************************************************/
void sensorUpdate(void)
{ 
  char pData[16];
  char *p = pData;
  
  ZXBeeBegin();                                                 // 智云数据帧格式包头
  
  // 根据D0的位状态判定需要主动上报的数值
  if ((D0 & 0x01) == 0x01){                                     // 若火焰报警值上报允许，则pData的数据包中添加火焰报警值数据
    updateA0();
    sprintf(p, "%u", A0); 
    ZXBeeAdd("A0", p);
  }
  
  sprintf(p, "%u", D1);                                         // 上报控制编码 
  ZXBeeAdd("D1", p);
  
  p = ZXBeeEnd();                                               // 智云数据帧格式包尾
  if (p != NULL) {												
    ZXBeeInfSend(p, strlen(p));	                                // 将需要上传的数据进行打包操作，并通过zb_SendDataRequest()发送到协调器
  }
}
/*********************************************************************************************
* 名称：sensorCheck()
* 功能：传感器监测
* 参数：无
* 返回：无
* 修改：
* 注释：
*********************************************************************************************/
void sensorCheck(void)
{
  static uint8 last_A0 = 0;
  updateA0();
  if(last_A0 != A0)
  {
    sensorUpdate();
    last_A0 = A0;
  }
}
/*********************************************************************************************
* 名称：sensorControl()
* 功能：传感器控制
* 参数：cmd - 控制命令
* 返回：无
* 修改：
* 注释：
*********************************************************************************************/
void sensorControl(uint8 cmd)
{
  // 根据cmd参数处理对应的控制程序
  if(cmd & 0x01){ 
    RELAY1 = ON;                                                // 开启继电器1
  }
  else{
    RELAY1 = OFF;                                               // 关闭继电器1
  }
  if(cmd & 0x02){  
    RELAY2 = ON;                                                // 开启继电器2
  }
  else{
    RELAY2 = OFF;                                               // 关闭继电器2        
  }
}
/*********************************************************************************************
* 名称：ZXBeeUserProcess()
* 功能：解析收到的控制命令
* 参数：*ptag -- 控制命令名称
*       *pval -- 控制命令参数
* 返回：ret -- pout字符串长度
* 修改：
* 注释：
*********************************************************************************************/
int ZXBeeUserProcess(char *ptag, char *pval)
{ 
  int val;
  int ret = 0;	
  char pData[16];
  char *p = pData;
  
  // 将字符串变量pval解析转换为整型变量赋值
  val = atoi(pval);	
  // 控制命令解析
  if (0 == strcmp("CD0", ptag)){                                // 对D0的位进行操作，CD0表示位清零操作
    D0 &= ~val;
  }
  if (0 == strcmp("OD0", ptag)){                                // 对D0的位进行操作，OD0表示位置一操作
    D0 |= val;
  }
  if (0 == strcmp("D0", ptag)){                                 // 查询上报使能编码
    if (0 == strcmp("?", pval)){
      ret = sprintf(p, "%u", D0);
      ZXBeeAdd("D0", p);
    } 
  }
  if (0 == strcmp("CD1", ptag)){                                // 对D1的位进行操作，CD1表示位清零操作
    D1 &= ~val;
    sensorControl(D1);                                          // 处理执行命令
  }
  if (0 == strcmp("OD1", ptag)){                                // 对D1的位进行操作，OD1表示位置一操作
    D1 |= val;
    sensorControl(D1);                                          // 处理执行命令
  }
  if (0 == strcmp("D1", ptag)){                                 // 查询执行器命令编码
    if (0 == strcmp("?", pval)){
      ret = sprintf(p, "%u", D1);
      ZXBeeAdd("D1", p);
    } 
  }
  if (0 == strcmp("A0", ptag)){ 
    if (0 == strcmp("?", pval)){
      updateA0();
      ret = sprintf(p, "%u", A0);     
      ZXBeeAdd("A0", p);
    } 
  }
  if (0 == strcmp("V0", ptag)){
    if (0 == strcmp("?", pval)){
      ret = sprintf(p, "%u", V0);                         		// 上报时间间隔
      ZXBeeAdd("V0", p);
    }else{
      updateV0(pval);
    }
  }
  return ret;
}
/*********************************************************************************************
* 名称：MyEventProcess()
* 功能：自定义事件处理
* 参数：event -- 事件编号
* 返回：无
* 修改：
* 注释：
*********************************************************************************************/
void MyEventProcess( uint16 event )
{
  if (event & MY_REPORT_EVT) { 
    sensorUpdate();
    //启动定时器，触发事件：MY_REPORT_EVT 
    osal_start_timerEx(sapi_TaskID, MY_REPORT_EVT, V0*1000);
  }  
  if (event & MY_CHECK_EVT) { 
    sensorCheck(); 
    // 启动定时器，触发事件：MY_CHECK_EVT 
    osal_start_timerEx(sapi_TaskID, MY_CHECK_EVT, 1000);
  } 
}