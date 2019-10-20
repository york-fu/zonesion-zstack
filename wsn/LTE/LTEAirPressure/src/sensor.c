/*********************************************************************************************
* �ļ���sensor.c
* ���ߣ�Xuzhy 2018.5.16
* ˵�����ڵ㹤������ģ��
*       �ǻ۽�ͨϵͳ���������豸���ɼ��ࣨ����ѹ���ɼ����������ࣨɽ�廬���񶯱������������ࣨRGB��ͨ���̵ƣ�
*       ���ݰ���ʽ��{airPressure=101.0,shockStatus=0,rgbStatus=0}
*       airPressure��ʾ����ѹ��ֵ��shockStatusΪ��״̬��0/1����rgbStatusΪrgb�Ƶ�״̬��R/G/Y��
*       ��ѯ�����ѯ��ֵ�ο��������ѯ��ǰ����ѹ��ֵ��{airPressure=?}��
*       �������cmd=0Ϊrgb����cmd=1Ϊ��ƣ�cmd=2Ϊ�̵ƣ�cmd=3Ϊ�Ƶ�
* �޸ģ�
* ע�ͣ�
*********************************************************************************************/

/*********************************************************************************************
* ͷ�ļ�
*********************************************************************************************/
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "contiki.h"
#include "stm32f10x.h"
#include "delay.h"
#include "fbm320.h"
#include "iic.h"
#include "math.h"
#include "zxbee.h"
#include "zxbee-inf.h"
#include "sensor.h"

PROCESS(sensor, "sensor");

/*********************************************************************************************
* ȫ�ֱ���
*********************************************************************************************/
static float airPressure = 0.0;                              // ����ѹ��ֵ
/*********************************************************************************************
* ���ƣ�updateAirPressure()
* ���ܣ����´���ѹ��ֵ
* ������
* ���أ�
* �޸ģ�
* ע�ͣ�
*********************************************************************************************/
void updateAirPressure(void)
{
  // ��ȡ����ѹ��ֵ
  float temperature = 0; 
  long pressure = 0;  
  
  // ��ȡ����ѹ��ֵ��������A4
  fbm320_data_get(&temperature,&pressure);                      // ��ȡ�¶ȡ�ѹ������
  airPressure = pressure/100.0f;                                // ת��ѹ����λΪ����
}
/*********************************************************************************************
* ���ƣ�sensorInit()
* ���ܣ�������Ӳ����ʼ��
* ��������
* ���أ���
* �޸ģ�
* ע�ͣ�
*********************************************************************************************/
void sensorInit(void)
{
  // ��ʼ������������
  fbm320_init();                                                // ����ѹ����������ʼ��
}
/*********************************************************************************************
* ���ƣ�sensorLinkOn()
* ���ܣ��������ڵ������ɹ����ú���
* ��������
* ���أ���
* �޸ģ�
* ע�ͣ�
*********************************************************************************************/
void sensorLinkOn(void)
{
  sensorUpdate();
}
/*********************************************************************************************
* ���ƣ�sensorUpdate()
* ���ܣ����������ϱ�������
* ��������
* ���أ���
* �޸ģ�
* ע�ͣ�
*********************************************************************************************/
void sensorUpdate(void)
{ 
  char pData[16];
  char *p = pData;
  
  // ����ѹ���ɼ�
  updateAirPressure();
  
  ZXBeeBegin();                                                 // ��������֡��ʽ��ͷ

  // �ϱ�����ѹ��ֵ
  sprintf(p, "%.1f", airPressure); 
  ZXBeeAdd("airPressure", p);

  p = ZXBeeEnd();                                               // ��������֡��ʽ��β
  if (p != NULL) {												
    ZXBeeInfSend(p, strlen(p));	                                // ����Ҫ�ϴ������ݷ��͵�����ƽ̨
  }
}
/*********************************************************************************************
* ���ƣ�ZXBeeUserProcess()
* ���ܣ������յ��Ŀ�������
* ������*ptag -- ������������
*       *pval -- �����������
* ���أ�<0:��֧��ָ�>=0 ָ���Ѵ���
* �޸ģ�
* ע�ͣ�
*********************************************************************************************/
int ZXBeeUserProcess(char *ptag, char *pval)
{ 
  int ret = 0;	
  char pData[16];
  char *p = pData;

  // �����������
  if (0 == strcmp("airPressure", ptag)){                                 // ��ѯִ�����������
    if (0 == strcmp("?", pval)){
      updateAirPressure();
      ret = sprintf(p, "%.1f", airPressure);
      ZXBeeAdd("airPressure", p);
    } 
  }
  
  return ret;
}
/*********************************************************************************************
* ���ƣ�sensor()
* ���ܣ��������ɼ��߳�
* ������
* ���أ�
* �޸ģ�
* ע�ͣ�
*********************************************************************************************/
PROCESS_THREAD(sensor, ev, data)
{
  static struct etimer et_update;
  //static struct etimer et_check;
  
  PROCESS_BEGIN();
  
  ZXBeeInfInit();
  
  sensorInit();
  
  etimer_set(&et_update, CLOCK_SECOND*20); 
  
  while (1) {
    PROCESS_WAIT_EVENT_UNTIL(ev == PROCESS_EVENT_TIMER); 
    if (etimer_expired(&et_update)) {
      sensorUpdate();
      etimer_set(&et_update, CLOCK_SECOND*20); 
    } 
  }
  PROCESS_END();
}