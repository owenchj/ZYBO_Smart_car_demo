#include <stdio.h>
#include <stdlib.h>
#include "platform.h"

#include "xparameters.h"
#include "xstatus.h"
#include "xil_io.h"
#include "xil_printf.h"

#include "mb_motor.h"
#include "mb_ultrasonic.h"
#include "mb_bluetooth.h"
#include "mb_speed.h"
#include "mb_gpio.h"
#include "mb_timer.h"
#include "mb_interrupt.h"
#include "mb_define.h"

#include "xuartlite.h"

static char answer[4];
static char answer1[8];
static u32 pspl_last_order;
static u32 pspl_orderbuffer[3];
static int ld, rd, ls, rs;
static char ldc, rdc, lsc, rsc;

int MainRoutine(void){

	int successfullReceive;
	u32 pivot, pivot1;

	while (1){
		if(IRQ_US_FLAG){
			XUartLite_Send(&UartLiteInstancePtr,(u8*) "D100", 4);
			XUartLite_Send(&UartLiteInstancePtr,(u8*) "\r\n", 2);

			Xil_Out32(XPAR_INSTRUCTION_PL2PS_PS2PL_1_S00_AXI_BASEADDR, (u32) 0x00000002);
			Xil_Out32(XPAR_INSTRUCTION_PL2PS_PS2PL_1_S00_AXI_BASEADDR +16, (u32) 0x00000001);

			MotorStopFast();

			IRQ_US_FLAG = FALSE;
		}
		if(IRQ_PSPL){
			pivot = Xil_In32(XPAR_INSTRUCTION_PL2PS_PS2PL_0_S00_AXI_BASEADDR);
			xil_printf("IRQ_PSPL: Recieved this at R0 from IRQ PS2PL- %x\r\n", pivot);
			pivot = Xil_In32(XPAR_INSTRUCTION_PL2PS_PS2PL_0_S00_AXI_BASEADDR +4);
			xil_printf("IRQ_PSPL: Recieved this at R1 from IRQ PS2PL- %x\r\n", pivot);
			pivot = Xil_In32(XPAR_INSTRUCTION_PL2PS_PS2PL_0_S00_AXI_BASEADDR +8);
			xil_printf("IRQ_PSPL: Recieved this at R2 from IRQ PS2PL- %x\r\n", pivot);
			pivot = Xil_In32(XPAR_INSTRUCTION_PL2PS_PS2PL_0_S00_AXI_BASEADDR +12);
			xil_printf("IRQ_PSPL: Recieved this at R3 from IRQ PS2PL- %x\r\n", pivot);
			if(op_mode != OP_MODE_USER_ONLY){
				pspl_last_order = Xil_In32(XPAR_INSTRUCTION_PL2PS_PS2PL_0_S00_AXI_BASEADDR);
				switch (pspl_last_order){
				case 1:
					if ((op_mode == OP_MODE_IMAGE_PROC_ONLY) || (op_mode == OP_MODE_USER_CORRECTION)){
						pspl_orderbuffer[0] = pspl_last_order = Xil_In32(XPAR_INSTRUCTION_PL2PS_PS2PL_0_S00_AXI_BASEADDR +4);
						pspl_orderbuffer[1] = pspl_last_order = Xil_In32(XPAR_INSTRUCTION_PL2PS_PS2PL_0_S00_AXI_BASEADDR +8);
						pspl_orderbuffer[2] = pspl_last_order = Xil_In32(XPAR_INSTRUCTION_PL2PS_PS2PL_0_S00_AXI_BASEADDR +12);

						ld = pspl_orderbuffer[0] & (1 << 1);
						rd = pspl_orderbuffer[0] & 1;
						ls = pspl_orderbuffer[1];
						rs = pspl_orderbuffer[2];
						MotorUpdate(ld, rd, ls, rs);
					}
					break;
				case 3:
					pivot = Xil_In32(XPAR_ULTRASONIC_CONTROL_0_S00_AXI_BASEADDR +16);
					pivot = US_CYCLE_TICKS - pivot;
					pivot = (pivot* 17000)/US_CLOCK;
					Xil_Out32(XPAR_INSTRUCTION_PL2PS_PS2PL_1_S00_AXI_BASEADDR, (u32) 0x00000003);
					Xil_Out32(XPAR_INSTRUCTION_PL2PS_PS2PL_1_S00_AXI_BASEADDR + 4, pivot);
					Xil_Out32(XPAR_INSTRUCTION_PL2PS_PS2PL_1_S00_AXI_BASEADDR +16, (u32) 0x00000001);
					break;
				case 4:
					pivot = Xil_In32(XPAR_SPEED_CONTROL_0_S00_AXI_BASEADDR +4);
					pivot = (2 *3.14 *3.25 *SP_CLOCK)/pivot;
					pivot1 = Xil_In32(XPAR_SPEED_CONTROL_0_S00_AXI_BASEADDR +8);
					pivot1 = (2 *3.14 *3.25 *SP_CLOCK)/pivot1;
					Xil_Out32(XPAR_INSTRUCTION_PL2PS_PS2PL_1_S00_AXI_BASEADDR, (u32) 0x00000004);
					Xil_Out32(XPAR_INSTRUCTION_PL2PS_PS2PL_1_S00_AXI_BASEADDR + 4, pivot);
					Xil_Out32(XPAR_INSTRUCTION_PL2PS_PS2PL_1_S00_AXI_BASEADDR + 8, pivot1);
					Xil_Out32(XPAR_INSTRUCTION_PL2PS_PS2PL_1_S00_AXI_BASEADDR +16, (u32) 0x00000001);

					break;
				}
			}
			IRQ_PSPL = FALSE;
		}
		switch(BluetoothState){
		case WAIT_ORDER:
			successfullReceive = XUartLite_Recv(&UartLiteInstancePtr, &lastOrder, 1);
			if(successfullReceive){
				xil_printf("Bluetooth: Recieved this order - %c\r\n", (char) lastOrder);
				BluetoothState = ORDER_RECEIVED;
			}
			break;
		case ORDER_RECEIVED:
			switch((char) lastOrder){
			case 'W':
				successfullReceive = XUartLite_Recv(&UartLiteInstancePtr, &orderBuffer[orderCounter], 1);
				if(successfullReceive){
					xil_printf("Bluetooth: Recieved this command - %c\r\n", (char) orderBuffer[orderCounter]);
					if((orderBuffer[orderCounter] >= (u8)'0') && (orderBuffer[orderCounter] <= (u8)'9')){
						orderCounter += 1;
						if (orderCounter == 4){
							if ((op_mode == OP_MODE_USER_ONLY) || (op_mode == OP_MODE_USER_CORRECTION)){
								ldc = (char) orderBuffer[0];
								rdc = (char) orderBuffer[1];
								lsc = (char) orderBuffer[2];
								rsc = (char) orderBuffer[3];
								MotorUpdateBluetooth(ldc, rdc, lsc, rsc);
							}
							BluetoothState = WAIT_ORDER;
							orderCounter = 0;
						}
					}
					else{
						xil_printf("Bluetooth: Not a valid command\r\n");
						BluetoothState = WAIT_ORDER;
						orderCounter = 0;
					}
				}
				break;
			case 'D':
				pivot = Xil_In32(XPAR_ULTRASONIC_CONTROL_0_S00_AXI_BASEADDR +16);
				pivot = US_CYCLE_TICKS - pivot;
				pivot = (pivot* 17000)/US_CLOCK;
				sprintf(answer, "D0%lu%lu", pivot/10, pivot%10);
				xil_printf("Ultrasonic Distance: %d\r\n", pivot);
				XUartLite_Send(&UartLiteInstancePtr,(u8*) answer, 4);
				XUartLite_Send(&UartLiteInstancePtr,(u8*) "\r\n", 2);
				BluetoothState = WAIT_ORDER;
				orderCounter = 0;
				break;
			case 'S':
				pivot = Xil_In32(XPAR_SPEED_CONTROL_0_S00_AXI_BASEADDR +4);
				pivot = (2 *3.14 *3.25 *SP_CLOCK)/pivot;
				xil_printf("Speed left Km/h: %d\r\n", pivot);
				pivot1 = Xil_In32(XPAR_SPEED_CONTROL_0_S00_AXI_BASEADDR +8);
				pivot1 = (2 *3.14 *3.25 *SP_CLOCK)/pivot1;
				xil_printf("Speed right Km/h: %d\r\n", pivot1);
				sprintf(answer1, "S0%lu%lu%lu%lu%lu%lu", pivot/100, pivot/10, pivot%10, pivot1/100, pivot1/10, pivot1%10);
				XUartLite_Send(&UartLiteInstancePtr,(u8*) answer1, 8);
				XUartLite_Send(&UartLiteInstancePtr,(u8*) "\r\n", 2);
				BluetoothState = WAIT_ORDER;
				orderCounter = 0;
				break;
			case 'M':
				successfullReceive = XUartLite_Recv(&UartLiteInstancePtr, &orderBuffer[orderCounter], 1);
				if(successfullReceive){
					xil_printf("Bluetooth: Recieved this command - %c\r\n", (char) orderBuffer[orderCounter]);
					if((orderBuffer[orderCounter] >= (u8)'0') && (orderBuffer[orderCounter] <= (u8)'2')){
						orderCounter += 1;
						if (orderCounter == 1){
							op_mode = (u32) ((u8) orderBuffer[0] - (u8)'0');
							xil_printf("New OP_MODE: %d\r\n", op_mode);
							Xil_Out32(XPAR_INSTRUCTION_PL2PS_PS2PL_1_S00_AXI_BASEADDR, (u32) 0x00000000);
							Xil_Out32(XPAR_INSTRUCTION_PL2PS_PS2PL_1_S00_AXI_BASEADDR +4, op_mode);
							Xil_Out32(XPAR_INSTRUCTION_PL2PS_PS2PL_1_S00_AXI_BASEADDR +16, (u32) 0x00000001);
							BluetoothState = WAIT_ORDER;
							orderCounter = 0;
						}
					}
					else{
						xil_printf("Bluetooth: Not a valid command\r\n");
						BluetoothState = WAIT_ORDER;
						orderCounter = 0;
					}
				}
				break;
			default:
				xil_printf("Bluetooth: Not a valid order\r\n");
				BluetoothState = WAIT_ORDER;
				break;
			}
			break;
			default:
				break;
		}
	}

	return XST_SUCCESS;
}

int main()
{
	int Status;

	init_platform();

	xil_printf("Initiating Motor System...\r\n");
	Status = InitializeMotorSystem();
	if (Status != XST_SUCCESS){
		xil_printf("Motor System Initialization Failed!\r\n");
	}

	xil_printf("Initiating Bluetooth System...\r\n");
	Status = InitializeBluetoothSystem();
	if (Status != XST_SUCCESS){
		xil_printf("Bluetooth System Initialization Failed!\r\n");
	}

//	xil_printf("Initiating Bluetooth Routine...\r\n");
//	Status = BluetoothRoutine();
//	if (Status != XST_SUCCESS){
//		xil_printf("Bluetooth Routine Initialization Failed!\r\n");
//	}

	xil_printf("Initiating Ultrasonic System...\r\n");
	Status = InitializeUltrasonicSystem();
	if (Status != XST_SUCCESS){
		xil_printf("Ultrasonic System Initialization Failed!\r\n");
	}

	xil_printf("Initiating Speed System...\r\n");
	Status = InitializeSpeedSystem();
	if (Status != XST_SUCCESS){
		xil_printf("Speed System Initialization Failed!\r\n");
	}

	xil_printf("Initiating Interrupt System...\r\n");
	Status = InitializeInterruptSystem();
	if (Status != XST_SUCCESS){
		xil_printf("Interrupt System Initialization Failed!\r\n");
	}

	xil_printf("Initiating Microblaze Main Routine...\r\n");
	Status = MainRoutine();
	if (Status != XST_SUCCESS){
		xil_printf("Microblaze Main Routine Failed!\r\n");
	}

    return 0;
}
