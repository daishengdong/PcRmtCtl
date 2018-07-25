#include "24l01.h"
//#include "stm32f10x_it.h"
#include "../FWLib/inc/stm32f10x_spi.h"
#include "../FWLib/inc/stm32f10x_gpio.h"
#include "../FWLib/inc/stm32f10x_rcc.h"
#include "tty.h"
//#include "Delay.h"
/****************************
*file:24L01.c
*author:Zachary(F.SB)
*NRF24L01 deriver based on Lib
*time:2013/8/21
*****************************/

const u8 TX_ADDRESS[TX_ADR_WIDTH]={0x34,0x43,0x10,0x10,0x01}; //发送地址
const u8 RX_ADDRESS[RX_ADR_WIDTH]={0x34,0x43,0x10,0x10,0x01}; //接受地址

u8 SPI_NRF_RW(u8 dat){
	while(SPI_I2S_GetFlagStatus(SPI2,SPI_I2S_FLAG_TXE)==RESET);
	SPI_I2S_SendData(SPI2,dat);
	while(SPI_I2S_GetFlagStatus(SPI2,SPI_I2S_FLAG_RXNE)==RESET);
	return SPI_I2S_ReceiveData(SPI2);
}

//SPI写寄存器
//reg:指定寄存器地址
//value:写入的值
u8 NRF24L01_Write_Reg(u8 reg,u8 value)
{
	u8 status;	
   	NRF24L01_CSN_0;                 //使能SPI传输
  	status =SPI_NRF_RW(reg);//发送寄存器号 
  	SPI_NRF_RW(value);      //写入寄存器的值
  	NRF24L01_CSN_1;                 //禁止SPI传输	   
  	return(status);       			//返回状态值
}
//读取SPI寄存器值
//reg:要读的寄存器
u8 NRF24L01_Read_Reg(u8 reg)
{
	u8 reg_val;	    
	NRF24L01_CSN_0;          //使能SPI传输
  	SPI_NRF_RW(reg);   //发送寄存器号
  	reg_val=SPI_NRF_RW(0XFF);//读取寄存器内容
  	NRF24L01_CSN_1;          //禁止SPI传输		    
  	return(reg_val);           //返回状态值
}


//初始化24L01的IO口
void NRF24L01_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	SPI_InitTypeDef SPI_InitStructure;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB | RCC_APB2Periph_AFIO,ENABLE);//AFIO是用于SPI复用必须开启
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_SPI2,ENABLE);
	
	/* Configure SPI2 pins: SCK13, MISO14 and MOSI15 */
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_Init(GPIOB, &GPIO_InitStructure);
	/*****************/
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11 ;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_Init(GPIOB, &GPIO_InitStructure);
	//CSN
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12;
   GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
   GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
   GPIO_Init(GPIOB, &GPIO_InitStructure);
	/*****************/	//| GPIO_Pin_0;
   GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
   GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
   GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
   GPIO_Init(GPIOB, &GPIO_InitStructure);
	/*****************/
   NRF24L01_CSN_1;
	/* SPI2 Config*/
   SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;
   SPI_InitStructure.SPI_Mode = SPI_Mode_Master;
   SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;
   SPI_InitStructure.SPI_CPOL = SPI_CPOL_Low;//this is so important
   SPI_InitStructure.SPI_CPHA = SPI_CPHA_1Edge;//important
   SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;
   SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_8;
   SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;
   SPI_InitStructure.SPI_CRCPolynomial = 7;
   SPI_Init(SPI2, &SPI_InitStructure);
   /* Enable SPI2 */
   SPI_Cmd(SPI2, ENABLE);
}


//检测24L01是否存在
//返回值:0，成功;1，失败	
u8 NRF24L01_Check(void)
{
	u8 buf[5]={0XA5,0XA5,0XA5,0XA5,0XA5};
	u8 buf1[5];
	u8 i; 
	//usart_send("check in\r\n");
	NRF24L01_Write_Buf(WRITE_REG_G+TX_ADDR,buf,5);//写入5个字节的地址.	
	NRF24L01_Read_Buf(TX_ADDR,buf1,5); //读出写入的地址  
	for(i=0;i<5;i++)if(buf1[i]!=0XA5)break;
	//usart_send("check out\r\n");
	if(i!=5)return 0;//检测24L01错误	
	return 1;		 //检测到24L01
}

//在指定位置读出指定长度的数据
//reg:寄存器(位置)
//*pBuf:数据指针
//len:数据长度
//返回值,此次读到的状态寄存器值 
u8 NRF24L01_Read_Buf(u8 reg,u8 *pBuf,u8 len)
{
	u8 status,u8_ctr;	       
  	NRF24L01_CSN_0;           //使能SPI传输
	status=SPI_NRF_RW(reg);//发送寄存器值(位置),并读取状态值 
	for(u8_ctr=0;u8_ctr<len;u8_ctr++)
	pBuf[u8_ctr]=SPI_NRF_RW(0XFF);//读出数据
  	NRF24L01_CSN_1;       //关闭SPI传输
  	return status;        //返回读到的状态值
}
//在指定位置写指定长度的数据
//reg:寄存器(位置)
//*pBuf:数据指针
//len:数据长度
//返回值,此次读到的状态寄存器值
u8 NRF24L01_Write_Buf(u8 reg, u8 *pBuf, u8 len)
{
	
	u8 status,u8_ctr;	
	//usart_send("write buf \r\n");
	NRF24L01_CE_0;
	NRF24L01_CSN_0;          //使能SPI传输
    status = SPI_NRF_RW(reg);//发送寄存器值(位置),并读取状态值
	for(u8_ctr=0; u8_ctr<len; u8_ctr++)
	SPI_NRF_RW(*pBuf++); //写入数据	 
  	NRF24L01_CSN_1;       //关闭SPI传输
  	return status;          //返回读到的状态值
	//usart_send("write buf ok \r\n");
}				   
//启动NRF24L01发送一次数据
//txbuf:待发送数据首地址
//返回值:发送完成状况
u8 NRF_Tx_Dat(u8 *txbuf)
{
	u8 sta;  
	NRF24L01_CE_0;
    NRF24L01_Write_Buf(WR_TX_PLOAD,txbuf,TX_PLOAD_WIDTH);//写数据到TX BUF  4个字节
 	NRF24L01_CE_1;//启动发送	   
	while(NRF24L01_IRQ!=0);//等待发送完成
	sta=NRF24L01_Read_Reg(STATUS);  //读取状态寄存器的值	   
	NRF24L01_Write_Reg(WRITE_REG_G+STATUS,sta); //清除TX_DS或MAX_RT中断标志
	if(sta&MAX_TX)//达到最大重发次数
	{
		NRF24L01_Write_Reg(FLUSH_TX,0xff);//清除TX FIFO寄存器 
		return MAX_TX; 
	}
	if(sta&TX_OK)//发送完成
	{
		return TX_OK;
	}
	return 0xff;//其他原因发送失败
}
//启动NRF24L01发送一次数据
//txbuf:待发送数据首地址
//返回值:0，接收完成；其他，错误代码
u8 NRF_Rx_Dat(u8 *rxbuf)
{
	u8 sta;			
	NRF24L01_CE_1;
	//tty_printf("0\n");
	while(NRF24L01_IRQ!=0);//wait receive over
	//tty_printf("1\n");
	sta=NRF24L01_Read_Reg(STATUS);  //读取状态寄存器的值 
	   	 
	NRF24L01_Write_Reg(WRITE_REG_G+STATUS,sta); //清除TX_DS或MAX_RT中断标志
	if(sta&RX_OK)//接收到数据
	{
		NRF24L01_Read_Buf(RD_RX_PLOAD,rxbuf,RX_PLOAD_WIDTH);//读取数据
		NRF24L01_Write_Reg(FLUSH_RX,0xff);//清除RX FIFO寄存器 
		return 0; 
	}	   
	return 1;//没收到任何数据
}					    
//该函数初始化NRF24L01到RX模式
//设置RX地址,写RX数据宽度,选择RF频道,波特率和LNA HCURR
//当CE变高后,即进入RX模式,并可以接收数据了		   
u8 NRF_RX_Mode(void)
{
	NRF24L01_CE_0;
  	NRF24L01_Write_Buf(WRITE_REG_G+RX_ADDR_P0,(u8*)RX_ADDRESS,RX_ADR_WIDTH);//写RX节点地址	 
  	NRF24L01_Write_Reg(WRITE_REG_G+EN_AA,0x01);    //使能通道0的自动应答 
  	NRF24L01_Write_Reg(WRITE_REG_G+EN_RXADDR,0x01);//使能通道0的接收地址  
  	NRF24L01_Write_Reg(WRITE_REG_G+RF_CH,1);	     //设置RF通信频率
  	NRF24L01_Write_Reg(WRITE_REG_G+RX_PW_P0,RX_PLOAD_WIDTH);//选择通道0的有效数据宽度 	
  	NRF24L01_Write_Reg(WRITE_REG_G+RF_SETUP,0x0f);//设置TX发射参数,0db增益,2Mbps,低噪声增益开启  
  	NRF24L01_Write_Reg(WRITE_REG_G+CONFIG, 0x0f);//配置基本工作模式的参数;PWR_UP,EN_CRC,16BIT_CRC,接收模式 
		//NRF24L01_CE_1; //CE为高,进入接收模式 
	return 1;
}		

//该函数初始化NRF24L01到TX模式
//设置TX地址,写TX数据宽度,设置RX自动应答的地址,填充TX发送数据,选择RF频道,波特率和LNA HCURR
//PWR_UP,CRC使能
//当CE变高后,即进入RX模式,并可以接收数据了		   
//CE为高大于10us,则启动发送.	 
u8 NRF_TX_Mode(void)
{		
	//u8 status;
	NRF24L01_CE_0;	    
  	NRF24L01_Write_Buf(WRITE_REG_G+TX_ADDR,(u8*)TX_ADDRESS,TX_ADR_WIDTH);//写TX节点地址 
  	NRF24L01_Write_Buf(WRITE_REG_G+RX_ADDR_P0,(u8*)RX_ADDRESS,RX_ADR_WIDTH); //设置TX节点地址,主要为了使能ACK	  

  	NRF24L01_Write_Reg(WRITE_REG_G+EN_AA,0x01);     //使能通道0的自动应答    
  	NRF24L01_Write_Reg(WRITE_REG_G+EN_RXADDR,0x01); //使能通道0的接收地址  
  	NRF24L01_Write_Reg(WRITE_REG_G+SETUP_RETR,0x1a);//设置自动重发间隔时间:500us + 86us;最大自动重发次数:10次
  	NRF24L01_Write_Reg(WRITE_REG_G+RF_CH,1);       //设置RF通道为1
  	NRF24L01_Write_Reg(WRITE_REG_G+RF_SETUP,0x0f);  //设置TX发射参数,0db增益,2Mbps,低噪声增益开启   
  	NRF24L01_Write_Reg(WRITE_REG_G+CONFIG,0x0e);    //配置基本工作模式的参数;PWR_UP,EN_CRC,16BIT_CRC,接收模式,开启所有中断  	
	  //NRF24L01_Write_Reg(WRITE_REG_G+CONFIG,0x0e);    //配置基本工作模式的参数;PWR_UP,EN_CRC,16BIT_CRC,接收模式,开启所有中断
	  NRF24L01_CE_1;//CE为高,10us后启动发送   
	return 1;
}
