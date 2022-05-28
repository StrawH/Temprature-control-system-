/*
 * dcmotor.h
 *
 *  Created on: Feb 25, 2022
 *      Author: Omar
 */

#ifndef DCMOTOR_H_
#define DCMOTOR_H_
#include "std_types.h"

typedef enum
{
	CW,A_CW
}DcMotor_state;

void DcMotor_Init(void);

void DcMotor_Rotate(DcMotor_state state, uint8 speed);

void Timer0_PWM_Init(unsigned char set_duty_cycle);


#endif /* DCMOTOR_H_ */
