#include "int.h"
#include "210io.h"
#include "ucos_ii.h"
#include "lcd.h"


#define 	GPJ2CON 	(*(volatile unsigned long *) 0xE0200280)
#define 	GPJ2DAT		(*(volatile unsigned long *) 0xE0200284)
#define 	GPH2CON 	(*(volatile unsigned long *) 0xE0200C40)
#define 	GPH2DAT		(*(volatile unsigned long *) 0xE0200C44)

#define 	GPJ2_0_OUTPUT 	(1<<(0*4))
#define 	GPJ2_1_OUTPUT 	(1<<(1*4))
#define 	GPJ2_2_OUTPUT 	(1<<(2*4))
#define 	GPJ2_3_OUTPUT 	(1<<(3*4))

#define 	GPH2_0_EINT16 	(0xf<<(0*4))
#define 	GPH2_1_EINT17 	(0xf<<(1*4))
#define 	GPH2_2_EINT18 	(0xf<<(2*4))
#define 	GPH2_3_EINT19 	(0xf<<(3*4))

#define		EXT_INT_0_CON  			( *((volatile unsigned long *)0xE0200E00) )
#define		EXT_INT_1_CON  			( *((volatile unsigned long *)0xE0200E04) )
#define		EXT_INT_2_CON  			( *((volatile unsigned long *)0xE0200E08) )
#define		EXT_INT_3_CON  			( *((volatile unsigned long *)0xE0200E0C) )

#define		EXT_INT_0_MASK   		( *((volatile unsigned long *)0xE0200F00) )
#define		EXT_INT_1_MASK   		( *((volatile unsigned long *)0xE0200F04) )
#define		EXT_INT_2_MASK   		( *((volatile unsigned long *)0xE0200F08) )
#define		EXT_INT_3_MASK   		( *((volatile unsigned long *)0xE0200F0C) )

#define		EXT_INT_0_PEND   		( *((volatile unsigned long *)0xE0200F40) )
#define		EXT_INT_1_PEND   		( *((volatile unsigned long *)0xE0200F44) )
#define		EXT_INT_2_PEND   		( *((volatile unsigned long *)0xE0200F48) )
#define		EXT_INT_3_PEND   		( *((volatile unsigned long *)0xE0200F4C) )
#define		lcdtype					0x3
void uart_init();
void timer_request(void);
void timer_init(unsigned long utimer,unsigned long uprescaler,unsigned long udivider,unsigned long utcntb,unsigned long utcmpb);
int command_do(int argc, char * argv[]);
int shell_parse(char * buf, char * argv[]);
char * gets(char * s);
extern unsigned char *gImage_bmp;

void TestTask(void *pdata)
{
	INT32U time_cnt=0;
	printf("Task begin\r\n");
	while(1)
	{	
		if(time_cnt%2==0)
		{
			Led_On();
		}else
		{
			Led_OFF();
		}
		OSTimeDly(OS_TICKS_PER_SEC);
		printf("task1 has run %d s\r\n",time_cnt);
		++time_cnt;
	}
}

void TaskShell(void *pdata)
{
	char buf[64];
	int argc = 0;
	char * argv[10];

	while (1)
	{
		printf("mini210s: ");
		gets(buf);								// 等待用户输入命令

		argc = shell_parse(buf, argv);			// 解析命令

		command_do(argc, argv);				// 运行命令
	}
}

OS_STK stk[1024];
OS_STK stk2[1024];

int main(void)
{	

	Led_On();
	// 初始化串口
	uart_init();
	
	// 中断相关初始化
	system_initexception();					
	// 设置timer
	timer_request();
	Lcd_Init(lcdtype);
	OSInit();
	lcd_clear_screen(0x0);
	Lcd_Draw_Bmp(gImage_bmp,lcdtype);
	
	printf("UCOS2 init finished\r\n");
	lcd_draw_char('U');
	lcd_draw_char('C');
	lcd_draw_char('O');
	lcd_draw_char('S');
	timer_init(0,65,4,625,0);
	OSTaskCreate(TestTask,NULL,&stk[sizeof(stk)-1],0);
	//OSTaskCreate(TaskShell,NULL,&stk2[sizeof(stk)-1],1);
	OSStart();
}
