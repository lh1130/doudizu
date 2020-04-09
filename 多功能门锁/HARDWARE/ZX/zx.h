#ifndef __ZX_H
#define __ZX_H
#include "delay.h"
#include "sys.h"
#include "AS608.h"

#define usart2_baund  57600//串口2波特率，根据指纹模块波特率更改（注意：指纹模块默认57600）

void link(void);
	
void Add_FR(void);	//录指纹
void Del_FR(void);	//删除指纹
void press_FR(void);//刷指纹
void passKS(void);//密码开锁
void Administrator(u16 a[],u16 b[],u8* q);//管理员
void linkA(void);//主界面


void AS608_load_keyboard(u16 x,u16 y,u8 **kbtbl);
void AS608_key_staset(u16 x,u16 y,u8 keyx,u8 sta);
u8 AS608_get_keynum(u16 x,u16 y);
u16 GET_NUM(void);
void ShowErrMessage(u8 ensure);


#endif
