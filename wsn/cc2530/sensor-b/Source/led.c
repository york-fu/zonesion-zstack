/*********************************************************************************************
* �ļ���led.c
* ���ߣ�Meixin 2017.09.15
* ˵����LED����������     
* �޸ģ�liutong 20171027 �޸���LED�������ų�ʼ���������ơ��Ż���ע�͡�������LED�򿪺͹رպ���
* ע�ͣ�
*********************************************************************************************/

/*********************************************************************************************
* ͷ�ļ�
*********************************************************************************************/
#include "led.h"

/*********************************************************************************************
* ���ƣ�led_init()
* ���ܣ�LED�������ų�ʼ��
* ��������
* ���أ���
* �޸ģ�
* ע�ͣ�
*********************************************************************************************/
void led_init(void)
{
  P0SEL &= ~0x30;                                               //���ÿ��ƹܽţ�p0.4, p0.5��Ϊͨ��IOģʽ
  P0DIR |= 0x30;                                                //���ÿ��ƹܽţ�p0.4, p0.5��Ϊ���ģʽ
  
  LED1 = OFF;                                                     //��ʼ״̬Ϊ�ر�
  LED2 = OFF;                                                     //��ʼ״̬Ϊ�ر�
}
  
/*********************************************************************************************
* ���ƣ�led_on()
* ���ܣ�LED���ƴ򿪺���
* ������LED�ţ���led.h�к궨��ΪLED1��LED2
* ���أ�0����LED�ɹ���-1����������
* �޸ģ�
* ע�ͣ�����ֻ������LED1��LED2,����᷵��-1
*********************************************************************************************/
signed char led_on(unsigned char led)
{
  if(led == LED1){                                                //���Ҫ��LED1
    LED1 = ON;
    return 0;
  }
    
  if(led == LED2){                                                //���Ҫ��LED2
    LED2 = ON;
    return 0;
  }
  
  return -1;                                                    //�������󣬷���-1
}

/*********************************************************************************************
* ���ƣ�led_off()
* ���ܣ�LED���ƹرպ���
* ������LED�ţ���led.h�к궨��ΪLED1��LED2
* ���أ�0���ر�LED�ɹ���-1����������
* �޸ģ�
* ע�ͣ�����ֻ������LED1��LED2,����᷵��-1
*********************************************************************************************/
signed char led_off(unsigned char led)
{
  if(led == LED1){                                                //���Ҫ�ر�LED1
    LED1 = OFF;
    return 0;
  }
    
  if(led == LED2){                                                //���Ҫ�ر�LED2
    LED2 = OFF;
    return 0;
  }
  
  return -1;                                                    //�������󣬷���-1
}