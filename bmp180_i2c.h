
 
#include "stdint.h"


#ifndef BMP180_I2C__H
#define BMP180_I2C__H


void  i2cBasla(void);
void i2cYaz(uint8_t adres,uint8_t veri);
int I2C1_Oku (int adres);
void i2cislemler(void);
void i2cDongu(void);


#endif


















