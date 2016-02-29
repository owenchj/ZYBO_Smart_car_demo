/*
 * mb_bluetooth.h


 *
 *  Created on: 12 janv. 2016
 *      Author: Artur
 */


#include "xuartlite.h"

#ifndef MB_BLUETOOTH_H_
#define MB_BLUETOOTH_H_

#define WAIT_ORDER 0
#define ORDER_RECEIVED 1

extern int BluetoothState;
extern u8 lastOrder;
extern int orderCounter;
extern u8 orderBuffer[4];
extern u32 op_mode;

extern XUartLite UartLiteInstancePtr;
extern XUartLite_Config *UartLiteConfigPtr;

int InitializeBluetoothSystem(void);
int BluetoothRoutine(void);

#endif /* MB_BLUETOOTH_H_ */
