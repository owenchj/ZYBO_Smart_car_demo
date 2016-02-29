/*
 * mb_ultrasonic.c

 *
 *  Created on: 12 janv. 2016
 *      Author: Artur
 */

#include "mb_ultrasonic.h"

#include "xparameters.h"
#include "xstatus.h"
#include "xil_io.h"
#include "xil_printf.h"

int UltrasonicState = TRIGGER_ON;
int UltrasonicNextState = TRIGGER_ON;

int InitializeUltrasonicSystem(void){

	u32 pivot1;

	Xil_Out32(XPAR_ULTRASONIC_CONTROL_0_S00_AXI_BASEADDR, US_TRIGGER_TICKS);
	pivot1 = Xil_In32(XPAR_ULTRASONIC_CONTROL_0_S00_AXI_BASEADDR);
	xil_printf("Register value: %x\r\n", pivot1);
	Xil_Out32(XPAR_ULTRASONIC_CONTROL_0_S00_AXI_BASEADDR +4, US_CYCLE_TICKS);
	pivot1 = Xil_In32(XPAR_ULTRASONIC_CONTROL_0_S00_AXI_BASEADDR +4);
	xil_printf("Register value: %x\r\n", pivot1);
	Xil_Out32(XPAR_ULTRASONIC_CONTROL_0_S00_AXI_BASEADDR +8, US_MAX_INTERN_COUNTER);
	pivot1 = Xil_In32(XPAR_ULTRASONIC_CONTROL_0_S00_AXI_BASEADDR +8);
	xil_printf("Register value: %x\r\n", pivot1);
	Xil_Out32(XPAR_ULTRASONIC_CONTROL_0_S00_AXI_BASEADDR +12, (u32) US_MIN_VALID_INTERN_COUNTER);
	pivot1 = Xil_In32(XPAR_ULTRASONIC_CONTROL_0_S00_AXI_BASEADDR +12);
	xil_printf("Register value: %x\r\n", pivot1);
	Xil_Out32(XPAR_ULTRASONIC_CONTROL_0_S00_AXI_BASEADDR +16, (u32) US_MINIMUM_IRQ_TOLERANCE);
	pivot1 = Xil_In32(XPAR_ULTRASONIC_CONTROL_0_S00_AXI_BASEADDR +16);
	xil_printf("Register value: %x\r\n", pivot1);

	return XST_SUCCESS;
}

int UltrasonicRoutine(void){

	u32 pivot1;
	int i;

	while (1){
		pivot1 = Xil_In32(XPAR_ULTRASONIC_CONTROL_0_S00_AXI_BASEADDR +16);
		pivot1 = US_CYCLE_TICKS - pivot1;
		pivot1 = (pivot1* 17000)/US_CLOCK;
		xil_printf("Ultrasonic Distance: %d\r\n", pivot1);

		for (i=0; i<100000; i++) ;
	}

	return XST_SUCCESS;

}

//int UltrasonicRoutine(void){
//
//	u32 Timer1;
//	u32 sparedCycles;
//	u32 distance;
//
//	while (1){
//		switch(UltrasonicState){
//		case TRIGGER_ON:
//			UltrasonicState = UltrasonicNextState;
//			if (UltrasonicNextState == WAIT_ECHO){
//				Xil_Out32(XPAR_AXI_GPIO_0_BASEADDR, 0);
//
//				XTmrCtr_SetResetValue(&TimerInstancePtr, 0, US_CYCLE_TICKS);
//				XTmrCtr_Start(&TimerInstancePtr, 0);
//			}
//			break;
//		case WAIT_ECHO:
//			UltrasonicState = UltrasonicNextState;
//			if (UltrasonicNextState == ECHO_ON){
//				XTmrCtr_SetResetValue(&TimerInstancePtr, 1, US_CYCLE_TICKS);
//				XTmrCtr_Start(&TimerInstancePtr, 1);
//			}
//			else if(UltrasonicNextState == TRIGGER_ON){
//				xil_printf("Ultrasonic Timeout!!!\r\n");
//				Xil_Out32(XPAR_AXI_GPIO_0_BASEADDR, 1);
//
//				XTmrCtr_SetResetValue(&TimerInstancePtr, 0, 500);
//				XTmrCtr_Start(&TimerInstancePtr, 0);
//			}
//			break;
//		case ECHO_ON:
//			UltrasonicState = UltrasonicNextState;
//			if (UltrasonicNextState == WAIT_CYCLE){
//				Timer1 = XTmrCtr_GetValue(&TimerInstancePtr, 1);
//				sparedCycles = US_CYCLE_TICKS - Timer1;
//				distance = (sparedCycles* 17000)/CLOCK;
//				xil_printf("Ultrasonic distance: %d!!!\r\n", distance);
//			}
//			else if(UltrasonicNextState == TRIGGER_ON){
//				xil_printf("Ultrasonic Timeout!!!\r\n");
//				Xil_Out32(XPAR_AXI_GPIO_0_BASEADDR, 1);
//
//				XTmrCtr_SetResetValue(&TimerInstancePtr, 0, 500);
//				XTmrCtr_Start(&TimerInstancePtr, 0);
//			}
//			break;
//		case WAIT_CYCLE:
//			UltrasonicState = UltrasonicNextState;
//			if(UltrasonicNextState == TRIGGER_ON){
//				Xil_Out32(XPAR_AXI_GPIO_0_BASEADDR, 1);
//
//				XTmrCtr_SetResetValue(&TimerInstancePtr, 0, 500);
//				XTmrCtr_Start(&TimerInstancePtr, 0);
//			}
//			break;
//		default:
//			break;
//		}
//	}
//
//	return XST_SUCCESS;
//}
