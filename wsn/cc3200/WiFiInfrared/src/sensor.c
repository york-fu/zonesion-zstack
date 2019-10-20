/*********************************************************************************************
* 文件：sensor.c
* 作者：Xuzhy 2018.5.16
* 说明：节点工程驱动模版
*       智能家居三种类型设备：采集类（温度采集）、安防类（人体红外报警状态）、控制类（家用电器）
*       数据包格式：{Temperature=124.0,infraredStatus=0,switchStatus=0}
*       temperature表示温度值，infraredStatus为人体红外报警状态（0/1），switchStatus为家用电器的状态（0：关闭，1：开空调，2：开加湿器，3：开空调和加湿器）
*       查询命令：查询数值参考（比如查询当前温度：{temperature=?}）
*       控制命令：cmd=0为关闭电器，cmd=1为打开空调，cmd=2为打开加湿器，cmd=3为打开空调和加湿器
* 修改：
* 注释：
*********************************************************************************************/

/*********************************************************************************************
* 头文件
*********************************************************************************************/
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "zxbee.h"
#include "udma_if.h"
#include "common.h"
#include "hw_ints.h"
#include "hw_types.h"
#ifndef NOTERM
#include "uart_if.h"
#endif
#include "systick_if.h"
#include "infrared.h"
#include "zxbee-inf.h"
#include "sensor.h"
/*********************************************************************************************
* 宏定义
*********************************************************************************************/
#define DEBUG 1
#if DEBUG
#define DebugMsg        DBG_PRINT 
#else
#define DebugMsg(...)
#endif
/*********************************************************************************************
* 全局变量
*********************************************************************************************/
static uint8 infraredStatus = 0;                                   // 人体红外报警状态值
/*********************************************************************************************
* 名称：updateInfraredStatus()
* 功能：更新人体红外报警值
* 参数：
* 返回：
* 修改：
* 注释：
*********************************************************************************************/
void updateInfraredStatus(void)
{
   static uint32 ct = 0;
   
  // 更新人体红外传感器的值
  infraredStatus = get_infrared_status();
  if (infraredStatus != 0) {
    ct = clock_time();
  } else if (clock_time() > ct+1000) {
    ct = 0;
    infraredStatus = 0;
  } else {
    infraredStatus = 1;
  }
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
  // 初始化传感器代码
  infrared_init();                                            // 人体红外传感器初始化
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
  
  // 人体红外报警状态更新
  updateInfraredStatus();
  
  ZXBeeBegin();                                                 // 智云数据帧格式包头

  // 更新infraredStatus的值
  sprintf(p, "%u", infraredStatus); 
  ZXBeeAdd("infraredStatus", p); 
  
  p = ZXBeeEnd();                                               // 智云数据帧格式包尾
  if (p != NULL) {												
    ZXBeeInfSend(p, strlen(p));	 
  }
  DebugMsg("sensor->sensorUpdate(): infraredStatus=%u\r\n", infraredStatus); 
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
  static char lastinfraredStatus=0;
  static uint32 ct0=0;
  char pData[16];
  char *p = pData;
  
  // 人体红外报警状态采集
  updateInfraredStatus();

  ZXBeeBegin();

  if (lastinfraredStatus != infraredStatus || (ct0 != 0 && clock_time() > (ct0+3000))) {  // 人体红外报警状态监测
    sprintf(p, "%u", infraredStatus);
    ZXBeeAdd("infraredStatus", p);
    ct0 = clock_time();
    if (infraredStatus == 0) {
      ct0 = 0;
    }
    lastinfraredStatus = infraredStatus;
  }

  p = ZXBeeEnd();
  if (p != NULL) {
    int len = strlen(p); 
    ZXBeeInfSend(p, len);
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
  int ret = 0;	
  char pData[16];
  char *p = pData;

  // 控制命令解析
  if (0 == strcmp("infraredStatus", ptag)){                        // 查询执行器命令编码
    if (0 == strcmp("?", pval)){
      updateInfraredStatus();
      ret = sprintf(p, "%u", infraredStatus);
      ZXBeeAdd("infraredStatus", p);
    } 
  }
  
  return ret;
}
/*********************************************************************************************
* 名称：sensorLoop()
* 功能：定时触发功能
* 参数：无
* 返回：无
* 修改：
* 注释：
*********************************************************************************************/
void sensorLoop(void)
{
  static unsigned long ct_update = 0;
  static unsigned long ct_check = 0;
  
  if (t4exp(ct_update)) {
    sensorUpdate();
    ct_update = t4ms()+20*1000;
  }
  if (t4exp(ct_check)) {
    sensorCheck();
    ct_check = t4ms()+100;
  }
}