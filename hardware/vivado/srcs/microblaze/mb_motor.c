/*
 * mb_motor.c
 *
 *  Created on: 12 janv. 2016
 *      Author: Artur
 */

#include <stdio.h>

#include "mb_motor.h"
#include "mb_bluetooth.h"
#include "mb_define.h"
#include "mb_speed.h"

#include "xparameters.h"
#include "xstatus.h"
#include "xil_io.h"
#include "xil_printf.h"

int moving_forward = 0;

int InitializeMotorSystem(void){

	u32 pivot1;

	Xil_Out32(XPAR_MOTOR_CONTROL_0_S00_AXI_BASEADDR, PWM_MOTOR_FREQUECY);
	pivot1 = Xil_In32(XPAR_MOTOR_CONTROL_0_S00_AXI_BASEADDR);
	xil_printf("Register value: %x\r\n", pivot1);
	Xil_Out32(XPAR_MOTOR_CONTROL_0_S00_AXI_BASEADDR +4, PWM_MOTOR_FREQUECY);
	pivot1 = Xil_In32(XPAR_MOTOR_CONTROL_0_S00_AXI_BASEADDR +4);
	xil_printf("Register value: %x\r\n", pivot1);
	Xil_Out32(XPAR_MOTOR_CONTROL_0_S00_AXI_BASEADDR +8, PWM_MOTOR_SPEED_9);
	pivot1 = Xil_In32(XPAR_MOTOR_CONTROL_0_S00_AXI_BASEADDR +8);
	xil_printf("Register value: %x\r\n", pivot1);
	Xil_Out32(XPAR_MOTOR_CONTROL_0_S00_AXI_BASEADDR +12, PWM_MOTOR_SPEED_9);
	pivot1 = Xil_In32(XPAR_MOTOR_CONTROL_0_S00_AXI_BASEADDR +12);
	xil_printf("Register value: %x\r\n", pivot1);
	Xil_Out32(XPAR_MOTOR_CONTROL_0_S00_AXI_BASEADDR +16, 0x300);
	pivot1 = Xil_In32(XPAR_MOTOR_CONTROL_0_S00_AXI_BASEADDR +16);
	xil_printf("Register value: %x\r\n", pivot1);

	BluetoothState = WAIT_ORDER;
	orderCounter = 0;

	return XST_SUCCESS;
}

void MotorUpdateBluetooth(char ldc, char rdc, char lsc, char rsc){

	int ld = (u32) (ldc  -'0');
	int rd = (u32) (rdc  -'0');
	int ls = (u32) (lsc  -'0');
	int rs = (u32) (rsc  -'0');

	MotorUpdate(ld, rd, ls, rs);
}

void MotorUpdate(int ld, int rd, int ls, int rs){

	xil_printf("Motor: MotorLeftDirectionInt - %d\r\n", ld);
	xil_printf("Motor: MotorRightDirectionInt - %d\r\n", rd);
	xil_printf("Motor: MotorLeftSpeedInt - %d\r\n", ls);
	xil_printf("Motor: MotorRightSpeedInt - %d\r\n", rs);

	Xil_Out32(XPAR_MOTOR_CONTROL_0_S00_AXI_BASEADDR +8, rs *PWM_MOTOR_FREQUENCY_SLICE);
	Xil_Out32(XPAR_MOTOR_CONTROL_0_S00_AXI_BASEADDR +12, ls *PWM_MOTOR_FREQUENCY_SLICE);

	if (!ld && !rd)
		Xil_Out32(XPAR_MOTOR_CONTROL_0_S00_AXI_BASEADDR +16, (u32) 0x300 | MOTOR_LEFT_BACKWARD | MOTOR_RIGHT_BACKWARD);
	else if (!ld && rd)
		Xil_Out32(XPAR_MOTOR_CONTROL_0_S00_AXI_BASEADDR +16, (u32) 0x300 | MOTOR_LEFT_BACKWARD | MOTOR_RIGHT_FOWARD);
	else if (ld && !rd)
		Xil_Out32(XPAR_MOTOR_CONTROL_0_S00_AXI_BASEADDR +16, (u32) 0x300 | MOTOR_LEFT_FOWARD | MOTOR_RIGHT_BACKWARD);
	else
		Xil_Out32(XPAR_MOTOR_CONTROL_0_S00_AXI_BASEADDR +16, (u32) 0x300 | MOTOR_LEFT_FOWARD | MOTOR_RIGHT_FOWARD);

	if (!ld || !rd)
		if ((ls != 0) || (rs != 0)){
			moving_forward = 1;
		}
		else{
			moving_forward = 0;
		}
	else{
		moving_forward = 0;
	}

	if ((ls == 0) && (rs == 0)){
		stopSpeedSystem();
	}
	else{
		startSpeedSystem();
	}
}

int MotorRoutine(void){

	char pivot;
	u32 pivot1;

	while (1){
		pivot = getchar();
		xil_printf("Got a: %c\r\n", pivot);
		pivot1 = Xil_In32(XPAR_MOTOR_CONTROL_0_S00_AXI_BASEADDR +16);
		xil_printf("Register value: %x\r\n", pivot1);
		switch(pivot){
		case 'w':
			Xil_Out32(XPAR_MOTOR_CONTROL_0_S00_AXI_BASEADDR +8, PWM_MOTOR_SPEED_5);
			Xil_Out32(XPAR_MOTOR_CONTROL_0_S00_AXI_BASEADDR +12, PWM_MOTOR_SPEED_5);
			Xil_Out32(XPAR_MOTOR_CONTROL_0_S00_AXI_BASEADDR +16, (u32) 0x300 | MOTOR_LEFT_FOWARD | MOTOR_RIGHT_FOWARD);
			break;
		case 's':
			Xil_Out32(XPAR_MOTOR_CONTROL_0_S00_AXI_BASEADDR +8, PWM_MOTOR_SPEED_5);
			Xil_Out32(XPAR_MOTOR_CONTROL_0_S00_AXI_BASEADDR +12, PWM_MOTOR_SPEED_5);
			Xil_Out32(XPAR_MOTOR_CONTROL_0_S00_AXI_BASEADDR +16, (u32) 0x300 | MOTOR_LEFT_BACKWARD | MOTOR_RIGHT_BACKWARD);
			break;
		case 'a':
			Xil_Out32(XPAR_MOTOR_CONTROL_0_S00_AXI_BASEADDR +8, PWM_MOTOR_SPEED_3);
			Xil_Out32(XPAR_MOTOR_CONTROL_0_S00_AXI_BASEADDR +12, PWM_MOTOR_SPEED_5);
			Xil_Out32(XPAR_MOTOR_CONTROL_0_S00_AXI_BASEADDR +16, (u32) 0x300 | MOTOR_LEFT_FOWARD | MOTOR_RIGHT_FOWARD);
			break;
		case 'd':
			Xil_Out32(XPAR_MOTOR_CONTROL_0_S00_AXI_BASEADDR +8, PWM_MOTOR_SPEED_5);
			Xil_Out32(XPAR_MOTOR_CONTROL_0_S00_AXI_BASEADDR +12, PWM_MOTOR_SPEED_3);
			Xil_Out32(XPAR_MOTOR_CONTROL_0_S00_AXI_BASEADDR +16, (u32) 0x300 | MOTOR_LEFT_FOWARD | MOTOR_RIGHT_FOWARD);
			break;
		default:
			Xil_Out32(XPAR_MOTOR_CONTROL_0_S00_AXI_BASEADDR +16, (u32) 0x00000000);
			break;
		}
		pivot1 = Xil_In32(XPAR_MOTOR_CONTROL_0_S00_AXI_BASEADDR +16);
		xil_printf("New Register value: %x\r\n", pivot1);
	}

	return XST_SUCCESS;
}

int Motor_isMovingForward(){
	return moving_forward;
}

void MotorStopFast(){
	Xil_Out32(XPAR_MOTOR_CONTROL_0_S00_AXI_BASEADDR +16, (u32) 0x00000000);
	moving_forward = 0;
	stopSpeedSystem();
}

//int InitializeMotorSystem(void){
//
//	int Status;
//
//	GpioConfigPtr0 = XGpio_LookupConfig(XPAR_AXI_GPIO_1_DEVICE_ID);
//	if (GpioConfigPtr0 == NULL){
//		return XST_FAILURE;
//	}
//
//	Status = XGpio_CfgInitialize(&GpioInstancePtr0, GpioConfigPtr0, GpioConfigPtr0->BaseAddress);
//	if(Status != XST_SUCCESS){
//		return XST_FAILURE;
//	}
//
//	// Set all the OUTPUT GPIOs to output
//	XGpio_SetDataDirection(&GpioInstancePtr0, 1, (u32) 0x00000000);
//
//	// Set all motors to be stopped
//	Xil_Out32(XPAR_AXI_GPIO_1_BASEADDR, (u32) 0x00000000);
//
//	return XST_SUCCESS;
//}
//

