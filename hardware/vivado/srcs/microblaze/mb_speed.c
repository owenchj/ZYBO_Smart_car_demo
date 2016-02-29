/*
 * mb_speed.c


 *
 *  Created on: 18 janv. 2016
 *      Author: Artur
 */

#include "mb_speed.h"

#include "xparameters.h"
#include "xstatus.h"
#include "xil_io.h"
#include "xil_printf.h"

int InitializeSpeedSystem(void){

	u32 pivot1;

	Xil_Out32(XPAR_SPEED_CONTROL_0_S00_AXI_BASEADDR +12, (u32) 0x00000000);
	pivot1 = Xil_In32(XPAR_SPEED_CONTROL_0_S00_AXI_BASEADDR +12);
	xil_printf("Register value: %x\r\n", pivot1);

	return XST_SUCCESS;
}

void stopSpeedSystem(){
	Xil_Out32(XPAR_SPEED_CONTROL_0_S00_AXI_BASEADDR +12, (u32) 0x0000000);
}

void startSpeedSystem(){
	Xil_Out32(XPAR_SPEED_CONTROL_0_S00_AXI_BASEADDR +12, (u32) 0x00000001);
}
