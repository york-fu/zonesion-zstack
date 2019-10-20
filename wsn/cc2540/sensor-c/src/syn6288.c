/*********************************************************************************************
* �ļ���syn6288.c
* ���ߣ�zonesion
* ˵����syn6288��������
* �޸ģ�Chenkm 2017.01.04 ������ע��
* ע�ͣ�
*********************************************************************************************/
/*********************************************************************************************
* ͷ�ļ�
*********************************************************************************************/
#include "syn6288.h"
#include "string.h"

/*********************************************************************************************
* ���ƣ�uart0_Init(unsigned char StopBits,unsigned char Parity)
* ���ܣ�����0��ʼ��
* ��������
* ���أ���
* �޸ģ�
* ע�ͣ�
****************************************** 
*    CC253O 32Mϵͳʱ�Ӳ����ʲ�����      * 
*----------------------------------------* 
*  ������  UxBAUD         UxGCRM         * 
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
void uart0_Init(unsigned char StopBits,unsigned char Parity)
{
  P0SEL |=  0x0C;                                               //��ʼ��UART0�˿�
  PERCFG&= ~0x01;                                               //ѡ��UART0Ϊ��ѡλ��һ
  P2DIR &= ~0xC0;                                               //P0������Ϊ����0
  U0CSR = 0xC0;                                                 //����ΪUARTģʽ,����ʹ�ܽ�����
   
  U0GCR = 0x8;                  
  U0BAUD = 59;                                                  //����������Ϊ9600
  
  //U0UCR |= 2|StopBits|Parity;                                     //����ֹͣλ����żУ��
  U0UCR = 2;
}

/*********************************************************************************************
* ���ƣ�uart_send_char()
* ���ܣ����ڷ����ֽں���
* ��������
* ���أ���
* �޸ģ�
* ע�ͣ�
*********************************************************************************************/
void uart_send_char(unsigned char ch)
{
  //while(!(U0CSR & 0x02));
  
  U0DBUF = ch;                                                  //��Ҫ���͵��������뷢�ͻ���Ĵ���
  while(UTX0IF == 0);                                           //�ȴ����ݷ������
  UTX0IF = 0;                                                   //������ɺ���������
  //while(U0CSR & 0x02);

}

/*********************************************************************************************
* ���ƣ�syn6288_init()
* ���ܣ�syn6288��ʼ��
* ������
* ���أ�
* �޸ģ�
* ע�ͣ�
*********************************************************************************************/
void syn6288_init()
{
  uart0_Init(0,0); 
  P0SEL &= ~0x02;                                               //���ùܽ�Ϊͨ��IOģʽ
  P0DIR &= ~0x02;                                               //���ÿ��ƹܽ�Ϊ����ģʽ
}

int syn6288_busy(void)
{ 
  if(P0_1)                                                      //æ�������
    return 0;                                                   //û�м�⵽�źŷ��� 0
  else
    return 1;                                                   //��⵽�źŷ��� 1
}
/*********************************************************************************************
* ���ƣ�syn6288_play()
* ���ܣ�
* ������s -- ������
* ���أ�
* �޸ģ�
* ע�ͣ�
*********************************************************************************************/
void syn6288_play(char *s)
{
  int i;
  int len = strlen(s);
  unsigned char c = 0;  
  unsigned char head[] = {0xFD,0x00,0x00,0x01,0x00};            //���ݰ�ͷ  
  head[1] = (len+3)>>8;
  head[2] = (len+3)&0xff;
  for (i=0; i<5; i++){
    uart_send_char(head[i]);
    c ^= head[i];
  }
  for (i=0; i<len; i++){
    uart_send_char(s[i]);
    c ^= s[i];
  }
  uart_send_char(0xA5);
}

/*******************************************************************************
* ����: hex2unicode()
* ����: ��16�����ַ���ת����bin��ʽ
* ����: 
* ����: 
* �޸�:
* ע��: 
*******************************************************************************/
char *hex2unicode(char *str)
{       
  static char uni[64];
  int n = strlen(str)/2;
  if (n > 64) n = 64;
  
  for (int i=0; i<n; i++) {
    unsigned int x = 0;
    for (int j=0; j<2; j++) {
      char c = str[2*i+j];
      char o;
      if (c>='0' && c<='9') o = c - '0';
      else if (c>='A' && c<='F') o = 10+(c-'A');
      else if (c>='a' && c<='f') o = 10+(c-'a');
      else o = 0;
      x = (x<<4) | (o&0x0f);
    }
    uni[i] = x;
  }
  uni[n] = 0;
  return uni;
}


/*******************************************************************************
* syn6288_play_unicode()
* ���ܣ�
* ������s -- ������
* ���أ�
* �޸ģ�
* ע�ͣ�
*******************************************************************************/
void syn6288_play_unicode(char *s, int len)
{
  int i;
  char c = 0;  
  unsigned char head[] = {0xFD,0x00,0x00,0x01,0x03};            //���ݰ�ͷ  
  head[1] = (len+3)>>8;
  head[2] = (len+3)&0xff;
  for (i=0; i<5; i++){
    uart_send_char(head[i]);
    c ^= head[i];
  }
  for (i=0; i<len; i++){
    uart_send_char(s[i]);
    c ^= s[i];
  }
  uart_send_char(c);
}