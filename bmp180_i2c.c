
#include "degiskenler.h"
#include "bmp180_i2c.h"
#include "uart.h"

#define BMP180_WRITE 0xEE
#define BMP180_READ  0xEF


//CS ==>VDD
//I2C1 ==>APB1 
//PB6 ==>SCL
//PB7 ==>SDA
//AF4

//OTYPER OPEN-DRAIN SCL ve SDA
//MODER 10

//1/apb hatti.=That  ==> (1/100khz==10us/2)==>5us ==>5us/That
//1000NS/(1/APB) +1 ==>trise

void delay(uint32_t sure)
{
	
	count=0;
	while(count<sure);
}
void i2cDongu()
{

  i2cYaz(0xF4,0x2E);//Datasheet Sayfa 15
	delay(5);//4.5ms
	UT=(I2C1_Oku(0xF6)<<8)+I2C1_Oku(0xF7);
	
	
	i2cYaz(0xF4,(0x34+(1<<6))); //Oss 1 
	delay(8);//7.5 ms
	UP=(((I2C1_Oku(0xF6)<<16)+(I2C1_Oku(0xF7)<<8)+(I2C1_Oku(0xF8)))>>(8-1));
	X1=((UT-AC6)*AC5)/32768;
	X2=(MC*2048)/(X1+MD);
	B5=X1+X2;
	T=(B5+8)/16;
	T=T/10;//Gerçek sicaklik degeri
	
	
	veri_yolla(T);
	delay(1000);
	
	
	
	/*
	B6=B5-4000;
	X1=(B2*(B6*B6/4096))/2048;
	X2=AC2*B6/2048;
	X3=X1+X2;
	B3=(((AC1*4+X3)<<1)+2)/4;
	X1=AC3*B6/8192;
	X2=(B1*(B6*B6/4096))/65536;
	X3=((X1+X2)+2)/4;
	B4=AC4*(unsigned long)(X3+32768)/32768;
	B7=((unsigned long)UP-B3)*(50000>>1);
	
	if(B7<0x80000000)
		p=(B7*2)/B4;
	else
		p=(B7/B4) *2;
	
	X1=(p/256) *(p/256);
	X1=(X1*3038)/65536;
	X2=(-7357*p)/65536;
	p=p+(X1+X2+3791)/16;  //Birimi Pa
	*/
	
	
		


}

void hazirla()
{

	while(donenAdres!=0x55)  //Cihaz Adresi.
	{
	donenAdres=I2C1_Oku(0xD0);
	}
	

}
void  i2cBasla()
	
{

RCC->AHB1ENR |= 0x00000002; //GPIOB aktif..
RCC->APB1ENR |= 0x00200000; //I2C1  aktif.
GPIOB->AFR[0]= 0x44000000; //PB7 AF4 ayari ve PB6 AF4 ayari.
GPIOB->MODER |= 0x0000A000; //PB7 ve PB6 Alter.Function
GPIOB->OTYPER =0x000000C0; //PB7 ve PB6 Open-Drain.
	
I2C1->CR2 = 0x0008 ; //8mhz APB1 hat.PLL yok.
I2C1->CCR = 0x0028;  //i2c 100khz.
I2C1->TRISE=0x09;// Maximum yukselme zamani(Datasheette yaziyor) 
I2C1->CR1=0x0001;// I2C aktif.
}

void i2cYaz(uint8_t adres,uint8_t veri)
{
 I2C1->CR1 |= 0x0100; // START biti
 while (!(I2C1->SR1 & 0x0001)); // SB=1 olmasini bekle.
 I2C1->DR = BMP180_WRITE; // Slave adresi.
 while (!(I2C1->SR1 & 0x0002)); // ADDR=1 olmasini bekle.
 while (!(I2C1->SR2 & 0x0001));  //Master
 int Status2 = I2C1->SR2; // Bayrak temizle.
 I2C1->DR = adres; // Register adresi.
 while (!(I2C1->SR1 & 0x0080)); // TXE=1 olmasini bekle.
 I2C1->DR = veri; //Veriyi gonder.
 while (!(I2C1->SR1 & 0x0080)); // TXE=1 olmasini bekle.
 while (!(I2C1->SR1 & 0x0004)); // BTF=1 olmasini bekle.
 I2C1->CR1 |= 0x0200; // STOP biti.
 
	
}

int I2C1_Oku(int adres) { 

	
 I2C1->CR1 |= 0x0100; // START biti.
 while (!(I2C1->SR1 & 0x0001)) {}; // SB=1 olmasini bekle.
 I2C1->DR = BMP180_WRITE; // Slave Adresi.(Yazma)
 while (!(I2C1->SR1 & 0x0002)); // ADDR=1 olmasini bekle.
 int temp = I2C1->SR2; // Bayrak temizle.
 I2C1->DR = adres; // Register adresi.
 while (!(I2C1->SR1 & 0x0080)) {}; //TXE=1 olmasini bekle.
 while (!(I2C1->SR1 & 0x0004)) {}; // BTF=1 olmasini bekle.

 I2C1->CR1 |= 0x0100; // START biti.
 while (!(I2C1->SR1 & 0x0001)) {}; // SB=1 olmasini bekle.
 I2C1->DR = BMP180_READ; // Slave Adresi.(Okuma)
 while (!(I2C1->SR1 & 0x0002)) {}; // ADDR=1 olmasini bekle.
 temp= I2C1->SR2; // Bayrak temizle.
 	 
 while (!(I2C1->SR1 & 0x0040)) {}; // RxNE=1 olmasini bekle.while dmatcif 
 I2C1->CR1 |= 0x0200; // STOP biti.

 return I2C1->DR; // gelen veriyi al. 
	 
}
void i2cislemler()
{

  i2cBasla();
	hazirla();
	AC1=256*I2C1_Oku(0xAA)+I2C1_Oku(0xAB);
	AC2=256*I2C1_Oku(0xAC)+I2C1_Oku(0xAD);
	AC3=256*I2C1_Oku(0xAE)+I2C1_Oku(0xAF);
	AC4=256*I2C1_Oku(0xB0)+I2C1_Oku(0xB1);
	AC5=256*I2C1_Oku(0xB2)+I2C1_Oku(0xB3);
	AC6=256*I2C1_Oku(0xB4)+I2C1_Oku(0xB5);
	B1=256*I2C1_Oku(0xB6)+I2C1_Oku(0xB7);
	B2=256*I2C1_Oku(0xB8)+I2C1_Oku(0xB9);
	MB=256*I2C1_Oku(0xBA)+I2C1_Oku(0xBB);
	MC=256*I2C1_Oku(0xBC)+I2C1_Oku(0xBD);
	MD=256*I2C1_Oku(0xBE)+I2C1_Oku(0xBF);

}








