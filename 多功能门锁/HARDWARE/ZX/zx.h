#ifndef __ZX_H
#define __ZX_H
#include "delay.h"
#include "sys.h"
#include "AS608.h"

#define usart2_baund  57600//����2�����ʣ�����ָ��ģ�鲨���ʸ��ģ�ע�⣺ָ��ģ��Ĭ��57600��

void link(void);
	
void Add_FR(void);	//¼ָ��
void Del_FR(void);	//ɾ��ָ��
void press_FR(void);//ˢָ��
void passKS(void);//���뿪��
void Administrator(u16 a[],u16 b[],u8* q);//����Ա
void linkA(void);//������


void AS608_load_keyboard(u16 x,u16 y,u8 **kbtbl);
void AS608_key_staset(u16 x,u16 y,u8 keyx,u8 sta);
u8 AS608_get_keynum(u16 x,u16 y);
u16 GET_NUM(void);
void ShowErrMessage(u8 ensure);


#endif
