#include "s5pv210.h"

#define 	GPJ2CON 	(*(volatile unsigned long *) 0xE0200280)
#define 	GPJ2DAT		(*(volatile unsigned long *) 0xE0200284)

// —” ±∫Ø ˝
void delay(unsigned long count)
{
	volatile unsigned long i = count;
	while (i--)
		;
}

void Led_Blink()				//LED …¡À∏
{
	GPJ2CON = 0x00001111;		// ≈‰÷√“˝Ω≈
	while(1)					// …¡À∏
	{
		GPJ2DAT = 0;			// LED on
		delay(0x1000000);
		GPJ2DAT = 0xf;			// LED off
		delay(0x1000000);
	}
}

void Led_On()				
{
	GPJ2CON = 0x00001111;		
	GPJ2DAT = 0;			
}

void Led_OFF()				
{
	GPJ2CON = 0x00001111;		
	GPJ2DAT = 0xf;			
}


