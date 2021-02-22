

#include "stm32f4xx.h"  
#include "uart.h"


void USART_Config()
{
	
	//PB10 ve PB11 aktif edildi.
	RCC->AHB1ENR  |= 0x00000002;		// GPIOB Aktif
	GPIOB->MODER  |= (1 << 21) | (1 << 23);		// AF PB10 AND PB11
	GPIOB->AFR[1] |= (1 << 8) | (1 << 9) |(1<<10);// PB10  AF7 (USART3)
	
	
	//USART Hatti.
	
	RCC->APB1ENR |= 1 << 18;//Uart Hatti aktif
	//USART3->CR1 |= 1 << 2;		// Rx aktif
	USART3->CR1 |= 1 << 3;		// Tx aktif
	USART3->CR1 &= ~(1 << 10);		// Parity pasif
	USART3->CR1 &= ~(1 << 12);		// 8bit
	USART3->CR1 &= ~(1<< 15);		//oversampling by 16
	USART3->CR2 &= ~(1 << 12);		// Stop bit 1
	USART3->CR2 &= ~(1 << 13);		// Stop bit 1
	USART3->BRR = 0x1117;		// BaudRate 9600
	USART3->CR1 |= 1 << 13;		// Usart aktif
}

void veri_yolla(int a)
{
	 USART3->DR=a;
   while(!(USART3->SR&(1<<6)));  // TX Buffer dolu ise bekle (Rehber Sayfa 646)
 
}

