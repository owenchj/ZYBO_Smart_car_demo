/*
 * mb_ultrasonic.h

 *
 *  Created on: 12 janv. 2016
 *      Author: Artur
 */

#ifndef MB_ULTRASONIC_H_
#define MB_ULTRASONIC_H_

// Ultrasonic States
#define TRIGGER_ON 0
#define WAIT_ECHO 1
#define ECHO_ON 2
#define WAIT_CYCLE 3

#define US_CLOCK 50000000
#define US_MINIMUM_DISTANCE_IRQ 10
#define US_MINIMUM_DISTANCE_VALID 8
#define US_MINIMUM_IRQ_TOLERANCE 2

#define US_TRIGGER_TICKS US_CLOCK/100000
#define US_CYCLE_TICKS (US_CLOCK*6)/100
#define US_MAX_INTERN_COUNTER (US_CYCLE_TICKS- ((US_MINIMUM_DISTANCE_IRQ*US_CLOCK)/17000))
#define US_MIN_VALID_INTERN_COUNTER (US_CYCLE_TICKS- ((US_MINIMUM_DISTANCE_VALID*US_CLOCK)/17000))

extern int UltrasonicState;
extern int UltrasonicNextState;

int InitializeUltrasonicSystem(void);
int UltrasonicRoutine(void);


#endif /* MB_ULTRASONIC_H_ */
