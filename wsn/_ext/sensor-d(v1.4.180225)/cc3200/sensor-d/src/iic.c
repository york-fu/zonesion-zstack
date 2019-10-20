/*********************************************************************************************
* 文件：iic.c
* 作者：zonesion
* 说明：iic驱动程序
* 修改：
* 注释：
*********************************************************************************************/

/*********************************************************************************************
* 头文件
*********************************************************************************************/
#include "iic.h"
/*********************************************************************************************
* 宏定义
*********************************************************************************************/
#define G13_UCPINS              ( 1 << (13%8))
#define G3_UCPINS               ( 1 << (3%8))
#define SDA_OUT                 GPIODirModeSet(GPIOA0_BASE, G3_UCPINS, GPIO_DIR_MODE_OUT);   //设置GPIO3为输出模式
#define SDA_IN                  GPIODirModeSet(GPIOA0_BASE, G3_UCPINS, GPIO_DIR_MODE_IN);   //设置GPIO3为输出模式

/*********************************************************************************************
* 名称：iic_delay_us()
* 功能：延时函数
* 参数：i -- 延时设置
* 返回：无
* 修改：
* 注释：
*********************************************************************************************/
#pragma optimize=none
static void  iic_delay_us(unsigned int i)
{
  while(i--){
    
  }
}

/*********************************************************************************************
* 名称：iic_init()
* 功能：I2C初始化函数
* 参数：无
* 返回：无
* 修改：
* 注释：
*********************************************************************************************/
void iic_init(void)
{
  PRCMPeripheralClkEnable(PRCM_GPIOA0, PRCM_RUN_MODE_CLK);      //使能时钟
  PinTypeGPIO(PIN_58,PIN_MODE_0,false);                           //选择引脚为GPIO模式（gpio4）
  PinConfigSet(PIN_58,PIN_STRENGTH_2MA,PIN_TYPE_STD_PU);
  GPIODirModeSet(GPIOA0_BASE, G3_UCPINS, GPIO_DIR_MODE_OUT);   //设置GPIO17为输出模式
  
  PRCMPeripheralClkEnable(PRCM_GPIOA1, PRCM_RUN_MODE_CLK);      //使能时钟
  PinTypeGPIO(PIN_04,PIN_MODE_0,false);                           //选择引脚为GPIO模式（gpio5）
  PinConfigSet(PIN_04,PIN_STRENGTH_2MA,PIN_TYPE_STD_PU);
  GPIODirModeSet(GPIOA1_BASE, G13_UCPINS, GPIO_DIR_MODE_OUT);   //设置GPIO3为输出模式
  
  GPIOPinWrite(GPIOA0_BASE, G3_UCPINS, 0xFF);                                                      //拉高数据线
  iic_delay_us(5);                                             //延时10us
  GPIOPinWrite(GPIOA1_BASE, G13_UCPINS, 0XFF);                                                      //拉高时钟线
  iic_delay_us(5);                                             //延时10us
}

/*********************************************************************************************
* 名称：iic_start()
* 功能：I2C起始信号
* 参数：无
* 返回：无
* 修改：
* 注释：
*********************************************************************************************/
void iic_start(void)
{
  GPIOPinWrite(GPIOA0_BASE, G3_UCPINS, 0xFF);                                                      //拉高数据线
  iic_delay_us(1);
  GPIOPinWrite(GPIOA1_BASE, G13_UCPINS, 0XFF);                                                      //拉高时钟线
  iic_delay_us(1);                                              //延时
  GPIOPinWrite(GPIOA0_BASE, G3_UCPINS, 0);                                                      //产生下降沿
  iic_delay_us(1);                                              //延时
  GPIOPinWrite(GPIOA1_BASE, G13_UCPINS, 0);                                                      //拉低时钟线
}

/*********************************************************************************************
* 名称：iic_stop()
* 功能：I2C停止信号
* 参数：无
* 返回：无
* 修改：
* 注释：
*********************************************************************************************/
void iic_stop(void)
{
  GPIOPinWrite(GPIOA0_BASE, G3_UCPINS, 0);                                                       //拉低数据线
  iic_delay_us(1);  
  GPIOPinWrite(GPIOA1_BASE, G13_UCPINS, 0XFF);                                                       //拉高时钟线
  iic_delay_us(1);                                              //延时5us
  GPIOPinWrite(GPIOA0_BASE, G3_UCPINS, 0xFF);                                                        //产生上升沿
                                         
}

/*********************************************************************************************
* 名称：iic_write_byte()
* 功能：I2C写一个字节数据，返回ACK或者NACK，从高到低，依次发送
* 参数：data -- 要写的数据
* 返回：无
* 修改：
* 注释：
*********************************************************************************************/
unsigned char iic_write_byte(unsigned char data)
{
  unsigned char i;
  
  for(i = 0;i < 8;i++){                                         
    if(data & 0x80){                                            //判断数据最高位是否为1
      GPIOPinWrite(GPIOA0_BASE, G3_UCPINS, 0xFF);	                                                
    }
    else
      GPIOPinWrite(GPIOA0_BASE, G3_UCPINS, 0);
    iic_delay_us(1);                                            //延时5us
    GPIOPinWrite(GPIOA1_BASE, G13_UCPINS, 0XFF);	        //输出SDA稳定后，拉高SCL给出上升沿，从机检测到后进行数据采样
    iic_delay_us(1);                                            //延时5us
    GPIOPinWrite(GPIOA1_BASE, G13_UCPINS, 0);                                                
    data <<= 1;                                                 //数组左移一位
  } 

  SDA_IN;
  iic_delay_us(1);
  GPIOPinWrite(GPIOA1_BASE, G13_UCPINS, 0XFF);                  //拉高时钟线
                     
  if(GPIOPinRead(GPIOA0_BASE, G3_UCPINS) != 0){			//SDA为高，收到NACK
    iic_delay_us(1); 
     GPIOPinWrite(GPIOA1_BASE, G13_UCPINS, 0);
     SDA_OUT;
     return 1;	
  }else{ 				                        //SDA为低，收到ACK
    iic_delay_us(1);
    GPIOPinWrite(GPIOA1_BASE, G13_UCPINS, 0);
    SDA_OUT;
    return 0;
  }
}

/*********************************************************************************************
* 名称：iic_read_byte()
* 功能：I2C写一个字节数据，返回ACK或者NACK，从高到低，依次发送
* 参数：data -- 要写的数据
* 返回：无
* 修改：
* 注释：
*********************************************************************************************/
unsigned char iic_read_byte(unsigned char ack)
{
  unsigned char i,data = 0;
  GPIOPinWrite(GPIOA1_BASE, G13_UCPINS, 0);
  SDA_IN;
  for(i = 0;i < 8;i++){
    iic_delay_us(1);
    GPIOPinWrite(GPIOA1_BASE, G13_UCPINS, 0XFF);		                 //给出上升沿
    data <<= 1;	
    if(GPIOPinRead(GPIOA0_BASE, G3_UCPINS) != 0){ 		                 //采样获取数据
      data |= 0x01;
    }else{
      data &= 0xfe;
    }
    iic_delay_us(1);
    GPIOPinWrite(GPIOA1_BASE, G13_UCPINS, 0);   		                 //下降沿，从机给出下一位值
  }
  SDA_OUT;
  GPIOPinWrite(GPIOA0_BASE, G3_UCPINS,  (ack!=0)?0xff:0x00);	                                 //应答状态
  iic_delay_us(1);
  GPIOPinWrite(GPIOA1_BASE, G13_UCPINS, 0XFF);                         
  iic_delay_us(1);          
  GPIOPinWrite(GPIOA1_BASE, G13_UCPINS, 0);
  return data;
}
int iic_read_buf(char addr, char r, char *buf, int len)
{
    int i;
    
    iic_start();
    if (iic_write_byte(addr<<1)) {
      iic_stop();
      return -1;
    }
    if (iic_write_byte(r)) return -1;
   
    iic_start();
    if (iic_write_byte((addr<<1)|1)){
      iic_stop();
      return -1;
    }
   
    for (i=0; i<len-1; i++) {
      buf[i] = iic_read_byte(0);
    }
    buf[i++] = iic_read_byte(1);
    iic_stop();
    return i;
}
int iic_write_buf(char addr, char r, char *buf, int len)
{
  int i;
  
  iic_start();
  if (iic_write_byte(addr<<1)) {
      iic_stop();
      return -1;
    }

  if (iic_write_byte(r)) {
      iic_stop();
      return -1;
    }
 
  for (i=0; i<len; i++) {
    if (iic_write_byte(buf[i])) {
      iic_stop();
      return -1;
    }
  }
  iic_stop();
  return i;
}