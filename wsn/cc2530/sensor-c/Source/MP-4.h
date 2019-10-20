/*********************************************************************************************
* �ļ���Infrared.h
* ���ߣ�Lixm 2017.10.17
* ˵������ȼ���崫������������ͷ�ļ�  
* �޸ģ�fuyou 2018.08.03 ���Ӷ�1.4�汾Ӳ�������ֶ���
* ע�ͣ�
*********************************************************************************************/

/*********************************************************************************************
* ����������
*********************************************************************************************/
#ifndef __MP_4_H__
#define __MP_4_H__

/*********************************************************************************************
* ͷ�ļ�
*********************************************************************************************/
#include <ioCC2530.h>

#ifndef HW_VERSION_1_2
#ifndef HW_VERSION_1_4
#define HW_VERSION_1_4
#warning Hardware version not specified,Default HW_VERSION_1_4
#endif
#endif

#ifdef HW_VERSION_1_2
#ifdef HW_VERSION_1_4
#error Hardware version definition conflict
#endif
#endif

#ifdef HW_VERSION_1_2
#define GAS_THRESHOLD 1600
#endif

#ifdef HW_VERSION_1_4
#define GAS_THRESHOLD 900
#endif

/*********************************************************************************************
* �ڲ�ԭ�ͺ���
*********************************************************************************************/
void combustiblegas_init(void);                          		//��ȼ���崫������ʼ��
unsigned int get_combustiblegas_data(void);            			//��ȡ��ȼ���崫����״̬

#endif /*__MP_4_H__*/
