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

const u8 TX_ADDRESS[TX_ADR_WIDTH]={0x34,0x43,0x10,0x10,0x01}; //���͵�ַ
const u8 RX_ADDRESS[RX_ADR_WIDTH]={0x34,0x43,0x10,0x10,0x01}; //���ܵ�ַ

u8 SPI_NRF_RW(u8 dat){
	while(SPI_I2S_GetFlagStatus(SPI2,SPI_I2S_FLAG_TXE)==RESET);
	SPI_I2S_SendData(SPI2,dat);
	while(SPI_I2S_GetFlagStatus(SPI2,SPI_I2S_FLAG_RXNE)==RESET);
	return SPI_I2S_ReceiveData(SPI2);
}

//SPIд�Ĵ���
//reg:ָ���Ĵ�����ַ
//value:д���ֵ
u8 NRF24L01_Write_Reg(u8 reg,u8 value)
{
	u8 status;	
   	NRF24L01_CSN_0;                 //ʹ��SPI����
  	status =SPI_NRF_RW(reg);//���ͼĴ����� 
  	SPI_NRF_RW(value);      //д��Ĵ�����ֵ
  	NRF24L01_CSN_1;                 //��ֹSPI����	   
  	return(status);       			//����״ֵ̬
}
//��ȡSPI�Ĵ���ֵ
//reg:Ҫ���ļĴ���
u8 NRF24L01_Read_Reg(u8 reg)
{
	u8 reg_val;	    
	NRF24L01_CSN_0;          //ʹ��SPI����
  	SPI_NRF_RW(reg);   //���ͼĴ�����
  	reg_val=SPI_NRF_RW(0XFF);//��ȡ�Ĵ�������
  	NRF24L01_CSN_1;          //��ֹSPI����		    
  	return(reg_val);           //����״ֵ̬
}


//��ʼ��24L01��IO��
void NRF24L01_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	SPI_InitTypeDef SPI_InitStructure;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB | RCC_APB2Periph_AFIO,ENABLE);//AFIO������SPI���ñ��뿪��
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


//���24L01�Ƿ����
//����ֵ:0���ɹ�;1��ʧ��	
u8 NRF24L01_Check(void)
{
	u8 buf[5]={0XA5,0XA5,0XA5,0XA5,0XA5};
	u8 buf1[5];
	u8 i; 
	//usart_send("check in\r\n");
	NRF24L01_Write_Buf(WRITE_REG_G+TX_ADDR,buf,5);//д��5���ֽڵĵ�ַ.	
	NRF24L01_Read_Buf(TX_ADDR,buf1,5); //����д��ĵ�ַ  
	for(i=0;i<5;i++)if(buf1[i]!=0XA5)break;
	//usart_send("check out\r\n");
	if(i!=5)return 0;//���24L01����	
	return 1;		 //��⵽24L01
}

//��ָ��λ�ö���ָ�����ȵ�����
//reg:�Ĵ���(λ��)
//*pBuf:����ָ��
//len:���ݳ���
//����ֵ,�˴ζ�����״̬�Ĵ���ֵ 
u8 NRF24L01_Read_Buf(u8 reg,u8 *pBuf,u8 len)
{
	u8 status,u8_ctr;	       
  	NRF24L01_CSN_0;           //ʹ��SPI����
	status=SPI_NRF_RW(reg);//���ͼĴ���ֵ(λ��),����ȡ״ֵ̬ 
	for(u8_ctr=0;u8_ctr<len;u8_ctr++)
	pBuf[u8_ctr]=SPI_NRF_RW(0XFF);//��������
  	NRF24L01_CSN_1;       //�ر�SPI����
  	return status;        //���ض�����״ֵ̬
}
//��ָ��λ��дָ�����ȵ�����
//reg:�Ĵ���(λ��)
//*pBuf:����ָ��
//len:���ݳ���
//����ֵ,�˴ζ�����״̬�Ĵ���ֵ
u8 NRF24L01_Write_Buf(u8 reg, u8 *pBuf, u8 len)
{
	
	u8 status,u8_ctr;	
	//usart_send("write buf \r\n");
	NRF24L01_CE_0;
	NRF24L01_CSN_0;          //ʹ��SPI����
    status = SPI_NRF_RW(reg);//���ͼĴ���ֵ(λ��),����ȡ״ֵ̬
	for(u8_ctr=0; u8_ctr<len; u8_ctr++)
	SPI_NRF_RW(*pBuf++); //д������	 
  	NRF24L01_CSN_1;       //�ر�SPI����
  	return status;          //���ض�����״ֵ̬
	//usart_send("write buf ok \r\n");
}				   
//����NRF24L01����һ������
//txbuf:�����������׵�ַ
//����ֵ:�������״��
u8 NRF_Tx_Dat(u8 *txbuf)
{
	u8 sta;  
	NRF24L01_CE_0;
    NRF24L01_Write_Buf(WR_TX_PLOAD,txbuf,TX_PLOAD_WIDTH);//д���ݵ�TX BUF  4���ֽ�
 	NRF24L01_CE_1;//��������	   
	while(NRF24L01_IRQ!=0);//�ȴ��������
	sta=NRF24L01_Read_Reg(STATUS);  //��ȡ״̬�Ĵ�����ֵ	   
	NRF24L01_Write_Reg(WRITE_REG_G+STATUS,sta); //���TX_DS��MAX_RT�жϱ�־
	if(sta&MAX_TX)//�ﵽ����ط�����
	{
		NRF24L01_Write_Reg(FLUSH_TX,0xff);//���TX FIFO�Ĵ��� 
		return MAX_TX; 
	}
	if(sta&TX_OK)//�������
	{
		return TX_OK;
	}
	return 0xff;//����ԭ����ʧ��
}
//����NRF24L01����һ������
//txbuf:�����������׵�ַ
//����ֵ:0��������ɣ��������������
u8 NRF_Rx_Dat(u8 *rxbuf)
{
	u8 sta;			
	NRF24L01_CE_1;
	//tty_printf("0\n");
	while(NRF24L01_IRQ!=0);//wait receive over
	//tty_printf("1\n");
	sta=NRF24L01_Read_Reg(STATUS);  //��ȡ״̬�Ĵ�����ֵ 
	   	 
	NRF24L01_Write_Reg(WRITE_REG_G+STATUS,sta); //���TX_DS��MAX_RT�жϱ�־
	if(sta&RX_OK)//���յ�����
	{
		NRF24L01_Read_Buf(RD_RX_PLOAD,rxbuf,RX_PLOAD_WIDTH);//��ȡ����
		NRF24L01_Write_Reg(FLUSH_RX,0xff);//���RX FIFO�Ĵ��� 
		return 0; 
	}	   
	return 1;//û�յ��κ�����
}					    
//�ú�����ʼ��NRF24L01��RXģʽ
//����RX��ַ,дRX���ݿ��,ѡ��RFƵ��,�����ʺ�LNA HCURR
//��CE��ߺ�,������RXģʽ,�����Խ���������		   
u8 NRF_RX_Mode(void)
{
	NRF24L01_CE_0;
  	NRF24L01_Write_Buf(WRITE_REG_G+RX_ADDR_P0,(u8*)RX_ADDRESS,RX_ADR_WIDTH);//дRX�ڵ��ַ	 
  	NRF24L01_Write_Reg(WRITE_REG_G+EN_AA,0x01);    //ʹ��ͨ��0���Զ�Ӧ�� 
  	NRF24L01_Write_Reg(WRITE_REG_G+EN_RXADDR,0x01);//ʹ��ͨ��0�Ľ��յ�ַ  
  	NRF24L01_Write_Reg(WRITE_REG_G+RF_CH,1);	     //����RFͨ��Ƶ��
  	NRF24L01_Write_Reg(WRITE_REG_G+RX_PW_P0,RX_PLOAD_WIDTH);//ѡ��ͨ��0����Ч���ݿ�� 	
  	NRF24L01_Write_Reg(WRITE_REG_G+RF_SETUP,0x0f);//����TX�������,0db����,2Mbps,���������濪��  
  	NRF24L01_Write_Reg(WRITE_REG_G+CONFIG, 0x0f);//���û�������ģʽ�Ĳ���;PWR_UP,EN_CRC,16BIT_CRC,����ģʽ 
		//NRF24L01_CE_1; //CEΪ��,�������ģʽ 
	return 1;
}		

//�ú�����ʼ��NRF24L01��TXģʽ
//����TX��ַ,дTX���ݿ��,����RX�Զ�Ӧ��ĵ�ַ,���TX��������,ѡ��RFƵ��,�����ʺ�LNA HCURR
//PWR_UP,CRCʹ��
//��CE��ߺ�,������RXģʽ,�����Խ���������		   
//CEΪ�ߴ���10us,����������.	 
u8 NRF_TX_Mode(void)
{		
	//u8 status;
	NRF24L01_CE_0;	    
  	NRF24L01_Write_Buf(WRITE_REG_G+TX_ADDR,(u8*)TX_ADDRESS,TX_ADR_WIDTH);//дTX�ڵ��ַ 
  	NRF24L01_Write_Buf(WRITE_REG_G+RX_ADDR_P0,(u8*)RX_ADDRESS,RX_ADR_WIDTH); //����TX�ڵ��ַ,��ҪΪ��ʹ��ACK	  

  	NRF24L01_Write_Reg(WRITE_REG_G+EN_AA,0x01);     //ʹ��ͨ��0���Զ�Ӧ��    
  	NRF24L01_Write_Reg(WRITE_REG_G+EN_RXADDR,0x01); //ʹ��ͨ��0�Ľ��յ�ַ  
  	NRF24L01_Write_Reg(WRITE_REG_G+SETUP_RETR,0x1a);//�����Զ��ط����ʱ��:500us + 86us;����Զ��ط�����:10��
  	NRF24L01_Write_Reg(WRITE_REG_G+RF_CH,1);       //����RFͨ��Ϊ1
  	NRF24L01_Write_Reg(WRITE_REG_G+RF_SETUP,0x0f);  //����TX�������,0db����,2Mbps,���������濪��   
  	NRF24L01_Write_Reg(WRITE_REG_G+CONFIG,0x0e);    //���û�������ģʽ�Ĳ���;PWR_UP,EN_CRC,16BIT_CRC,����ģʽ,���������ж�  	
	  //NRF24L01_Write_Reg(WRITE_REG_G+CONFIG,0x0e);    //���û�������ģʽ�Ĳ���;PWR_UP,EN_CRC,16BIT_CRC,����ģʽ,���������ж�
	  NRF24L01_CE_1;//CEΪ��,10us����������   
	return 1;
}
