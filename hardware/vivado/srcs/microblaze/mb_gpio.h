/*
 * mb_gpio.h
 *
 *  Created on: 12 janv. 2016
 *      Author: Artur
 */

#ifndef MB_GPIO_H_
#define MB_GPIO_H_

//#include "xgpio.h"
//
//extern XGpio GpioInstancePtr0;
//extern XGpio_Config *GpioConfigPtr0;
//extern XGpio GpioInstancePtr1;
//extern XGpio_Config *GpioConfigPtr1;

int InitializeGpioSystem(void);
void GpioInterruptHandler(void);

#endif /* MB_GPIO_H_ */
