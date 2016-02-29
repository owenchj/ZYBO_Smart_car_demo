/*
 * mb_gpio.c


 *
 *  Created on: 12 janv. 2016
 *      Author: Artur
 */

#include "mb_gpio.h"
#include "mb_ultrasonic.h"
#include "mb_timer.h"
#include "mb_interrupt.h"

#include "xintc.h"

//XGpio GpioInstancePtr0;
//XGpio_Config *GpioConfigPtr0;
//XGpio GpioInstancePtr1;
//XGpio_Config *GpioConfigPtr1;

void GpioInterruptHandler(void){

//	// Set next state and stop the right timers
//	switch(UltrasonicNextState){
//	case WAIT_ECHO:
//			UltrasonicNextState = ECHO_ON;
//		break;
//	case ECHO_ON:
//			UltrasonicNextState = WAIT_CYCLE;
//
//			XTmrCtr_Stop(&TimerInstancePtr, 1);
//			break;
//	default:
//		break;
//	}
//
//	// Clear Interrupt
//	XGpio_InterruptClear(&GpioInstancePtr1, (u32) 0xFFFFFFFF);
//	XIntc_Acknowledge(&InterruptInstancePtr, XPAR_INTC_0_GPIO_0_VEC_ID);

}

int InitializeGpioSystem(void){

//	int Status;
//
////	GpioConfigPtr.BaseAddress = XPAR_GPIO_0_BASEADDR;
////	GpioConfigPtr.DeviceId = XPAR_GPIO_0_DEVICE_ID;
////	GpioConfigPtr.InterruptPresent = XPAR_GPIO_0_INTERRUPT_PRESENT;
////	GpioConfigPtr.IsDual = XPAR_GPIO_0_IS_DUAL;
//
//	GpioConfigPtr1 = XGpio_LookupConfig(XPAR_AXI_GPIO_0_DEVICE_ID);
//	if (GpioConfigPtr1 == NULL){
//		return XST_FAILURE;
//	}
//
//	Status = XGpio_CfgInitialize(&GpioInstancePtr1, GpioConfigPtr1, GpioConfigPtr1->BaseAddress);
//	if(Status != XST_SUCCESS){
//		return XST_FAILURE;
//	}
//
//	// Set all the OUTPUT GPIOs to output
//	XGpio_SetDataDirection(&GpioInstancePtr1, 1, (u32) 0x00000000);
//
//	// Set all the INPUT GPIOs to inputs
//	XGpio_SetDataDirection(&GpioInstancePtr1, 2, (u32) 0xFFFFFFFF);
//
//	// Set all OUTPUT GPIOs to 0
//	XGpio_DiscreteWrite(&GpioInstancePtr1, 1, (u32) 0x00000000);
//
//	// Enable Interrupt for Gpio System
//	XGpio_InterruptGlobalEnable(&GpioInstancePtr1);
//	XGpio_InterruptEnable(&GpioInstancePtr1, (u32) 0xFFFFFFFF);


	return XST_SUCCESS;

}
