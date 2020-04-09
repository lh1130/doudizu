#include "zx.h"
#include "lcd.h"
#include "key.h"
#include "touch.h"	  
#include "malloc.h"
#include "text.h"	
#include "rc522.h"
#include "usart3.h"
#include "led.h"
SysPara AS608Para;//ָ��ģ��AS608����
u16 ValidN;//ģ������Чģ�����

u8** kbd_tbl;
u16 a[1]={111};//��������
u16 b[1]={222};//����Ա����
u16 c[1]={333};
u8* q=NULL;



const  u8* kbd_menu[15]={"del_fp"," : ","add_fp","1","2","3","4","5","6","7","8","9","Cancle","0","Enter",};//������
const  u8* kbd_delFR[15]={"BACK"," : ","del_all","1","2","3","4","5","6","7","8","9","Cancle","0","DEL",};//������


//��ȡ������ֵ
u16 GET_NUM(void)
{
	u8  key_num=0;
	u16 num=0;
	while(1)
	{
		key_num=AS608_get_keynum(0,170);	
		if(key_num)
		{
			if(key_num==1)return 0xFFFF;//�����ء���
			if(key_num==3)return 0xFF00;//		
			if(key_num>3&&key_num<13&&num<99)//��1-9����(��������3λ��)
				num =num*10+key_num-3;		
			if(key_num==13)num =num/10;//��Del����			
			if(key_num==14&&num<99)num =num*10;//��0����
			if(key_num==15)return num;  //��Enter����
		}
		LCD_ShowNum(80+15,170+7,num,6,16);
	}	
}
//��ָ֤��
void inspect(void){

	u8 key_num;

	while(1)
	{
		key_num=AS608_get_keynum(0,170);	
		if(key_num)
		{
			if(key_num==1)Del_FR();		//ɾָ��
			if(key_num==3)Add_FR();		//¼ָ��
			if(key_num==5)linkA();		// ����������			
		}
		if(PS_Sta)	 //���PS_Sta״̬���������ָ����
		{
			press_FR();//ˢָ��			
		}				 
	} 	
}
//��ʾ���棬���ӹ���
void link(void){

	u8 ensure;
	char *str;
	
	
	while(font_init()) 			//����ֿ�
	{	    
		LCD_ShowString(60,50,240,16,16,"Font Error!");
		delay_ms(200);				  
		LCD_Fill(60,50,240,66,WHITE);//�����ʾ
		delay_ms(200);		
	}
	/*����ָ��ʶ��ʵ�����*/
	LCD_Clear(WHITE);
	POINT_COLOR=BLACK;
	Show_Str_Mid(0,0,"ָ�ƿ����ӳɹ�",16,240);	    			    	 
	Show_Str_Mid(0,20,"ָ�ƹ���",16,240);				    	 
 	POINT_COLOR=BLACK;
	Show_Str_Mid(0,40,"Connect with AS608....",16,240);	
	while(PS_HandShake(&AS608Addr))//��AS608ģ������
	{
		LCD_Fill(0,40,240,80,WHITE);
		Show_Str_Mid(0,40,"Cannot connect with AS608!",16,240);
		delay_ms(1000);
		LCD_Fill(0,40,240,80,WHITE);
		Show_Str_Mid(0,40,"Try to connect again....",16,240);	//Try to connect again
		delay_ms(1000);	  
	}
	LCD_Fill(0,40,240,320,WHITE);
	Show_Str_Mid(0,40,"����������Ͻ��в���!",16,240);//ͨѶ�ɹ�
	Show_Str_Mid(0,120,"2.����������",16,240);
	
	str=mymalloc(30);
	sprintf(str,"Baudrate:%d   Addr:%x",usart2_baund,AS608Addr);//��ʾ������
	//Show_Str(0,60,240,16,(u8*)str,16,0);
	delay_ms(100);
	ensure=PS_ValidTempleteNum(&ValidN);//����ָ�Ƹ���
	if(ensure!=0x00)
		ShowErrMessage(ensure);//��ʾȷ���������Ϣ	
	ensure=PS_ReadSysPara(&AS608Para);  //��AS608ģ����� 
	if(ensure==0x00)
	{
		mymemset(str,0,50);
		sprintf(str,"RemainNum:%d    Level:%d",AS608Para.PS_max-ValidN,AS608Para.PS_level);//��ʾʣ��ָ�������Ͱ�ȫ�ȼ�
		Show_Str(0,60,240,16,(u8*)str,16,0);
	}
	else
		ShowErrMessage(ensure);	
	myfree(str);
	AS608_load_keyboard(0,170,(u8**)kbd_menu);//�����������
}


void passKS(void){
	u8 i=000;
	u16 Num[1];
	Num[0]=a[0];
	while(1){
		Show_Str_Mid(0,120,"���������룡",16,240);
		i=GET_NUM();//��ȡ���ص���ֵ
		if(i!=000){
			if(Num[0]==i){
				LCD_Fill(0,120,lcddev.width,160,WHITE);
				Show_Str_Mid(0,120,"����������ȷ���ѿ�����",16,240);//	
					LED1=0;
					delay_ms(50000);
					delay_ms(50000);
					delay_ms(50000);
					LED1=~LED1;
				delay_ms(6000);//��ʱ�������ʾ
				delay_ms(6000);//��ʱ�������ʾ
				delay_ms(6000);//��ʱ�������ʾ
			
				LCD_Fill(0,120,lcddev.width,140,WHITE);				
				
			}else{
				LCD_Fill(0,120,lcddev.width,160,WHITE);
				Show_Str_Mid(0,120,"����������������²�����",16,240);//	
			Show_Str_Mid(0,140,"000Ϊ��Ч���룡",16,240);
			delay_ms(6000);//��ʱ�������ʾ
			delay_ms(6000);//��ʱ�������ʾ
			delay_ms(6000);//��ʱ�������ʾ
			
			LCD_Fill(0,120,lcddev.width,160,WHITE);
				
			


			
				
				
			}
		}
		
		break;
	}
	
}
void XGGpas(u16 a[],u16 b[],u8* p){
	u16 num=0;
	u16 num1=1;
	
	

	Show_Str_Mid(0,120,"�������¹���Ա���룡",16,240);
	num=GET_NUM();
	delay_ms(6000);//��ʱ�������ʾ
	delay_ms(6000);//��ʱ�������ʾ
	delay_ms(6000);//��ʱ�������ʾ
	
	LCD_Fill(0,120,lcddev.width,140,WHITE);
	
	if(num!=000){
		num1=num;
		Show_Str_Mid(0,120,"������һ�Σ�",16,240);
		num=GET_NUM();
		delay_ms(6000);//��ʱ�������ʾ
		delay_ms(6000);//��ʱ�������ʾ
		delay_ms(6000);//��ʱ�������ʾ
	
		LCD_Fill(0,120,lcddev.width,140,WHITE);
		if(num==num1){
			printf("b1:%d",num);
			b[0]=num;
			printf("b2:%d",b[0]);
			Show_Str_Mid(0,120,"����Ա�����޸ĳɹ���",16,240);
			delay_ms(6000);//��ʱ�������ʾ
			delay_ms(6000);//��ʱ�������ʾ
			delay_ms(6000);//��ʱ�������ʾ
			
			LCD_Fill(0,120,lcddev.width,140,WHITE);
			linkA();
		}else{
		Show_Str_Mid(0,120,"�������벻һ�£�",16,240);
		delay_ms(6000);//��ʱ�������ʾ
		delay_ms(6000);//��ʱ�������ʾ
		delay_ms(6000);//��ʱ�������ʾ
		LCD_Fill(0,120,lcddev.width,140,WHITE);
		Administrator(a,b,q);
		}
	}else{
		Show_Str_Mid(0,120,"�����벻��Ϊ000��",16,240);
		delay_ms(6000);//��ʱ�������ʾ
		delay_ms(6000);//��ʱ�������ʾ
		delay_ms(6000);//��ʱ�������ʾ
	
		LCD_Fill(0,120,lcddev.width,140,WHITE);
		printf("b3:%d",b[0]);
		Administrator(a,b,q);
	}
}

void GLkspas(u16 a[],u16 b[],u8* p){
	u16 num=0;
	u16 num1=1;
	
	
	
	Show_Str_Mid(0,120,"�������¿������룡",16,240);
	num=GET_NUM();
	delay_ms(6000);//��ʱ�������ʾ
	delay_ms(6000);//��ʱ�������ʾ
	delay_ms(6000);//��ʱ�������ʾ
	
	LCD_Fill(0,120,lcddev.width,140,WHITE);
	
	if(num!=000){
		num1=num;
		Show_Str_Mid(0,120,"������һ�Σ�",16,240);
		num=GET_NUM();
		delay_ms(6000);//��ʱ�������ʾ
		delay_ms(6000);//��ʱ�������ʾ
		delay_ms(6000);//��ʱ�������ʾ
	
		LCD_Fill(0,120,lcddev.width,140,WHITE);
		if(num==num1){
			//printf("b1:%d",num);
			a[0]=num;
			//printf("b2:%d",b[0]);
			Show_Str_Mid(0,120,"���������޸ĳɹ���",16,240);
			delay_ms(6000);//��ʱ�������ʾ
			delay_ms(6000);//��ʱ�������ʾ
			delay_ms(6000);//��ʱ�������ʾ
	
			LCD_Fill(0,120,lcddev.width,140,WHITE);
			//Administrator(a,b,c);
			linkA();
		}else{
		Show_Str_Mid(0,120,"�������벻һ�£�",16,240);
		delay_ms(6000);//��ʱ�������ʾ
		delay_ms(6000);//��ʱ�������ʾ
		LCD_Fill(0,120,lcddev.width,140,WHITE);
		Administrator(a,b,q);
		}
	}else{
		Show_Str_Mid(0,120,"�����벻��Ϊ000��",16,240);
		delay_ms(6000);//��ʱ�������ʾ
		delay_ms(6000);//��ʱ�������ʾ
		delay_ms(6000);//��ʱ�������ʾ
	
		LCD_Fill(0,120,lcddev.width,140,WHITE);
		//printf("b3:%d",b[0]);
		Administrator(a,b,q);
	}
}


void addID(u8* p){

	    while(1)
    {

		 u8 num[9];
		 u8 Num[9];
		u8* o;
		u8 i;
	o=RC522_Handel();
	q=o;

		for(i=0; i<4; i++)
		{
        num[i*2]=o[i]/16;
        num[i*2]>9?(num[i*2]+='7'):(num[i*2]+='0');
        num[i*2+1]=o[i]%16;
        num[i*2+1]>9?(num[i*2+1]+='7'):(num[i*2+1]+='0');
		}
		num[8]=0;
		
				for(i=0; i<4; i++)
		{
        Num[i*2]=q[i]/16;
        Num[i*2]>9?(Num[i*2]+='7'):(Num[i*2]+='0');
        Num[i*2+1]=q[i]%16;
        Num[i*2+1]>9?(Num[i*2+1]+='7'):(Num[i*2+1]+='0');
		}
		Num[8]=0;
		
		printf("ID1>>>%s\r\n", num);
		
		printf("ID2>>>%s\r\n", Num);
		
			Show_Str_Mid(0,120,"ID����ӳɹ���",16,240);
			delay_ms(6000);//��ʱ�������ʾ
			delay_ms(6000);//��ʱ�������ʾ
			delay_ms(6000);//��ʱ�������ʾ
			LCD_Fill(0,120,lcddev.width,140,WHITE);
		
		 delay_ms(300);
		
		break;
    }
	//return q;
}
void CZID(u8* p){
	q=NULL;
	if(q==NULL){
			Show_Str_Mid(0,120,"ID�����óɹ���",16,240);
			delay_ms(6000);//��ʱ�������ʾ
			delay_ms(6000);//��ʱ�������ʾ
			delay_ms(6000);//��ʱ�������ʾ
			LCD_Fill(0,120,lcddev.width,140,WHITE);
		delay_ms(300);
		
	}
//	u8 i;
//	u8 num[9];
//			for(i=0; i<4; i++)
//		{
//        num[i*2]=q[i]/16;
//        num[i*2]>9?(num[i*2]+='7'):(num[i*2]+='0');
//        num[i*2+1]=q[i]%16;
//        num[i*2+1]>9?(num[i*2+1]+='7'):(num[i*2+1]+='0');
//		}
//		num[8]=0;
	printf("CZp>>>%s\r\n", q);
	//return q;
}
void IDpas(u8* p){
	u8 i,t;
	u8 r=0;
	u8* o;//������ˢ��p�Ǵ�
	u8  e;
	u8 Num[9];
	u8 num[9];
	u8* u;
	u=q;
		for(i=0; i<4; i++)
		{
        Num[i*2]=u[i]/16;
        Num[i*2]>9?(Num[i*2]+='7'):(Num[i*2]+='0');
        Num[i*2+1]=u[i]%16;
        Num[i*2+1]>9?(Num[i*2+1]+='7'):(Num[i*2+1]+='0');
		}
		Num[8]=0;
		
		e=idfh();	
	if(e==1){
				o=RC522_Handel();
		for(i=0; i<4; i++)
		{
        num[i*2]=o[i]/16;
        num[i*2]>9?(num[i*2]+='7'):(num[i*2]+='0');
        num[i*2+1]=o[i]%16;
        num[i*2+1]>9?(num[i*2+1]+='7'):(num[i*2+1]+='0');
		}
		num[8]=0;
			for(t=0;t<8;t++){
			if(num[t]==Num[t]){
				//printf ("SC\r\n");
				r=1;
			}else{
				//printf ("AC\r\n");
				r=0;
				break;
			}
		}
		if(r==0){
			printf("AC\r\n");
	Show_Str_Mid(0,120,"��֤ʧ�ܣ�",16,240);
	delay_ms(6000);//��ʱ�������ʾ
	delay_ms(6000);//��ʱ�������ʾ
	LCD_Fill(0,120,lcddev.width,140,WHITE);
			delay_ms(3000);
		}else{
			printf ("SC\r\n");
	Show_Str_Mid(0,120,"��֤�ɹ����ѿ�����",16,240);
	delay_ms(6000);//��ʱ�������ʾ
	delay_ms(6000);//��ʱ�������ʾ
	LCD_Fill(0,120,lcddev.width,140,WHITE);
		LED1=0;
        delay_ms(50000);
        delay_ms(50000);
        delay_ms(50000);
        LED1=~LED1;
			
			delay_ms(3000);
		}
		
		
		printf("ID1>>>%s\r\n", Num);
		printf("ID2>>>%s\r\n", num);
		
		
	}else if(e==2){
		printf(" �����ID��\r\n" );
			Show_Str_Mid(0,120,"�����ID����",16,240);
	delay_ms(6000);//��ʱ�������ʾ
	delay_ms(6000);//��ʱ�������ʾ
	LCD_Fill(0,120,lcddev.width,140,WHITE);
	}


		

//	printf("ID1>>>%s\r\n", q);
	

}
void UIID(u8*p){
	u8 key;
	char *str;
	LCD_Clear(WHITE);
	POINT_COLOR=BLACK;
	Show_Str_Mid(0,0,"ID������",16,240);
	Show_Str_Mid(0,40,"1.���ID����..",16,240);
	Show_Str_Mid(0,60,"2.����ID����..",16,240);
	Show_Str_Mid(0,80,"3.������ҳ�棡",16,240);
	myfree(str);
	AS608_load_keyboard(0,170,(u8**)kbd_menu);//�����������
	
	Show_Str_Mid(0,120,"��ѡ��",16,240);
	delay_ms(6000);//��ʱ�������ʾ
	delay_ms(6000);//��ʱ�������ʾ
	LCD_Fill(0,120,lcddev.width,140,WHITE);
	
	while(1){
		key=GET_NUM();
		if(key==1){
			//���ID��
			addID(q);
	}else if(key==2){
		//����ID��
		CZID(q);
	}else if(key==3){
		//
		linkA();
		 
	}else{
	continue;
	}
	}

	
}
void Administrator(u16 a[],u16 b[],u8* p){
	char *str;
	//u8 c[1];
	u16 numG,num;
	
	//c[0]=b[0];
	Show_Str_Mid(0,120,"���������Ա���룡",16,240);
	printf("b:%d",b[0]);
	delay_ms(6000);//��ʱ�������ʾ
	delay_ms(6000);//��ʱ�������ʾ
	delay_ms(6000);//��ʱ�������ʾ
	LCD_Fill(0,120,lcddev.width,140,WHITE);
	numG =GET_NUM ();
	if(b[0]==numG){
	while(font_init()) 			//����ֿ�
	{	    
		LCD_ShowString(60,50,240,16,16,"Font Error!");
		delay_ms(200);				  
		LCD_Fill(60,50,240,66,WHITE);//�����ʾ
		delay_ms(200);		
	}
	/*����ָ��ʶ��ʵ�����*/
	LCD_Clear(WHITE);
	POINT_COLOR=BLACK;
	Show_Str_Mid(0,0,"���й���ԱȨ�ޣ�������޸ģ�",16,240);	    			    	 
	Show_Str_Mid(0,20,"1.����������...",16,240);				    	 
 	POINT_COLOR=BLACK;
	Show_Str_Mid(0,40,"2.����ָ��......",16,240);
	Show_Str_Mid(0,60,"3.����ID��......",16,240);
	Show_Str_Mid(0,80,"4.�޸Ĺ���Ա����",16,240);
	Show_Str_Mid(0,100,"5.�˳�����ԱȨ��",16,240);
	myfree(str);
	AS608_load_keyboard(0,170,(u8**)kbd_menu);//�����������
	while(1){
		num=GET_NUM();
	if(num==1){
		//����������
		GLkspas(a,b,q);
	}else if(num==2){
		//����ָ��
		link();
		inspect();
	}else if(num==3){
		//����ID��
		UIID(q);
		//break;
	}else if(num==4){
		//�޸Ĺ���Ա����
		XGGpas(a,b,q);
	}else if(num==5){
		//�˻�������
		linkA();
	
	}	
	
	}
	}else{
			Show_Str_Mid(0,120,"����Ա�����������",16,240);
			delay_ms(6000);//��ʱ�������ʾ
			delay_ms(6000);//��ʱ�������ʾ
			LCD_Fill(0,120,lcddev.width,140,WHITE);
			}
 
}

 

 
//��ʾ������
void linkA(void){
	char *str;
	u16 num;
	u8 i=0;
	u8 reclen=0;
	USART3_RX_STA=0;


	while(font_init()) 			//����ֿ�
	{	    
		LCD_ShowString(60,50,240,16,16,"Font Error!");
		delay_ms(200);				  
		LCD_Fill(60,50,240,66,WHITE);//�����ʾ
		delay_ms(200);		
	}
	/*����ָ��ʶ��ʵ�����*/
	LCD_Clear(WHITE);
	POINT_COLOR=BLACK;
	Show_Str_Mid(0,0,"��ӭ�ؼң������Ž���",16,240);	    			    	 
	Show_Str_Mid(0,20,"��ѡ��~",16,240);				    	 
 	POINT_COLOR=BLACK;
	Show_Str_Mid(0,40,"1.���뿪��..",16,240);
	Show_Str_Mid(0,60,"2.ָ�ƿ���..",16,240);
	Show_Str_Mid(0,80,"3.ID������..",16,240);
    Show_Str_Mid(0,100,"4.����Աģʽ",16,240);
	myfree(str);
	AS608_load_keyboard(0,170,(u8**)kbd_menu);//�����������
	
	while(1){
//			u8 t;
//			t=KEY_Scan(1);
		
			tp_dev.scan(0); 
		if(tp_dev.sta&TP_PRES_DOWN){
		num=GET_NUM();//��ȡ���ص���ֵ
		if(num==1){
			passKS();//���뿪��
			//break;
		}else if(num==2){//ָ�ƿ���
			
			for(i=0;i<5;i++){
				if(PS_Sta){	 //���PS_Sta״̬���������ָ����
					press_FR();//ˢָ��		
					break;
				}
			
			}
				
		}else if(num==3){
				//ID������
			IDpas(q);
		}else if(num==4){
			 
			Administrator(a,b,q);
 
			// ����Աģʽ
		}else {
			Show_Str_Mid(0,120,"���������룡",16,240);
			delay_ms(6000);//��ʱ�������ʾ
			delay_ms(6000);//��ʱ�������ʾ
			delay_ms(6000);//��ʱ�������ʾ
			 LCD_Fill(0,120,lcddev.width,140,WHITE);
			
		}
			  
			  
			//printf("123\r\n");
			  delay_ms(100);	
			
			
			
			printf("123\r\n");
			delay_ms(500);
		}else{
		if(USART3_RX_STA&0X8000)			//���յ�һ��������
		{
 			reclen=USART3_RX_STA&0X7FFF;	//�õ����ݳ���
		  USART3_RX_BUF[reclen]=0;	 	//���������

			if(USART3_RX_BUF[0]=='1'){
				printf("145\r\n");
 			printf("%s\r\n",USART3_RX_BUF);//��ʾ���յ�������
				printf("146\r\n");
			LED1=0;
        delay_ms(50000);
        delay_ms(50000);
        delay_ms(50000);
        LED1=~LED1;
		printf("123");
			}
 			USART3_RX_STA=0;	 
		} 
			  
			  
			  
			//printf("456\r\n");
			  delay_ms(100);
			
			
			
			printf("456\r\n");
			delay_ms(500);
		}
		
		
//		  if(t==KEY0_PRES){

//		  }else{
//			  
// 
//		  }
		

		
		
	}
	

}





//���ذ������棨�ߴ�x,yΪ240*150��
//x,y:������ʼ���꣨240*320�ֱ��ʵ�ʱ��x����Ϊ0��
void AS608_load_keyboard(u16 x,u16 y,u8 **kbtbl)
{
	u16 i;
	POINT_COLOR=BLACK;
	kbd_tbl=kbtbl;
	LCD_Fill(x,y,x+240,y+150,WHITE);
	LCD_DrawRectangle(x,y,x+240,y+150);						   
	LCD_DrawRectangle(x+80,y,x+160,y+150);	 
	LCD_DrawRectangle(x,y+30,x+240,y+60);
	LCD_DrawRectangle(x,y+90,x+240,y+120);
	POINT_COLOR=BLUE;
	for(i=0;i<15;i++)
	{
		if(i==1)//�������2����:������Ҫ�м���ʾ
			Show_Str(x+(i%3)*80+2,y+7+30*(i/3),80,30,(u8*)kbd_tbl[i],16,0);	
		else
			Show_Str_Mid(x+(i%3)*80,y+7+30*(i/3),(u8*)kbd_tbl[i],16,80);
	} 
}


//����״̬����
//x,y:��������
//key:��ֵ(0~14)
//sta:״̬��0���ɿ���1�����£�
void AS608_key_staset(u16 x,u16 y,u8 keyx,u8 sta)
{		  
	u16 i=keyx/3,j=keyx%3;
	if(keyx>16)return;
	if(sta &&keyx!=1)//�������2����:������Ҫ���
		LCD_Fill(x+j*80+1,y+i*30+1,x+j*80+78,y+i*30+28,GREEN);
	else if(keyx!=1)
		LCD_Fill(x+j*80+1,y+i*30+1,x+j*80+78,y+i*30+28,WHITE);
	if(keyx!=1)//���ǡ�����
		Show_Str_Mid(x+j*80,y+7+30*i,(u8*)kbd_tbl[keyx],16,80);	
}
//�õ�������������
//x,y:��������
//����ֵ:��1~15,��Ӧ������
u8 AS608_get_keynum(u16 x,u16 y)
{
	u16 i,j;
	static u8 key_x=0;//0,û���κΰ�������
	u8 key=0;
	tp_dev.scan(0); 		 
	if(tp_dev.sta&TP_PRES_DOWN)//������������
	{	
		for(i=0;i<5;i++)
		{
			for(j=0;j<3;j++)
			{
			 	if(tp_dev.x[0]<(x+j*80+80)&&tp_dev.x[0]>(x+j*80)&&tp_dev.y[0]<(y+i*30+30)&&tp_dev.y[0]>(y+i*30))
				{	
					key=i*3+j+1;	 
					break;	 		   
				}
			}
			if(key)
			{	   
				if(key_x==key)key=0;
				else 
				{
					AS608_key_staset(x,y,key_x-1,0);
					key_x=key;
					AS608_key_staset(x,y,key_x-1,1);
				}
				break;
			}
	  }  
	}else 
if(key_x) 
	{
		AS608_key_staset(x,y,key_x-1,0);
		key_x=0;
	} 
	return key; 
}

//��ʾȷ���������Ϣ
void ShowErrMessage(u8 ensure)
{
	LCD_Fill(0,120,lcddev.width,160,WHITE);
	Show_Str_Mid(0,120,(u8*)EnsureMessage(ensure),16,240);
}



//ָ�Ʋ���
//¼ָ��
void Add_FR(void)
{
	u8 i=0,ensure ,processnum=0;
	u16 ID;
	u8 num;
	while(1)
	{
		switch (processnum)
		{
			case 0:
				i++;
				LCD_Fill(0,100,lcddev.width,160,WHITE);
				Show_Str_Mid(0,100,"Pleas touch finger!",16,240);//�밴��ָ
				ensure=PS_GetImage();
				if(ensure==0x00) 
				{
					ensure=PS_GenChar(CharBuffer1);//��������
					if(ensure==0x00)
					{
						LCD_Fill(0,120,lcddev.width,160,WHITE);
						Show_Str_Mid(0,120,"Fingerprint correct",16,240);//ָ����ȷ
						i=0;
						processnum=1;//�����ڶ���						
					}else ShowErrMessage(ensure);				
				}else ShowErrMessage(ensure);						
				break;
			
			case 1:
				i++;
				LCD_Fill(0,100,lcddev.width,160,WHITE);
				Show_Str_Mid(0,100,"Pleas touch finger again!",16,240);//�ٰ�һ����ָ
				ensure=PS_GetImage();
				if(ensure==0x00) 
				{
					ensure=PS_GenChar(CharBuffer2);//��������			
					if(ensure==0x00)
					{
						LCD_Fill(0,120,lcddev.width,160,WHITE);
						Show_Str_Mid(0,120,"Fingerprint correct",16,240);//ָ����ȷ
						i=0;
						processnum=2;//����������
					}else ShowErrMessage(ensure);	
				}else ShowErrMessage(ensure);		
				break;

			case 2:
				LCD_Fill(0,100,lcddev.width,160,WHITE);
				Show_Str_Mid(0,100,"Compare twice fingerprint",16,240);//�Ա�����ָ��
				ensure=PS_Match();
				if(ensure==0x00) 
				{
					LCD_Fill(0,120,lcddev.width,160,WHITE);
					Show_Str_Mid(0,120,"Twice fingerprint are same",16,240);//����ָ����һ����
					processnum=3;//�������Ĳ�
				}
				else 
				{
					LCD_Fill(0,100,lcddev.width,160,WHITE);
					Show_Str_Mid(0,100,"Compare fail,pleas touch again!",16,240);//�Ա�ʧ�ܣ������°���ָ
					ShowErrMessage(ensure);
					i=0;
					processnum=0;//���ص�һ��		
				}
				delay_ms(1000);
				break;

			case 3:
				LCD_Fill(0,100,lcddev.width,160,WHITE);
				Show_Str_Mid(0,100,"Generate fingerprint template",16,240);//����һ��ָ��ģ��
				ensure=PS_RegModel();
				if(ensure==0x00) 
				{
					LCD_Fill(0,120,lcddev.width,160,WHITE);
					Show_Str_Mid(0,120,"Generate fingerprint success",16,240);//����ָ��ģ��ɹ�
					processnum=4;//�������岽
				}else {processnum=0;ShowErrMessage(ensure);}
				delay_ms(1000);
				break;
				
			case 4:	
				LCD_Fill(0,100,lcddev.width,160,WHITE);
				Show_Str_Mid(0,100,"Intput ID and save with ENTER!",16,240);//����ID������Enter������
				Show_Str_Mid(0,120,"0=< ID <=299",16,240);	
				do
					ID=GET_NUM();
				while(!(ID<300));//����DI����С��300
				ensure=PS_StoreChar(CharBuffer2,ID);//����ģ��
				if(ensure==0x00) 
				{			
					LCD_Fill(0,100,lcddev.width,160,WHITE);					
					Show_Str_Mid(0,120,"Add fingerprint success!!!",16,240);//���ָ�Ƴɹ�
					PS_ValidTempleteNum(&ValidN);//����ָ�Ƹ���
					LCD_ShowNum(80,80,AS608Para.PS_max-ValidN,3,16);//��ʾʣ��ָ�Ƹ���
					delay_ms(1500);//��ʱ�������ʾ	
					LCD_Fill(0,100,240,160,WHITE);
						Show_Str_Mid(0,120,"2.����������",16,240);
						num=GET_NUM();
						if(num==5){
						link();
						inspect();
						}
					return ;
				}else {processnum=0;ShowErrMessage(ensure);}					
				break;				
		}
		delay_ms(800);
		if(i==5)//����5��û�а���ָ���˳�
		{
			LCD_Fill(0,100,lcddev.width,160,WHITE);
			break;	
		}				
	}
}

//ˢָ��
void press_FR(void)
{
	SearchResult seach;
	u8 ensure;
	char *str;
	ensure=PS_GetImage();
	if(ensure==0x00)//��ȡͼ��ɹ� 
	{	
		ensure=PS_GenChar(CharBuffer1);
		if(ensure==0x00) //���������ɹ�
		{		
			ensure=PS_HighSpeedSearch(CharBuffer1,0,300,&seach);
			if(ensure==0x00)//�����ɹ�
			{				
				LCD_Fill(0,120,lcddev.width,160,WHITE);
				Show_Str_Mid(0,120,"ָ����֤�ɹ����ѿ�����",16,240);//����ָ�Ƴɹ�
				
				str=mymalloc(50);
				sprintf(str,"�û�����%d  ƥ���:%d",seach.pageID,seach.mathscore);//��ʾƥ��ָ�Ƶ�ID�ͷ���
			LED1=0;
        delay_ms(50000);
        delay_ms(50000);
        delay_ms(50000);
        LED1=~LED1;
				Show_Str_Mid(0,140,(u8*)str,16,240);
				myfree(str);
			}
			else 
				ShowErrMessage(ensure);					
	  }
		else
			ShowErrMessage(ensure);

	 delay_ms(6000);//��ʱ�������ʾ
	delay_ms(6000);//��ʱ�������ʾ
	delay_ms(6000);//��ʱ�������ʾ
	 LCD_Fill(0,120,lcddev.width,160,WHITE);
	}
		
}

//ɾ��ָ��
void Del_FR(void)
{
	u8  ensure;
	u16 num;
	LCD_Fill(0,100,lcddev.width,160,WHITE);
	Show_Str_Mid(0,100,"Delete fingerprint",16,240);//��ʾɾ��ָ��
	Show_Str_Mid(0,120,"Input ID and touch Enter!",16,240);//��ʾ����ID�����¡�Enter��
	Show_Str_Mid(0,140,"0=< ID <=299",16,240);
	delay_ms(50);
	AS608_load_keyboard(0,170,(u8**)kbd_delFR);
	num=GET_NUM();//��ȡ���ص���ֵ
	//printf ("num:%d",num);
	if(num==0xFFFF)
		goto MENU ; //������ҳ��
	else if(num==0xFF00)
		ensure=PS_Empty();//���ָ�ƿ�
	else 
		ensure=PS_DeletChar(num,1);//ɾ������ָ��
	if(ensure==0)
	{
		LCD_Fill(0,120,lcddev.width,160,WHITE);
		Show_Str_Mid(0,140,"ָ��ɾ���ɹ�!!!",16,240);//ɾ��ָ�Ƴɹ�		
	}
  else
		ShowErrMessage(ensure);	
	delay_ms(1500);//��ʱ�������ʾ
	PS_ValidTempleteNum(&ValidN);//����ָ�Ƹ���
	LCD_ShowNum(80,80,AS608Para.PS_max-ValidN,3,16);//��ʾʣ��ָ�Ƹ���
MENU:	
	LCD_Fill(0,100,lcddev.width,160,WHITE);
	delay_ms(50);
	AS608_load_keyboard(0,170,(u8**)kbd_menu);
	Show_Str_Mid(0,120,"2.����������",16,240);
	num=GET_NUM();
	if(num==5){
		link();
		inspect();
	}
}


