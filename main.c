#include <stdio.h>
#include "stm32f4xx.h"
#include "bmp180_i2c.h"
#include "uart.h"
#include "pll.h"
#include "stdbool.h"

extern uint32_t count;


void SysTick_Handler(void)
{	
	
	count++;
}

void yaz(const int *ptr)
{
 printf("%d",*ptr);
}


int main()
	
{
	
  long a=20;
	RCC_Config();
	USART_Config();
	SystemCoreClockUpdate();
	SysTick_Config(SystemCoreClock/1000);//1 mslik timer için.
	
	i2cislemler();
	yaz(&a);
	
	
while(1)
{
  
	
	i2cDongu();
	
}

}

