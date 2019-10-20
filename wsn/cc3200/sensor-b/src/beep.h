/*********************************************************************************************
* �ļ���
* ���ߣ�
* ˵����
* �޸ģ�
* ע�ͣ�
*********************************************************************************************/

/*********************************************************************************************
* ����������
*********************************************************************************************/
#ifndef __BEEP_H__
#define __BEEP_H__

/*********************************************************************************************
* ͷ�ļ�
*********************************************************************************************/
#include "gpio.h"
#include "hw_memmap.h"
#include "systick_if.h"
#include "pin.h"
#include "prcm.h"
#include <stdio.h>
#include "rom_map.h"

/*********************************************************************************************
* �궨��
*********************************************************************************************/
#define G10_UCPINS  ( 1 << (10%8))
/*********************************************************************************************
* ��������
*********************************************************************************************/
extern void Beep_init(void);                                            
signed char Beep_on(unsigned char fan);                        
signed char Beep_off(unsigned char fan);                        

#endif /*__FAN_H_*/
