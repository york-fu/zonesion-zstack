/*********************************************************************************************
* �ļ���sensor.c
* ���ߣ�Xuzhy 2018.5.16
* ˵�����ڵ㹤������ģ��
*       �����Ʒ���������豸���ɼ��ࣨʪ�Ȳɼ����������ࣨ�Ŵű���״̬���������ࣨLED��
*       ���ݰ���ʽ��{doorStatus=124.0,doorStatus=0,ledStatus=0}
*       doorStatus��ʾʪ��ֵ��doorStatusΪ�Ŵ�״̬��0/1����ledStatusΪLED��״̬��ON/OFF��
*       ��ѯ�����ѯ��ֵ�ο��������ѯ��ǰʪ�ȣ�{doorStatus=?}��
*       �������cmd=0Ϊ�ر�LED��cmd=1Ϊ��LED
* �޸ģ�
* ע�ͣ�
*********************************************************************************************/

/*********************************************************************************************
* ͷ�ļ�
*********************************************************************************************/
#include "hal_types.h"
#include "ioCC2541.h"
#include <osal.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "hal_uart.h"
#include "led.h"
#include "zxbee.h"
#include "sensor.h"
#include "zxbee-inf.h"
/*********************************************************************************************
* �궨��
*********************************************************************************************/

/*********************************************************************************************
* ȫ�ֱ���
*********************************************************************************************/
static uint8 ledStatus = 0;                                   // LED״ֵ̬
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
  // ��ʼ��LED����
  led_init();                                                   // LED�Ƴ�ʼ��
  
  // ������ʱ���������������ϱ������¼���MY_REPORT_EVT
  osal_start_timerEx(simpleBLEPeripheral_TaskID, MY_REPORT_EVT, (uint16)((osal_rand()%10) * 1000));
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
  
  ZXBeeBegin();                                                 // ��������֡��ʽ��ͷ

  // ����LED״ֵ̬
  sprintf(p, "%u", ledStatus); 
  ZXBeeAdd("ledStatus", p); 
  
  p = ZXBeeEnd();                                               // ��������֡��ʽ��β
  if (p != NULL) {												
    ZXBeeInfSend(p, strlen(p));	                                // ����Ҫ�ϴ������ݽ��д����������ͨ��zb_SendDataRequest()���͵�Э����
  }
}
/*********************************************************************************************
* ���ƣ�sensorControl()
* ���ܣ�����������
* ������cmd - ��������
* ���أ���
* �޸ģ�
* ע�ͣ�
*********************************************************************************************/
void sensorControl(uint8 cmd)
{
  // ����cmd����������Ӧ�Ŀ��Ƴ���
  if(cmd & 0x02){                                               // LED2�ƿ���λ��bit1
    LED2 = ON;                                                  // ����LED2
  }
  else{
    LED2 = OFF;                                                 // �ر�LED2        
  }
  if(cmd & 0x01){                                               // LED1�ƿ���λ��bit0
    LED1 = ON;                                                  // ����LED1
  }
  else{
    LED1 = OFF;                                                 // �ر�LED1
  }
  ledStatus = cmd;
}
/*********************************************************************************************
* ���ƣ�ZXBeeUserProcess()
* ���ܣ������յ��Ŀ�������
* ������*ptag -- ������������
*       *pval -- �����������
* ���أ�ret -- p�ַ�������
* �޸ģ�
* ע�ͣ�
*********************************************************************************************/
int ZXBeeUserProcess(char *ptag, char *pval)
{ 
  int val;
  int ret = 0;	
  char pData[16];
  char *p = pData;
  
  // ���ַ�������pval����ת��Ϊ���ͱ�����ֵ
  val = atoi(pval);	
  // �����������
  if (0 == strcmp("cmd", ptag)){                                // LED�Ŀ�������
    sensorControl(val);
  }
  if (0 == strcmp("ledStatus", ptag)){                        // ��ѯִ�����������
    if (0 == strcmp("?", pval)){
      ret = sprintf(p, "%u", ledStatus);
      ZXBeeAdd("ledStatus", p);
    } 
  }
  
  return ret;
}
/*********************************************************************************************
* ���ƣ�MyEventProcess()
* ���ܣ��Զ����¼�����
* ������event -- �¼����
* ���أ���
* �޸ģ�
* ע�ͣ�
*********************************************************************************************/
void MyEventProcess( uint16 event )
{
  if (event & MY_REPORT_EVT) { 
    sensorUpdate();                                             // ���������ݶ�ʱ�ϱ�
    //������ʱ���������¼���MY_REPORT_EVT 
    osal_start_timerEx(simpleBLEPeripheral_TaskID, MY_REPORT_EVT, 20*1000);
  } 
}