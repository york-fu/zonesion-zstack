#include <iocc2530.h>

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
void uart0Init()
{
  P0SEL |=  0x0C;                                               //初始化UART0端口
  PERCFG&= ~0x01;                                               //选择UART0为可选位置一
  P2DIR &= ~0xC0;                                               //P0优先作为串口0
  U0CSR = 0xC0;                                                 //设置为UART模式,而且使能接受器
  
  U0GCR = 11;                  
  U0BAUD = 216;                                                 //波特率设置为115200
  
  U0UCR = 2;
  URX0IE = 1;                                                   // 使能接收中断
}
/*********************************************************************************************
* 名称：uart_send_char()
* 功能：串口发送字节函数
* 参数：无
* 返回：无
* 修改：
* 注释：
*********************************************************************************************/
void uart0Send(unsigned char ch)
{
  U0DBUF = ch;                                                  //将要发送的数据填入发送缓存寄存器
  while((U0CSR&0x02) == 0);                                     
  U0CSR &= ~0x02;                                             //发送完成后将数据清零
}
static void (*_input_call)(char ch);
void uart0SetInputCall(void (*f)(char ch))
{
  _input_call = f;
}
#pragma vector=URX0_VECTOR  
__interrupt void UART0_ISR(void)  
{  
  URX0IF = 0;                                         // 清除接收中断标志  
  char ch = U0DBUF;                                    // 填充缓冲区 
  if (_input_call){
    _input_call(ch);
  }
}