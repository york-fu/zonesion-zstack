/*********************************************************************************************
* 文件：ExtInt.c
* 作者：fuyou
* 说明：cc2530外部中断驱动 
* 修改：
* 注释：
*********************************************************************************************/

/*********************************************************************************************
* 头文件
*********************************************************************************************/
#include "ExtInt.h"

/*********************************************************************************************
* 名称：extInt_init
* 功能：外部中断初始化
* 参数：port：端口选择，0-2对应P0-P2。
*       pin：io口选择，0-7对应Px_0-Px_7
*       mode：中断触发方式，0-->上升沿触发，1-->下降沿触发
* 返回：无
* 修改：
* 注释：port为2时，pin范围0-4
*********************************************************************************************/
void ExtInt_init(Ext_Px port,Ext_Pinx pin,Ext_mode mode)
{
    unsigned char port_pictl=0;
  
    EA = 0;                                                     //总中断失能
    
    switch(port)
    {
    case Ext_P0:
        P0SEL &= ~(1<<pin);                                     //通用IO    
        P0DIR &= ~(1<<pin);                                     //作输入  
        P0INP &= ~(1<<pin);                                     //上下拉模式
        P0IEN |= (1<<pin);                                      //使能pin口中断
        P0IE = 1;                                               //使能P0口中断
        break;
    case Ext_P1:
        P1SEL &= ~(1<<pin);                                     //通用IO    
        P1DIR &= ~(1<<pin);                                     //作输入  
        P1INP &= ~(1<<pin);                                     //上下拉模式       
        P1IEN |= (1<<pin);                                      //使能pin口中断
        IEN2 |= (1<<4);                                         //使能P1口中断
        if(pin>3) port_pictl=port+1;                            //P2占用了PICTL两个位
        break;
    case Ext_P2:
        if(pin>4) return;
        P2SEL &= ~(1<<pin);                                     //通用IO    
        P2DIR &= ~(1<<pin);                                     //作输入  
        P2INP &= ~(1<<pin);                                     //上下拉模式
        P2IEN |= (1<<pin);                                      //使能pin口中断
        IEN2 |= (1<<1);                                         //使能P2口中断
        port_pictl=port+1;
        break;
    }
    
    if(mode)    //上下拉和触发模式设置
    {
        P2INP &= ~(1<<(5+port));                                //Px上拉
        PICTL |= (1<<port_pictl);                               //下降沿触发
    }
    else
    {
        P2INP |= (1<<(5+port));                                 //Px下拉
        PICTL &= ~(1<<port_pictl);                              //上升沿触发
    }
    
    EA = 1;                                                     //总中断使能   
}


/*********************************************************************************************
* 名称：P0_ISR
* 功能：P0外部中断服务函数
* 参数：无
* 返回：无
* 修改：
* 注释：
*********************************************************************************************/
//
//#pragma vector = P0INT_VECTOR
//__interrupt void P0_ISR(void)
//{ 
//    if(P0IFG&(1<<0))
//    {
//    }
//    if(P0IFG&(1<<4))
//    {
//    }
//    P0IF = 0;
//}