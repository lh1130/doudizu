#include "sys.h"
#include "delay.h"
#include "usart.h"
#include "lcd.h"
#include "key.h"
#include "touch.h"	  
#include "malloc.h" 
#include "usmart.h"  
#include "MMC_SD.h"   
#include "ff.h"  
#include "exfuns.h"    
#include "fontupd.h"
#include "text.h"	
#include "usart2.h"
#include "timer.h"
#include "zx.h"
#include "rc522.h"
#include "led.h"
#include "hc05.h"
#include "usart3.h"
#define usart2_baund  57600//����2�����ʣ�����ָ��ģ�鲨���ʸ��ģ�ע�⣺ָ��ģ��Ĭ��57600��


int main(void)
{	
//	u8 reclen=0;
	
	
	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//����ϵͳ�ж����ȼ�����2
	delay_init();  	    //��ʼ����ʱ����
	uart_init(115200);	//��ʼ������1������Ϊ115200������֧��USMART
	usart2_init(usart2_baund);//��ʼ������2,������ָ��ģ��ͨѶ
	PS_StaGPIO_Init();	//��ʼ��FR��״̬����
	KEY_Init();					//������ʼ�� 
 	LCD_Init();					//LCD��ʼ�� 	
	tp_dev.init();			//��ʼ��������
	LED_Init();
	usmart_dev.init(72);//��ʼ��USMART
	mem_init();					//��ʼ���ڴ�� 
	exfuns_init();			//Ϊfatfs��ر��������ڴ�  
 	f_mount(fs[1],"1:",1);  //����FLASH.
	POINT_COLOR=RED;
	RC522_Init();       //��ʼ����Ƶ��ģ��
	delay_ms(1000);			//�ȴ�����ģ���ϵ��ȶ�
 	HC05_Init();		//��ʼ��ATK-HC05ģ��  
	delay_ms(100);
	USART3_RX_STA=0;
	
	

	while(1){
 
		linkA();
		
	}




}         

