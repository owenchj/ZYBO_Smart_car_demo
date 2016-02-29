/*
 * mb_deprecated_code.c

 *
 *  Created on: 12 janv. 2016
 *      Author: Artur
 */

#define LED_DELAY 10000000

#include "xgpiops.h"

static XGpioPs GpioPsInstancePtr;
static XGpioPs_Config *GpioPsConfigPtr;

//void GpioPsInterruptHandler(void){
//
//	// Do Something
//	u32 pivot;
//	xil_printf("GpioPs Interruption Arrived!!!\r\n");
//	// Read information from GPIO BANK0
//	pivot =  XGpioPs_Read(&GpioPsInstancePtr, XGPIOPS_BANK0);
//
//	// Enable or disables the led
//	if(!(pivot&(1<<10))){
//		pivot |= (1<<10);
//		XGpioPs_Write(&GpioPsInstancePtr, XGPIOPS_BANK0, pivot);
//	}
//	else{
//		pivot &= ~(1<<10);
//		XGpioPs_Write(&GpioPsInstancePtr, XGPIOPS_BANK0, pivot);
//	}
//
//	// Clear Interrupt
//	XGpioPs_IntrClear(&GpioPsInstancePtr, XGPIOPS_BANK0, (1<<12)|(1<<14));
//	XIntc_Acknowledge(&InterruptInstancePtr, XPAR_INTC_0_GPIOPS_0_VEC_ID);
//
//}

//int InitializeGpioPsSystem(void){
//
//	int Status;
//	u32 pivot, pivot0, pivot1, pivot2;
//
//	// Get standard configuration
//	GpioPsConfigPtr = XGpioPs_LookupConfig(XPAR_XGPIOPS_0_DEVICE_ID);
//	if (GpioPsConfigPtr == NULL){
//		return XST_FAILURE;
//	}
//
//	// GPIO driver initialization
//	Status = XGpioPs_CfgInitialize(&GpioPsInstancePtr, GpioPsConfigPtr, GpioPsConfigPtr->BaseAddr);
//	if (Status != XST_SUCCESS){
//		return XST_FAILURE;
//	}
//
//	// Configure GPIO PIN 10 to output and PIN 12/PIN 14 as inputs
//	pivot = XGpioPs_GetDirection(&GpioPsInstancePtr, XGPIOPS_BANK0);
//	xil_printf("GPIOPS Initial Direction: %04x\r\n", pivot);
//	pivot |= (1<<10);
//	pivot &= ~(1<<12);
//	pivot &= ~(1<<14);
//	XGpioPs_SetDirection(&GpioPsInstancePtr, XGPIOPS_BANK0, pivot);
//	pivot = XGpioPs_GetDirection(&GpioPsInstancePtr, XGPIOPS_BANK0);
//	xil_printf("GPIOPS NEW Direction: %04x\r\n", pivot);
//
//	// Enable GPIO PIN 10 output
//	pivot = XGpioPs_GetOutputEnable(&GpioPsInstancePtr, XGPIOPS_BANK0);
//	pivot |= (1<<10);
//	XGpioPs_SetOutputEnable(&GpioPsInstancePtr, XGPIOPS_BANK0, pivot);
//
//	// Configure GpioPs PIN 12 and 14 to generate interruptions at rising edge
//	XGpioPs_GetIntrType(&GpioPsInstancePtr, XGPIOPS_BANK0, &pivot0, &pivot1, &pivot2);
//	xil_printf("GPIOPS Initial Interrupt Type: %04x\r\n", pivot0);
//	xil_printf("GPIOPS Initial Interrupt Polarity: %04x\r\n", pivot1);
//	xil_printf("GPIOPS Initial Interrupt OnAny: %04x\r\n", pivot2);
//	pivot0 |= (1<<12);
//	pivot0 |= (1<<14);
//	pivot1 |= (1<<12);
//	pivot1 |= (1<<14);
//	pivot2 &= ~(1<<12);
//	pivot2 &= ~(1<<14);
//	XGpioPs_SetIntrType(&GpioPsInstancePtr, XGPIOPS_BANK0, pivot0, pivot1, pivot2);
//	XGpioPs_GetIntrType(&GpioPsInstancePtr, XGPIOPS_BANK0, &pivot0, &pivot1, &pivot2);
//	xil_printf("GPIOPS NEW Interrupt Type: %04x\r\n", pivot0);
//	xil_printf("GPIOPS NEW Interrupt Polarity: %04x\r\n", pivot1);
//	xil_printf("GPIOPS NEW Interrupt OnAny: %04x\r\n", pivot2);
//
//	// Link the handler function to the GpioPs Interruptions
//	XGpioPs_SetCallbackHandler(&GpioPsInstancePtr, (void*) &GpioPsInstancePtr, (XGpioPs_Handler) GpioPsInterruptHandler);
//
//	pivot = XGpioPs_IntrGetEnabled(&GpioPsInstancePtr, XGPIOPS_BANK0);
//	xil_printf("GPIOPS Initial Interrupt Enabled: %04x\r\n", pivot);
//	pivot |= (1<<12);
//	pivot |= (1<<14);
//	XGpioPs_IntrEnable(&GpioPsInstancePtr, XGPIOPS_BANK0, pivot);
//	pivot = XGpioPs_IntrGetEnabled(&GpioPsInstancePtr, XGPIOPS_BANK0);
//	xil_printf("GPIOPS NEW Interrupt Enabled: %04x\r\n", pivot);
//
//	return XST_SUCCESS;
//}

//int LedOutputExample(void){
//
//	volatile int Delay;
////	int led_on = 1;
////	u32 pivot;
//	u32 gpio_output_counter = 0;
//
//	// Loop forever, blinking the LED and reading switch values
//	while(1){
//
//		// Read information from GPIO BANK0
////		pivot =  XGpioPs_Read(&GpioPsInstancePtr, XGPIOPS_BANK0);
//
//		// Enable or disables the led
//		/*xil_printf("Current Conf: %04x\r\n", pivot);*/
////		if(led_on){
////			pivot |= (1<<10);
////			XGpioPs_Write(&GpioPsInstancePtr, XGPIOPS_BANK0, pivot);
////			/*xil_printf("On Conf: %04x\r\n", pivot);*/
////		}
////		else{
////			pivot &= ~(1<<10);
////			XGpioPs_Write(&GpioPsInstancePtr, XGPIOPS_BANK0, pivot);
////			/*xil_printf("Off Conf: %04x\r\n", pivot);*/
////		}
////		led_on = (led_on+1)%2;
//
//		gpio_output_counter = (gpio_output_counter +1)%16;
//		Xil_Out32(XPAR_AXI_GPIO_0_BASEADDR, gpio_output_counter);
////		XGpio_DiscreteWrite(&GpioInstancePtr, 1, gpio_output_counter);
//		xil_printf("Wrote %d to the GPIO OUTPUT...\r\n", gpio_output_counter);
//
//
//		// Verify the switchs
////		if (pivot & (1<<12))
////			xil_printf("SW15_2: ON\r\n");
////		else
////			xil_printf("SW15_2: OFF\r\n");
////		if (pivot & (1<<14))
////			xil_printf("SW15_1: ON\r\n");
////		else
////			xil_printf("SW15_1: OFF\r\n");
////		xil_printf("\r\n");
//
//		// Wait a small amount of time so that the LED blinking is visible
//		for (Delay = 0; Delay <LED_DELAY; Delay++);
//	}
//
//	return XST_SUCCESS;
//
//}

//int MainRoutine(void){
//
//	u32 Timer1;
//	u32 sparedCycles;
//	u32 distance;
//
//	int successfullReceive;
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
//		switch(BluetoothState){
//		case WAIT_ORDER:
//			successfullReceive = XUartLite_Recv(&UartLiteInstancePtr, &lastOrder, 1);
//			if(successfullReceive){
//				xil_printf("Bluetooth: Recieved this order - %c\r\n", (char) lastOrder);
//				BluetoothState = ORDER_RECEIVED;
//			}
//			break;
//		case ORDER_RECEIVED:
//			switch((char) lastOrder){
//			case 'W':
//				successfullReceive = XUartLite_Recv(&UartLiteInstancePtr, &orderBuffer[orderCounter], 1);
//				if(successfullReceive){
//					xil_printf("Bluetooth: Recieved this command - %c\r\n", (char) orderBuffer[orderCounter]);
//					if((orderBuffer[orderCounter] >= (u8)'0') && (orderBuffer[orderCounter] <= (u8)'9')){
//						orderCounter += 1;
//						if (orderCounter == 4){
//							MotorLeftDirection = (char) orderBuffer[0];
//							MotorRightDirection = (char) orderBuffer[1];
//							MotorLeftSpeed = (char) orderBuffer[2];
//							MotorRightSpeed = (char) orderBuffer[3];
//							BluetoothState = WAIT_ORDER;
//							MotorUpdate();
//							orderCounter = 0;
//						}
//					}
//					else{
//						xil_printf("Bluetooth: Not a valid command\r\n");
//						BluetoothState = WAIT_ORDER;
//						orderCounter = 0;
//					}
//				}
//				break;
//			default:
//				xil_printf("Bluetooth: Not a valid order\r\n");
//				BluetoothState = WAIT_ORDER;
//				break;
//			}
//			break;
//			default:
//				break;
//		}
//	}
//
//	return XST_SUCCESS;
//}
