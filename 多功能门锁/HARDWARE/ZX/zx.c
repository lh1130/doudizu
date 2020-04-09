#include "zx.h"
#include "lcd.h"
#include "key.h"
#include "touch.h"	  
#include "malloc.h"
#include "text.h"	
#include "rc522.h"
#include "usart3.h"
#include "led.h"
SysPara AS608Para;//指纹模块AS608参数
u16 ValidN;//模块内有效模板个数

u8** kbd_tbl;
u16 a[1]={111};//开锁密码
u16 b[1]={222};//管理员密码
u16 c[1]={333};
u8* q=NULL;



const  u8* kbd_menu[15]={"del_fp"," : ","add_fp","1","2","3","4","5","6","7","8","9","Cancle","0","Enter",};//按键表
const  u8* kbd_delFR[15]={"BACK"," : ","del_all","1","2","3","4","5","6","7","8","9","Cancle","0","DEL",};//按键表


//获取键盘数值
u16 GET_NUM(void)
{
	u8  key_num=0;
	u16 num=0;
	while(1)
	{
		key_num=AS608_get_keynum(0,170);	
		if(key_num)
		{
			if(key_num==1)return 0xFFFF;//‘返回’键
			if(key_num==3)return 0xFF00;//		
			if(key_num>3&&key_num<13&&num<99)//‘1-9’键(限制输入3位数)
				num =num*10+key_num-3;		
			if(key_num==13)num =num/10;//‘Del’键			
			if(key_num==14&&num<99)num =num*10;//‘0’键
			if(key_num==15)return num;  //‘Enter’键
		}
		LCD_ShowNum(80+15,170+7,num,6,16);
	}	
}
//验证指纹
void inspect(void){

	u8 key_num;

	while(1)
	{
		key_num=AS608_get_keynum(0,170);	
		if(key_num)
		{
			if(key_num==1)Del_FR();		//删指纹
			if(key_num==3)Add_FR();		//录指纹
			if(key_num==5)linkA();		// 返回主界面			
		}
		if(PS_Sta)	 //检测PS_Sta状态，如果有手指按下
		{
			press_FR();//刷指纹			
		}				 
	} 	
}
//显示界面，连接功能
void link(void){

	u8 ensure;
	char *str;
	
	
	while(font_init()) 			//检查字库
	{	    
		LCD_ShowString(60,50,240,16,16,"Font Error!");
		delay_ms(200);				  
		LCD_Fill(60,50,240,66,WHITE);//清除显示
		delay_ms(200);		
	}
	/*加载指纹识别实验界面*/
	LCD_Clear(WHITE);
	POINT_COLOR=BLACK;
	Show_Str_Mid(0,0,"指纹库连接成功",16,240);	    			    	 
	Show_Str_Mid(0,20,"指纹管理",16,240);				    	 
 	POINT_COLOR=BLACK;
	Show_Str_Mid(0,40,"Connect with AS608....",16,240);	
	while(PS_HandShake(&AS608Addr))//与AS608模块握手
	{
		LCD_Fill(0,40,240,80,WHITE);
		Show_Str_Mid(0,40,"Cannot connect with AS608!",16,240);
		delay_ms(1000);
		LCD_Fill(0,40,240,80,WHITE);
		Show_Str_Mid(0,40,"Try to connect again....",16,240);	//Try to connect again
		delay_ms(1000);	  
	}
	LCD_Fill(0,40,240,320,WHITE);
	Show_Str_Mid(0,40,"在虚拟键盘上进行操作!",16,240);//通讯成功
	Show_Str_Mid(0,120,"2.返回主界面",16,240);
	
	str=mymalloc(30);
	sprintf(str,"Baudrate:%d   Addr:%x",usart2_baund,AS608Addr);//显示波特率
	//Show_Str(0,60,240,16,(u8*)str,16,0);
	delay_ms(100);
	ensure=PS_ValidTempleteNum(&ValidN);//读库指纹个数
	if(ensure!=0x00)
		ShowErrMessage(ensure);//显示确认码错误信息	
	ensure=PS_ReadSysPara(&AS608Para);  //读AS608模块参数 
	if(ensure==0x00)
	{
		mymemset(str,0,50);
		sprintf(str,"RemainNum:%d    Level:%d",AS608Para.PS_max-ValidN,AS608Para.PS_level);//显示剩余指纹数量和安全等级
		Show_Str(0,60,240,16,(u8*)str,16,0);
	}
	else
		ShowErrMessage(ensure);	
	myfree(str);
	AS608_load_keyboard(0,170,(u8**)kbd_menu);//加载虚拟键盘
}


void passKS(void){
	u8 i=000;
	u16 Num[1];
	Num[0]=a[0];
	while(1){
		Show_Str_Mid(0,120,"请输入密码！",16,240);
		i=GET_NUM();//获取返回的数值
		if(i!=000){
			if(Num[0]==i){
				LCD_Fill(0,120,lcddev.width,160,WHITE);
				Show_Str_Mid(0,120,"密码输入正确，已开锁！",16,240);//	
					LED1=0;
					delay_ms(50000);
					delay_ms(50000);
					delay_ms(50000);
					LED1=~LED1;
				delay_ms(6000);//延时后清除显示
				delay_ms(6000);//延时后清除显示
				delay_ms(6000);//延时后清除显示
			
				LCD_Fill(0,120,lcddev.width,140,WHITE);				
				
			}else{
				LCD_Fill(0,120,lcddev.width,160,WHITE);
				Show_Str_Mid(0,120,"密码输入错误，请重新操作！",16,240);//	
			Show_Str_Mid(0,140,"000为无效密码！",16,240);
			delay_ms(6000);//延时后清除显示
			delay_ms(6000);//延时后清除显示
			delay_ms(6000);//延时后清除显示
			
			LCD_Fill(0,120,lcddev.width,160,WHITE);
				
			


			
				
				
			}
		}
		
		break;
	}
	
}
void XGGpas(u16 a[],u16 b[],u8* p){
	u16 num=0;
	u16 num1=1;
	
	

	Show_Str_Mid(0,120,"请输入新管理员密码！",16,240);
	num=GET_NUM();
	delay_ms(6000);//延时后清除显示
	delay_ms(6000);//延时后清除显示
	delay_ms(6000);//延时后清除显示
	
	LCD_Fill(0,120,lcddev.width,140,WHITE);
	
	if(num!=000){
		num1=num;
		Show_Str_Mid(0,120,"请再输一次！",16,240);
		num=GET_NUM();
		delay_ms(6000);//延时后清除显示
		delay_ms(6000);//延时后清除显示
		delay_ms(6000);//延时后清除显示
	
		LCD_Fill(0,120,lcddev.width,140,WHITE);
		if(num==num1){
			printf("b1:%d",num);
			b[0]=num;
			printf("b2:%d",b[0]);
			Show_Str_Mid(0,120,"管理员密码修改成功！",16,240);
			delay_ms(6000);//延时后清除显示
			delay_ms(6000);//延时后清除显示
			delay_ms(6000);//延时后清除显示
			
			LCD_Fill(0,120,lcddev.width,140,WHITE);
			linkA();
		}else{
		Show_Str_Mid(0,120,"两次密码不一致！",16,240);
		delay_ms(6000);//延时后清除显示
		delay_ms(6000);//延时后清除显示
		delay_ms(6000);//延时后清除显示
		LCD_Fill(0,120,lcddev.width,140,WHITE);
		Administrator(a,b,q);
		}
	}else{
		Show_Str_Mid(0,120,"新密码不可为000！",16,240);
		delay_ms(6000);//延时后清除显示
		delay_ms(6000);//延时后清除显示
		delay_ms(6000);//延时后清除显示
	
		LCD_Fill(0,120,lcddev.width,140,WHITE);
		printf("b3:%d",b[0]);
		Administrator(a,b,q);
	}
}

void GLkspas(u16 a[],u16 b[],u8* p){
	u16 num=0;
	u16 num1=1;
	
	
	
	Show_Str_Mid(0,120,"请输入新开锁密码！",16,240);
	num=GET_NUM();
	delay_ms(6000);//延时后清除显示
	delay_ms(6000);//延时后清除显示
	delay_ms(6000);//延时后清除显示
	
	LCD_Fill(0,120,lcddev.width,140,WHITE);
	
	if(num!=000){
		num1=num;
		Show_Str_Mid(0,120,"请再输一次！",16,240);
		num=GET_NUM();
		delay_ms(6000);//延时后清除显示
		delay_ms(6000);//延时后清除显示
		delay_ms(6000);//延时后清除显示
	
		LCD_Fill(0,120,lcddev.width,140,WHITE);
		if(num==num1){
			//printf("b1:%d",num);
			a[0]=num;
			//printf("b2:%d",b[0]);
			Show_Str_Mid(0,120,"开锁密码修改成功！",16,240);
			delay_ms(6000);//延时后清除显示
			delay_ms(6000);//延时后清除显示
			delay_ms(6000);//延时后清除显示
	
			LCD_Fill(0,120,lcddev.width,140,WHITE);
			//Administrator(a,b,c);
			linkA();
		}else{
		Show_Str_Mid(0,120,"两次密码不一致！",16,240);
		delay_ms(6000);//延时后清除显示
		delay_ms(6000);//延时后清除显示
		LCD_Fill(0,120,lcddev.width,140,WHITE);
		Administrator(a,b,q);
		}
	}else{
		Show_Str_Mid(0,120,"新密码不可为000！",16,240);
		delay_ms(6000);//延时后清除显示
		delay_ms(6000);//延时后清除显示
		delay_ms(6000);//延时后清除显示
	
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
		
			Show_Str_Mid(0,120,"ID卡添加成功！",16,240);
			delay_ms(6000);//延时后清除显示
			delay_ms(6000);//延时后清除显示
			delay_ms(6000);//延时后清除显示
			LCD_Fill(0,120,lcddev.width,140,WHITE);
		
		 delay_ms(300);
		
		break;
    }
	//return q;
}
void CZID(u8* p){
	q=NULL;
	if(q==NULL){
			Show_Str_Mid(0,120,"ID卡重置成功！",16,240);
			delay_ms(6000);//延时后清除显示
			delay_ms(6000);//延时后清除显示
			delay_ms(6000);//延时后清除显示
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
	u8* o;//接受现刷，p是存
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
	Show_Str_Mid(0,120,"验证失败！",16,240);
	delay_ms(6000);//延时后清除显示
	delay_ms(6000);//延时后清除显示
	LCD_Fill(0,120,lcddev.width,140,WHITE);
			delay_ms(3000);
		}else{
			printf ("SC\r\n");
	Show_Str_Mid(0,120,"验证成功，已开锁！",16,240);
	delay_ms(6000);//延时后清除显示
	delay_ms(6000);//延时后清除显示
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
		printf(" 请放入ID卡\r\n" );
			Show_Str_Mid(0,120,"请放入ID卡！",16,240);
	delay_ms(6000);//延时后清除显示
	delay_ms(6000);//延时后清除显示
	LCD_Fill(0,120,lcddev.width,140,WHITE);
	}


		

//	printf("ID1>>>%s\r\n", q);
	

}
void UIID(u8*p){
	u8 key;
	char *str;
	LCD_Clear(WHITE);
	POINT_COLOR=BLACK;
	Show_Str_Mid(0,0,"ID卡管理！",16,240);
	Show_Str_Mid(0,40,"1.添加ID卡！..",16,240);
	Show_Str_Mid(0,60,"2.重置ID卡！..",16,240);
	Show_Str_Mid(0,80,"3.返回主页面！",16,240);
	myfree(str);
	AS608_load_keyboard(0,170,(u8**)kbd_menu);//加载虚拟键盘
	
	Show_Str_Mid(0,120,"请选择！",16,240);
	delay_ms(6000);//延时后清除显示
	delay_ms(6000);//延时后清除显示
	LCD_Fill(0,120,lcddev.width,140,WHITE);
	
	while(1){
		key=GET_NUM();
		if(key==1){
			//添加ID卡
			addID(q);
	}else if(key==2){
		//重置ID卡
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
	Show_Str_Mid(0,120,"请输入管理员密码！",16,240);
	printf("b:%d",b[0]);
	delay_ms(6000);//延时后清除显示
	delay_ms(6000);//延时后清除显示
	delay_ms(6000);//延时后清除显示
	LCD_Fill(0,120,lcddev.width,140,WHITE);
	numG =GET_NUM ();
	if(b[0]==numG){
	while(font_init()) 			//检查字库
	{	    
		LCD_ShowString(60,50,240,16,16,"Font Error!");
		delay_ms(200);				  
		LCD_Fill(60,50,240,66,WHITE);//清除显示
		delay_ms(200);		
	}
	/*加载指纹识别实验界面*/
	LCD_Clear(WHITE);
	POINT_COLOR=BLACK;
	Show_Str_Mid(0,0,"已有管理员权限，请谨慎修改！",16,240);	    			    	 
	Show_Str_Mid(0,20,"1.管理开锁密码...",16,240);				    	 
 	POINT_COLOR=BLACK;
	Show_Str_Mid(0,40,"2.管理指纹......",16,240);
	Show_Str_Mid(0,60,"3.管理ID卡......",16,240);
	Show_Str_Mid(0,80,"4.修改管理员密码",16,240);
	Show_Str_Mid(0,100,"5.退出管理员权限",16,240);
	myfree(str);
	AS608_load_keyboard(0,170,(u8**)kbd_menu);//加载虚拟键盘
	while(1){
		num=GET_NUM();
	if(num==1){
		//管理开锁密码
		GLkspas(a,b,q);
	}else if(num==2){
		//管理指纹
		link();
		inspect();
	}else if(num==3){
		//管理ID卡
		UIID(q);
		//break;
	}else if(num==4){
		//修改管理员密码
		XGGpas(a,b,q);
	}else if(num==5){
		//退回主界面
		linkA();
	
	}	
	
	}
	}else{
			Show_Str_Mid(0,120,"管理员密码输入错误！",16,240);
			delay_ms(6000);//延时后清除显示
			delay_ms(6000);//延时后清除显示
			LCD_Fill(0,120,lcddev.width,140,WHITE);
			}
 
}

 

 
//显示主界面
void linkA(void){
	char *str;
	u16 num;
	u8 i=0;
	u8 reclen=0;
	USART3_RX_STA=0;


	while(font_init()) 			//检查字库
	{	    
		LCD_ShowString(60,50,240,16,16,"Font Error!");
		delay_ms(200);				  
		LCD_Fill(60,50,240,66,WHITE);//清除显示
		delay_ms(200);		
	}
	/*加载指纹识别实验界面*/
	LCD_Clear(WHITE);
	POINT_COLOR=BLACK;
	Show_Str_Mid(0,0,"欢迎回家，输入门禁！",16,240);	    			    	 
	Show_Str_Mid(0,20,"请选择~",16,240);				    	 
 	POINT_COLOR=BLACK;
	Show_Str_Mid(0,40,"1.密码开锁..",16,240);
	Show_Str_Mid(0,60,"2.指纹开锁..",16,240);
	Show_Str_Mid(0,80,"3.ID卡开锁..",16,240);
    Show_Str_Mid(0,100,"4.管理员模式",16,240);
	myfree(str);
	AS608_load_keyboard(0,170,(u8**)kbd_menu);//加载虚拟键盘
	
	while(1){
//			u8 t;
//			t=KEY_Scan(1);
		
			tp_dev.scan(0); 
		if(tp_dev.sta&TP_PRES_DOWN){
		num=GET_NUM();//获取返回的数值
		if(num==1){
			passKS();//密码开锁
			//break;
		}else if(num==2){//指纹开锁
			
			for(i=0;i<5;i++){
				if(PS_Sta){	 //检测PS_Sta状态，如果有手指按下
					press_FR();//刷指纹		
					break;
				}
			
			}
				
		}else if(num==3){
				//ID卡开锁
			IDpas(q);
		}else if(num==4){
			 
			Administrator(a,b,q);
 
			// 管理员模式
		}else {
			Show_Str_Mid(0,120,"请重新输入！",16,240);
			delay_ms(6000);//延时后清除显示
			delay_ms(6000);//延时后清除显示
			delay_ms(6000);//延时后清除显示
			 LCD_Fill(0,120,lcddev.width,140,WHITE);
			
		}
			  
			  
			//printf("123\r\n");
			  delay_ms(100);	
			
			
			
			printf("123\r\n");
			delay_ms(500);
		}else{
		if(USART3_RX_STA&0X8000)			//接收到一次数据了
		{
 			reclen=USART3_RX_STA&0X7FFF;	//得到数据长度
		  USART3_RX_BUF[reclen]=0;	 	//加入结束符

			if(USART3_RX_BUF[0]=='1'){
				printf("145\r\n");
 			printf("%s\r\n",USART3_RX_BUF);//显示接收到的数据
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





//加载按键界面（尺寸x,y为240*150）
//x,y:界面起始坐标（240*320分辨率的时候，x必须为0）
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
		if(i==1)//按键表第2个‘:’不需要中间显示
			Show_Str(x+(i%3)*80+2,y+7+30*(i/3),80,30,(u8*)kbd_tbl[i],16,0);	
		else
			Show_Str_Mid(x+(i%3)*80,y+7+30*(i/3),(u8*)kbd_tbl[i],16,80);
	} 
}


//按键状态设置
//x,y:键盘坐标
//key:键值(0~14)
//sta:状态，0，松开；1，按下；
void AS608_key_staset(u16 x,u16 y,u8 keyx,u8 sta)
{		  
	u16 i=keyx/3,j=keyx%3;
	if(keyx>16)return;
	if(sta &&keyx!=1)//按键表第2个‘:’不需要清除
		LCD_Fill(x+j*80+1,y+i*30+1,x+j*80+78,y+i*30+28,GREEN);
	else if(keyx!=1)
		LCD_Fill(x+j*80+1,y+i*30+1,x+j*80+78,y+i*30+28,WHITE);
	if(keyx!=1)//不是‘：’
		Show_Str_Mid(x+j*80,y+7+30*i,(u8*)kbd_tbl[keyx],16,80);	
}
//得到触摸屏的输入
//x,y:键盘坐标
//返回值:（1~15,对应按键表）
u8 AS608_get_keynum(u16 x,u16 y)
{
	u16 i,j;
	static u8 key_x=0;//0,没有任何按键按下
	u8 key=0;
	tp_dev.scan(0); 		 
	if(tp_dev.sta&TP_PRES_DOWN)//触摸屏被按下
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

//显示确认码错误信息
void ShowErrMessage(u8 ensure)
{
	LCD_Fill(0,120,lcddev.width,160,WHITE);
	Show_Str_Mid(0,120,(u8*)EnsureMessage(ensure),16,240);
}



//指纹操作
//录指纹
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
				Show_Str_Mid(0,100,"Pleas touch finger!",16,240);//请按手指
				ensure=PS_GetImage();
				if(ensure==0x00) 
				{
					ensure=PS_GenChar(CharBuffer1);//生成特征
					if(ensure==0x00)
					{
						LCD_Fill(0,120,lcddev.width,160,WHITE);
						Show_Str_Mid(0,120,"Fingerprint correct",16,240);//指纹正确
						i=0;
						processnum=1;//跳到第二步						
					}else ShowErrMessage(ensure);				
				}else ShowErrMessage(ensure);						
				break;
			
			case 1:
				i++;
				LCD_Fill(0,100,lcddev.width,160,WHITE);
				Show_Str_Mid(0,100,"Pleas touch finger again!",16,240);//再按一次手指
				ensure=PS_GetImage();
				if(ensure==0x00) 
				{
					ensure=PS_GenChar(CharBuffer2);//生成特征			
					if(ensure==0x00)
					{
						LCD_Fill(0,120,lcddev.width,160,WHITE);
						Show_Str_Mid(0,120,"Fingerprint correct",16,240);//指纹正确
						i=0;
						processnum=2;//跳到第三步
					}else ShowErrMessage(ensure);	
				}else ShowErrMessage(ensure);		
				break;

			case 2:
				LCD_Fill(0,100,lcddev.width,160,WHITE);
				Show_Str_Mid(0,100,"Compare twice fingerprint",16,240);//对比两次指纹
				ensure=PS_Match();
				if(ensure==0x00) 
				{
					LCD_Fill(0,120,lcddev.width,160,WHITE);
					Show_Str_Mid(0,120,"Twice fingerprint are same",16,240);//两次指纹是一样的
					processnum=3;//跳到第四步
				}
				else 
				{
					LCD_Fill(0,100,lcddev.width,160,WHITE);
					Show_Str_Mid(0,100,"Compare fail,pleas touch again!",16,240);//对比失败，请重新按手指
					ShowErrMessage(ensure);
					i=0;
					processnum=0;//跳回第一步		
				}
				delay_ms(1000);
				break;

			case 3:
				LCD_Fill(0,100,lcddev.width,160,WHITE);
				Show_Str_Mid(0,100,"Generate fingerprint template",16,240);//产生一个指纹模板
				ensure=PS_RegModel();
				if(ensure==0x00) 
				{
					LCD_Fill(0,120,lcddev.width,160,WHITE);
					Show_Str_Mid(0,120,"Generate fingerprint success",16,240);//生成指纹模板成功
					processnum=4;//跳到第五步
				}else {processnum=0;ShowErrMessage(ensure);}
				delay_ms(1000);
				break;
				
			case 4:	
				LCD_Fill(0,100,lcddev.width,160,WHITE);
				Show_Str_Mid(0,100,"Intput ID and save with ENTER!",16,240);//输入ID并按“Enter”保存
				Show_Str_Mid(0,120,"0=< ID <=299",16,240);	
				do
					ID=GET_NUM();
				while(!(ID<300));//输入DI必须小于300
				ensure=PS_StoreChar(CharBuffer2,ID);//储存模板
				if(ensure==0x00) 
				{			
					LCD_Fill(0,100,lcddev.width,160,WHITE);					
					Show_Str_Mid(0,120,"Add fingerprint success!!!",16,240);//添加指纹成功
					PS_ValidTempleteNum(&ValidN);//读库指纹个数
					LCD_ShowNum(80,80,AS608Para.PS_max-ValidN,3,16);//显示剩余指纹个数
					delay_ms(1500);//延时后清除显示	
					LCD_Fill(0,100,240,160,WHITE);
						Show_Str_Mid(0,120,"2.返回主界面",16,240);
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
		if(i==5)//超过5次没有按手指则退出
		{
			LCD_Fill(0,100,lcddev.width,160,WHITE);
			break;	
		}				
	}
}

//刷指纹
void press_FR(void)
{
	SearchResult seach;
	u8 ensure;
	char *str;
	ensure=PS_GetImage();
	if(ensure==0x00)//获取图像成功 
	{	
		ensure=PS_GenChar(CharBuffer1);
		if(ensure==0x00) //生成特征成功
		{		
			ensure=PS_HighSpeedSearch(CharBuffer1,0,300,&seach);
			if(ensure==0x00)//搜索成功
			{				
				LCD_Fill(0,120,lcddev.width,160,WHITE);
				Show_Str_Mid(0,120,"指纹验证成功，已开锁！",16,240);//搜索指纹成功
				
				str=mymalloc(50);
				sprintf(str,"用户名：%d  匹配度:%d",seach.pageID,seach.mathscore);//显示匹配指纹的ID和分数
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

	 delay_ms(6000);//延时后清除显示
	delay_ms(6000);//延时后清除显示
	delay_ms(6000);//延时后清除显示
	 LCD_Fill(0,120,lcddev.width,160,WHITE);
	}
		
}

//删除指纹
void Del_FR(void)
{
	u8  ensure;
	u16 num;
	LCD_Fill(0,100,lcddev.width,160,WHITE);
	Show_Str_Mid(0,100,"Delete fingerprint",16,240);//显示删除指纹
	Show_Str_Mid(0,120,"Input ID and touch Enter!",16,240);//显示输入ID并按下“Enter”
	Show_Str_Mid(0,140,"0=< ID <=299",16,240);
	delay_ms(50);
	AS608_load_keyboard(0,170,(u8**)kbd_delFR);
	num=GET_NUM();//获取返回的数值
	//printf ("num:%d",num);
	if(num==0xFFFF)
		goto MENU ; //返回主页面
	else if(num==0xFF00)
		ensure=PS_Empty();//清空指纹库
	else 
		ensure=PS_DeletChar(num,1);//删除单个指纹
	if(ensure==0)
	{
		LCD_Fill(0,120,lcddev.width,160,WHITE);
		Show_Str_Mid(0,140,"指纹删除成功!!!",16,240);//删除指纹成功		
	}
  else
		ShowErrMessage(ensure);	
	delay_ms(1500);//延时后清除显示
	PS_ValidTempleteNum(&ValidN);//读库指纹个数
	LCD_ShowNum(80,80,AS608Para.PS_max-ValidN,3,16);//显示剩余指纹个数
MENU:	
	LCD_Fill(0,100,lcddev.width,160,WHITE);
	delay_ms(50);
	AS608_load_keyboard(0,170,(u8**)kbd_menu);
	Show_Str_Mid(0,120,"2.返回主界面",16,240);
	num=GET_NUM();
	if(num==5){
		link();
		inspect();
	}
}


