/*
 * mb_motor.h

 *
 *  Created on: 12 janv. 2016
 *      Author: Artur
 */

#ifndef MB_MOTOR_H_
#define MB_MOTOR_H_

#define MOTOR_LEFT_FOWARD 0xA0
#define MOTOR_LEFT_BACKWARD 0x50
#define MOTOR_RIGHT_FOWARD 0x0A
#define MOTOR_RIGHT_BACKWARD 0x05

#define PWM_MOTOR_FREQUECY 900000
#define PWM_MOTOR_FREQUENCY_SLICE PWM_MOTOR_FREQUECY/9
#define PWM_MOTOR_SPEED_0 0
#define PWM_MOTOR_SPEED_1 PWM_MOTOR_FREQUENCY_SLICE
#define PWM_MOTOR_SPEED_2 PWM_MOTOR_FREQUENCY_SLICE*2
#define PWM_MOTOR_SPEED_3 PWM_MOTOR_FREQUENCY_SLICE*3
#define PWM_MOTOR_SPEED_4 PWM_MOTOR_FREQUENCY_SLICE*4
#define PWM_MOTOR_SPEED_5 PWM_MOTOR_FREQUENCY_SLICE*5
#define PWM_MOTOR_SPEED_6 PWM_MOTOR_FREQUENCY_SLICE*6
#define PWM_MOTOR_SPEED_7 PWM_MOTOR_FREQUENCY_SLICE*7
#define PWM_MOTOR_SPEED_8 PWM_MOTOR_FREQUENCY_SLICE*8
#define PWM_MOTOR_SPEED_9 PWM_MOTOR_FREQUENCY_SLICE*9

int InitializeMotorSystem(void);
void MotorUpdateBluetooth(char ldc, char rdc, char lsc, char rsc);
void MotorUpdate(int ld, int rd, int ls, int rs);
int Motor_isMovingForward();
int MotorRoutine(void);
void MotorStopFast();

#endif /* MB_MOTOR_H_ */
