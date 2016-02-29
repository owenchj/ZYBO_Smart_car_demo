/*
 * mb_interrupt.h

 *
 *  Created on: 12 janv. 2016
 *      Author: Artur
 */

#ifndef MB_INTERRUPT_H_
#define MB_INTERRUPT_H_

#include "xintc.h"
#include "xil_exception.h"

extern XIntc InterruptInstancePtr;
extern XIntc_Config *InterruptConfigPtr;

extern int IRQ_US_FLAG;
extern int IRQ_PSPL;

int InitializeInterruptSystem(void);
int InterruptRoutine(void);

#endif /* MB_INTERRUPT_H_ */
