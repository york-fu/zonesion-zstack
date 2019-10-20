/*********************************************************************************************
* 文件：GPS.c
* 作者：zonesion
* 说明：驱动程序
* 修改：Chenkm 2017.01.04 增加了注释
* 注释：
*********************************************************************************************/
/*********************************************************************************************
* 头文件
*********************************************************************************************/
#include "hal_types.h"	
#include "iocc2530.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

/*********************************************************************************************
* 全局变量
*********************************************************************************************/

static char _lat[16], _lng[16];
static char _gps_status = '0';

static int gps_recv_ch(char ch);
/*********************************************************************************************
* 名称：uart0_init(unsigned char StopBits,unsigned char Parity)
* 功能：串口0初始化
* 参数：无
* 返回：无
* 修改：
* 注释：
****************************************** 
*    CC253O 32M系统时钟波特率参数表      * 
*----------------------------------------* 
*  波特率  UxBAUD         UxGCRM         * 
*  240     59             6              * 
*  4800    59             7              * 
*  9600    59             8              * 
*  14400   216            8              * 
*  19200   59             9              * 
*  28800   216            9              * 
*  38400   59             10             * 
*  57600   216            10             * 
*  76800   59             11             * 
*  115200  216            11             * 
*  23040   216            12             * 
****************************************** 
*********************************************************************************************/
void uart0_init(unsigned char StopBits,unsigned char Parity)
{
  P0SEL |=  0x0C;                                               //初始化UART0端口
  PERCFG&= ~0x01;                                               //选择UART0为可选位置一
  P2DIR &= ~0xC0;                                               //P0优先作为串口0
  U0CSR = 0xC0;                                                 //设置为UART模式,而且使能接受器
   
  U0GCR = 0x8;                  
  U0BAUD = 59;                                                  //波特率设置为9600
  
  //U0UCR |= 2|StopBits|Parity;                                     //设置停止位与奇偶校验
  U0UCR = 2;
  URX0IE = 1;   
}

/*********************************************************************************************
* 名称：uart_send_char()
* 功能：串口发送字节函数
* 参数：无
* 返回：无
* 修改：
* 注释：
*********************************************************************************************/
void uart_send_char(unsigned char ch)
{

  U0DBUF = ch;                                                  //将要发送的数据填入发送缓存寄存器
  while(UTX0IF == 0);                                           //等待数据发送完成
  UTX0IF = 0;                                                   //发送完成后将数据清零
}
/*********************************************************************************************
* 名称：gps_get()
* 功能：获取gps经纬度
* 参数：lat，lng 存放经纬度的缓存
* 返回：0：定位无效，1，定位有效
* 修改：
* 注释：
*********************************************************************************************/
#pragma vector=URX0_VECTOR  
__interrupt void UART0_ISR(void)  
{  
  URX0IF = 0;                                         // 清除接收中断标志  
  gps_recv_ch(U0DBUF);                                    // 填充缓冲区  
}

/*********************************************************************************************
* 名称：gps_get()
* 功能：获取gps经纬度
* 参数：lat，lng 存放经纬度的缓存
* 返回：0：定位无效，1，定位有效
* 修改：
* 注释：
*********************************************************************************************/
void gps_init(void)
{
  uart0_init(1, 0);
}
/*********************************************************************************************
* 名称：gps_get()
* 功能：获取gps经纬度
* 参数：lat，lng 存放经纬度的缓存
* 返回：0：定位无效，1，定位有效
* 修改：
* 注释：
*********************************************************************************************/
int gps_get(char *lat, char *lng)
{
  if (lat != NULL) strcpy(lat, _lat);
  if (lng != NULL) strcpy(lng, _lng);
  if (_gps_status == '1' || _gps_status == '2') {
    return 1;
  }
  return 0;
}

static char* next(char *ip, char **ot)
{
  char *e = strchr(ip, ',');
  *ot = ip;
  if (e != NULL) {
    *e = '\0';
    return e+1;
  } 
  return NULL;
}

/*********************************************************************************************
* 名称：gps_recv_ch()
* 功能：
* 参数：ch
* 返回：0
* 修改：
* 注释：
*********************************************************************************************/
static int gps_recv_ch(char ch)
{
  static char f_idx = 0;
  static char tag[128];
  if (f_idx == 0) {
    tag[0] = tag[1];
    tag[1] = tag[2];
    tag[2] = tag[3];
    tag[3] = tag[4];
    tag[4] = tag[5];
    tag[5] = ch;
    if (memcmp(tag, "$GNGGA", 6) == 0) {
      f_idx = 6;
      return 0;
    }
  }
  if (f_idx) {
    tag[f_idx++] = ch;
    if (tag[f_idx-2]=='\r' && tag[f_idx-1]=='\n') {
      tag[f_idx] = '\0';
      char *p = tag, *pt;
      p = next(p, &pt);                                         //got $GPGGA
      p = next(p, &pt);                                         //utc time
      p = next(p, &pt);                                         //lat  ddmm.mmmm
      sprintf(_lat, pt);
      p = next(p, &pt);                                         //N/S
      if (*pt=='S') {
         char buf[16];
         sprintf(buf, "-%s", _lat);
         sprintf(_lat, buf);
      }
      p = next(p, &pt);                                         //lng  dddmm.mmmm
      sprintf(_lng, pt);
      p = next(p, &pt);                                         //E/W
      if (*pt == 'W') {
         char buf[16];
         sprintf(buf, "-%s", _lng);
         sprintf(_lng, buf);
      }
      p = next(p, &pt);                                         //st
      _gps_status = *pt;      
      f_idx = 0;
    }
  }
  return 0;
}

