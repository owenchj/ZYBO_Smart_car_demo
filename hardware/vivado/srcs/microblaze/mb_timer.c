/*
 * mb_timer.c

 *
 *  Created on: 12 janv. 2016
 *      Author: Artur
 */

#include "mb_timer.h"
#include "mb_ultrasonic.h"
#include "mb_interrupt.h"

#include "xparameters.h"

//XTmrCtr TimerInstancePtr;
//XTmrCtr_Config *TimerConfigPtr;

void TimerInterruptHandler(void){

//	// Set next state and stop the right timers
//	switch(UltrasonicNextState){
//	case TRIGGER_ON:
//		UltrasonicNextState = WAIT_ECHO;
//
//		XTmrCtr_Stop(&TimerInstancePtr, 0);
//		XTmrCtr_Reset(&TimerInstancePtr, 0);
//
//		break;
//	default:
//		UltrasonicNextState = TRIGGER_ON;
//
//		XTmrCtr_Stop(&TimerInstancePtr, 0);
//		XTmrCtr_Stop(&TimerInstancePtr, 1);
//
//		XTmrCtr_Reset(&TimerInstancePtr, 0);
//		XTmrCtr_Reset(&TimerInstancePtr, 1);
//		break;
//	}
//
//	// Clear Interrupt
//	XTmrCtr_ClearStats(&TimerInstancePtr);
//	XIntc_Acknowledge(&InterruptInstancePtr, XPAR_INTC_0_TMRCTR_0_VEC_ID);

}

int InitializeTimerSystem(void){

//	int Status;
//
////	TimerConfigPtr = XTmrCtr_LookupConfig(XPAR_AXI_TIMER_0_DEVICE_ID);
////	if (TimerConfigPtr == NULL){
////		return XST_FAILURE;
////	}
//
//	Status = XTmrCtr_Initialize(&TimerInstancePtr, XPAR_AXI_TIMER_0_DEVICE_ID);
//	if(Status != XST_SUCCESS){
//		return XST_FAILURE;
//	}
//
//	// Set timer counter 0/1 to generate interruptions and count down
//	XTmrCtr_SetOptions(&TimerInstancePtr, 0, XTC_DOWN_COUNT_OPTION | XTC_INT_MODE_OPTION);
//	XTmrCtr_SetOptions(&TimerInstancePtr, 1, XTC_DOWN_COUNT_OPTION | XTC_INT_MODE_OPTION);
//
//	// Set trigger output to 1
//	Xil_Out32(XPAR_AXI_GPIO_0_BASEADDR, 1);
//
//	// Set 10us
//	XTmrCtr_SetResetValue(&TimerInstancePtr, 0, 500);
//
//	// Set Interruption Handler to Timer
//	XTmrCtr_SetHandler(&TimerInstancePtr, (XTmrCtr_Handler) TimerInterruptHandler, (void*) &TimerInstancePtr);
//
//	// Start counting listen cycle
//	XTmrCtr_Start(&TimerInstancePtr, 0);

	return XST_SUCCESS;
}
