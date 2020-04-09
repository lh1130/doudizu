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
#define usart2_baund  57600//串口2波特率，根据指纹模块波特率更改（注意：指纹模块默认57600）


int main(void)
{	
//	u8 reclen=0;
	
	
	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//设置系统中断优先级分组2
	delay_init();  	    //初始化延时函数
	uart_init(115200);	//初始化串口1波特率为115200，用于支持USMART
	usart2_init(usart2_baund);//初始化串口2,用于与指纹模块通讯
	PS_StaGPIO_Init();	//初始化FR读状态引脚
	KEY_Init();					//按键初始化 
 	LCD_Init();					//LCD初始化 	
	tp_dev.init();			//初始化触摸屏
	LED_Init();
	usmart_dev.init(72);//初始化USMART
	mem_init();					//初始化内存池 
	exfuns_init();			//为fatfs相关变量申请内存  
 	f_mount(fs[1],"1:",1);  //挂载FLASH.
	POINT_COLOR=RED;
	RC522_Init();       //初始化射频卡模块
	delay_ms(1000);			//等待蓝牙模块上电稳定
 	HC05_Init();		//初始化ATK-HC05模块  
	delay_ms(100);
	USART3_RX_STA=0;
	
	

	while(1){
 
		linkA();
		
	}




}         

