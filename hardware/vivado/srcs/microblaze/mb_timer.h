/*
 * mb_timer.h
 *
 *  Created on: 12 janv. 2016
 *      Author: Artur
 */

#ifndef MB_TIMER_H_
#define MB_TIMER_H_

//#include "xtmrctr.h"
//
//extern XTmrCtr TimerInstancePtr;
//extern XTmrCtr_Config *TimerConfigPtr;

int InitializeTimerSystem(void);
void TimerInterruptHandler(void);

#endif /* MB_TIMER_H_ */
