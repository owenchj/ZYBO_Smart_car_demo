/*
 * mb_bluetooth.c

 *
 *  Created on: 12 janv. 2016
 *      Author: Artur
 */

#include "mb_bluetooth.h"
#include "mb_define.h"

#include "xparameters.h"
#include "xstatus.h"
#include "xil_io.h"
#include "xil_printf.h"


#include <stdio.h>

int BluetoothState = WAIT_ORDER;
u8 lastOrder = 0;
int orderCounter = 0;
u8 orderBuffer[4];

XUartLite UartLiteInstancePtr;
XUartLite_Config *UartLiteConfigPtr;

u32 op_mode = OP_MODE_USER_CORRECTION;

int InitializeBluetoothSystem(void){

	int Status;

//	UartLiteConfigPtr = XUartLite_LookupConfig(XPAR_AXI_UARTLITE_0_DEVICE_ID);
//	if (UartLiteConfigPtr == NULL){
//		return XST_FAILURE;
//	}

//	UartLiteConfigPtr->BaudRate = (u32) 9600;
//	UartLiteConfigPtr->UseParity = 0;

//	xil_printf("Uart DeviceId: %x\r\n", UartLiteConfigPtr->DeviceId);
//	xil_printf("Uart RegBaseAddr: %x\r\n", UartLiteConfigPtr->RegBaseAddr);
//	xil_printf("Uart BaudRate: %x\r\n", UartLiteConfigPtr->BaudRate);
//	xil_printf("Uart UseParity: %x\r\n", UartLiteConfigPtr->UseParity);
//	xil_printf("Uart ParityOdd: %x\r\n", UartLiteConfigPtr->ParityOdd);
//	xil_printf("Uart DataBits: %x\r\n", UartLiteConfigPtr->DataBits);
//
//	Status = XUartLite_CfgInitialize(&UartLiteInstancePtr, UartLiteConfigPtr, UartLiteConfigPtr->RegBaseAddr);
//	if(Status != XST_SUCCESS){
//		return XST_FAILURE;
//	}

	Status = XUartLite_Initialize(&UartLiteInstancePtr, XPAR_AXI_UARTLITE_0_DEVICE_ID);
	if (Status != XST_SUCCESS) {
		return XST_FAILURE;
	}

	Status = XUartLite_SelfTest(&UartLiteInstancePtr);
	if (Status != XST_SUCCESS) {
		return XST_FAILURE;
	}

	return XST_SUCCESS;
}

int BluetoothRoutine(void){

	u8 pivot;
//	int successfullReceive;

	while (1){
		pivot = (u8) getchar();
		putchar((char) pivot);
		XUartLite_Send(&UartLiteInstancePtr, &pivot, 1);
		if (pivot == '\r'){
			pivot = (u8) '\n';
			putchar((char) pivot);
			XUartLite_Send(&UartLiteInstancePtr, &pivot, 1);
			while(1){
				XUartLite_Recv(&UartLiteInstancePtr, &pivot, 1);
				if(pivot){
					putchar((char) pivot);
//					xil_printf("%d", pivot);
					if ((pivot == '\r') || (pivot == '\n'))
						break;
				}
			}
		}
	}

//	while (1){
//		successfullReceive = XUartLite_Recv(&UartLiteInstancePtr, &pivot, 1);
//		if(successfullReceive)
//			xil_printf("Bluetooth: Recieved this char - %c\r\n", (char) pivot);
//	}

	return XST_SUCCESS;

}
