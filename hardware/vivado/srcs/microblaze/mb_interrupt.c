/*
 * mb_interrupt.c

 *
 *  Created on: 12 janv. 2016
 *      Author: Artur
 */

#include "mb_interrupt.h"
#include "mb_timer.h"
#include "mb_gpio.h"
#include "mb_motor.h"

#include "xparameters.h"
#include "xstatus.h"
#include "xintc_l.h"
#include "xil_exception.h"

XIntc InterruptInstancePtr;
XIntc_Config *InterruptConfigPtr;

int IRQ_US_FLAG = FALSE;
int IRQ_PSPL = FALSE;

void SetupInterruptSystem();

void UltrasonicInterruptHandler(void *CallbackRef);
void PsPlInterruptHandler(void *CallbackRef);

int InterruptRoutine(void){

	/*
	 * Run the low level example of Interrupt Controller, specify the Base
	 * Address generated in xparameters.h.
	 */
	while (1)
	{
		/*
		 * If the interrupt occurred which is indicated by the global
		 * variable which is set in the device driver handler, then
		 * stop waiting.
		 */
		if (IRQ_US_FLAG) {
			xil_printf("Ultrasonic Interrupt: Interruption received\r\n");
			IRQ_US_FLAG = FALSE;
		}
	}

	return XST_SUCCESS;

}

int InitializeInterruptSystem(void){


	/*
	 * Connect a device driver handler that will be called when an interrupt
	 * for the device occurs, the device driver handler performs the
	 * specific interrupt processing for the device.
	 */
	XIntc_RegisterHandler(XPAR_INTC_0_BASEADDR, XPAR_INTC_0_ULTRASONIC_CONTROL_0_VEC_ID,
			(XInterruptHandler)UltrasonicInterruptHandler,
			(void *)0);
	XIntc_RegisterHandler(XPAR_INTC_0_BASEADDR, XPAR_INTC_0_INSTRUCTION_PL2PS_PS2PL_0_VEC_ID,
			(XInterruptHandler)PsPlInterruptHandler,
			(void *)0);

	/*
	 * Enable interrupts for all devices that cause interrupts, and enable
	 * the INTC master enable bit.
	 */
	XIntc_EnableIntr(XPAR_INTC_0_BASEADDR, XPAR_ULTRASONIC_CONTROL_0_US_IRQ_MASK | XPAR_INSTRUCTION_PL2PS_PS2PL_0_IRQ_MASK);


	/*
	 * Set the master enable bit and the hardware interrupt enable bit
	 */
	XIntc_Out32(XPAR_INTC_0_BASEADDR + XIN_MER_OFFSET, XIN_INT_MASTER_ENABLE_MASK | XIN_INT_HARDWARE_ENABLE_MASK);

	/*
	 * This step is processor specific, connect the handler for the
	 * interrupt controller to the interrupt source for the processor.
	 */
	SetupInterruptSystem();

	/*
	 * Cause (simulate) an interrupt so the handler will be called. This is
	 * done by writing a 1 to the interrupt status bit for the device
	 * interrupt.
	 */
	//	XIntc_Out32(XPAR_INTC_0_BASEADDR + XIN_ISR_OFFSET, XPAR_ULTRASONIC_CONTROL_0_US_IRQ_MASK);

	/*
	 * Wait for the interrupt to be processed, if the interrupt does not
	 * occur this loop will wait forever.
	 */

	return XST_SUCCESS;
}

void SetupInterruptSystem()
{
	/*
	 * Initialize the exception table.
	 */
	Xil_ExceptionInit();

	/*
	 * Register the interrupt controller handler with the exception table.
	 */
	Xil_ExceptionRegisterHandler(XIL_EXCEPTION_ID_INT,
			(Xil_ExceptionHandler)XIntc_DeviceInterruptHandler,
			XPAR_INTC_0_DEVICE_ID);

	/*
	 * Enable exceptions.
	 */
	Xil_ExceptionEnable();

}

void UltrasonicInterruptHandler(void *CallbackRef)
{
	/*
	 * Indicate the interrupt has been processed using a shared variable.
	 */
	if(Motor_isMovingForward())
		IRQ_US_FLAG = TRUE;
}

void PsPlInterruptHandler(void *CallbackRef)
{
	/*
	 * Indicate the interrupt has been processed using a shared variable.
	 */
	IRQ_PSPL = TRUE;
}

//int InitializeInterruptSystem(void){
//
//	int Status;
//
//	// Exception Initialization
//	Xil_ExceptionInit();
//
//	// Interrupt Controller Initialization
//	Status = XIntc_Initialize(&InterruptInstancePtr, XPAR_INTC_0_DEVICE_ID);
//	if(Status != XST_SUCCESS){
//		return XST_FAILURE;
//	}
//
////	// Set Interrupt Controller to Ultrasonic System
////	Status = XIntc_Connect(&InterruptInstancePtr, XPAR_INTC_0_ULTRASONIC_0_VEC_ID,
////			(XInterruptHandler) UltrasonicInterruptHandler, NULL);
////	if(Status != XST_SUCCESS){
////		return XST_FAILURE;
////	}
//
////	// Set Interrupt Handler to GpioPs System
////	Status = XIntc_Connect(&InterruptInstancePtr, XPAR_INTC_0_GPIOPS_0_VEC_ID,
////			(XInterruptHandler) GpioPsInterruptHandler, &XGpioPs_IntrHandler);
////	if(Status != XST_SUCCESS){
////		return XST_FAILURE;
////	}
//
////	// Set Interrupt Controller to Timer System
////	Status = XIntc_Connect(&InterruptInstancePtr, XPAR_INTC_0_TMRCTR_0_VEC_ID,
////			(XInterruptHandler) TimerInterruptHandler, &XTmrCtr_InterruptHandler);
////	if(Status != XST_SUCCESS){
////		return XST_FAILURE;
////	}
//
////	// Set Interrupt Controller to Gpio System
////	Status = XIntc_Connect(&InterruptInstancePtr, XPAR_INTC_0_GPIO_0_VEC_ID,
////			(XInterruptHandler) GpioInterruptHandler, &GpioInstancePtr1);
////	if(Status != XST_SUCCESS){
////		return XST_FAILURE;
////	}
//
//	// Start Interrupt controller
//	Status = XIntc_Start(&InterruptInstancePtr, XIN_REAL_MODE);
//	if(Status != XST_SUCCESS){
//		return XST_FAILURE;
//	}
//
//	// Enable Interrupts for GpioPs
//	XIntc_Enable(&InterruptInstancePtr, XPAR_INTC_0_GPIOPS_0_VEC_ID);
//
//	// Enable Interrupts for Timer
//	XIntc_Enable(&InterruptInstancePtr, XPAR_INTC_0_TMRCTR_0_VEC_ID);
//
//	// Enable Interrupts for Gpio
//	XIntc_Enable(&InterruptInstancePtr, XPAR_INTC_0_GPIO_0_VEC_ID);
//
//	// Exception Setup - Configure exceptions to be trated by the Interrupt Controller System
//	Xil_ExceptionRegisterHandler(XIL_EXCEPTION_ID_INT,
//			(Xil_ExceptionHandler) XIntc_InterruptHandler,
//			(void *) &InterruptInstancePtr);
//	Xil_ExceptionEnable();
//
//	return XST_SUCCESS;
//}
