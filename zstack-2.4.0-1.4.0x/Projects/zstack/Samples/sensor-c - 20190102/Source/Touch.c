/*********************************************************************************************
* �ļ���touch.c
* ���ߣ�Lixm 2017.10.17
* ˵����������������
* �޸ģ�
* ע�ͣ�
*********************************************************************************************/

/*********************************************************************************************
* ͷ�ļ�
*********************************************************************************************/
#include "touch.h"

/*********************************************************************************************
* ���ƣ�touch_init()
* ���ܣ�������������ʼ��
* ��������
* ���أ���
* �޸ģ�
* ע�ͣ�
*********************************************************************************************/
void touch_init(void)
{
  P0SEL &= ~0x01;                                               //���ùܽ�Ϊͨ��IOģʽ
  P0DIR &= ~0x01;                                               //���ÿ��ƹܽ�Ϊ����ģʽ
}

/*********************************************************************************************
* ���ƣ�unsigned char get_touch_status(void)
* ���ܣ���ȡ����������״̬
* ��������
* ���أ���
* �޸ģ�
* ע�ͣ�
*********************************************************************************************/
unsigned char get_touch_status(void)
{
  if(P0_0){                                                      //���io�ڵ�ƽ
    return 0;
  }
  else{
    return 1;
  }
}