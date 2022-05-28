/*
 * dcmotor.c
 *
 *  Created on: Feb 25, 2022
 *      Author: Omar
 */


#include "dcmotor.h"
#include "gpio.h"
#include "common_macros.h"
#include "avr/io.h"

/* Initialize Motor */
void DcMotor_Init(void)
{
	/* declare motors PIN0, PIN1 in PortB as OUTPUT pins */
	GPIO_setupPinDirection(PORTB_ID, PIN0_ID,PIN_OUTPUT);
	GPIO_setupPinDirection(PORTB_ID, PIN1_ID,PIN_OUTPUT);

	/* stop motor by clearing motor pins */
	GPIO_writePin(PORTB_ID, PIN0_ID,LOGIC_LOW);
	GPIO_writePin(PORTB_ID, PIN1_ID,LOGIC_LOW);

}

/* Rotate motor according to given speed and direction */
void DcMotor_Rotate(DcMotor_state state, uint8 speed) {

	/* duty value for PWM 0 - 255 */
	uint8 duty_value;

	/* converting given speed from 0 - 100 scale to 0 255 scale */
	duty_value = ((speed * 255) / 100);
	Timer0_PWM_Init(duty_value);

	/* if speed not equal to zero rotate motor with given speed and direction */
	if (speed != 0) {

		/* in case of CLOCk WISE */
		if (state == CW) {

			GPIO_writePin(PORTB_ID, PIN0_ID, LOGIC_LOW);
			GPIO_writePin(PORTB_ID, PIN1_ID, LOGIC_HIGH);
			Timer0_PWM_Init(duty_value);

			/* in case of Anti CLOCk WISE */
		} else if (state == A_CW) {
			GPIO_writePin(PORTB_ID, PIN0_ID, LOGIC_HIGH);
			GPIO_writePin(PORTB_ID, PIN1_ID, LOGIC_LOW);
			Timer0_PWM_Init(duty_value);
		}
	}

	/* stop motor */
	else {
		GPIO_writePin(PORTB_ID, PIN0_ID, LOGIC_LOW);
		GPIO_writePin(PORTB_ID, PIN1_ID, LOGIC_LOW);
		Timer0_PWM_Init(0);
	}
}

void Timer0_PWM_Init(unsigned char set_duty_cycle)
{
	TCNT0 = 0; // Set Timer Initial Value to 0

	OCR0  = set_duty_cycle; //Set Compare value

	DDRB  = DDRB | (1<<PB3); // Configure PB3/OC0 as output pin --> pin where the PWM signal is generated from MC

	/* Configure timer control register
	 * 1. Fast PWM mode FOC0=0
	 * 2. Fast PWM Mode WGM01=1 & WGM00=1
	 * 3. Clear OC0 when match occurs (non inverted mode) COM00=0 & COM01=1
	 * 4. clock = F_CPU/8 CS00=0 CS01=1 CS02=0
	 */
	TCCR0 = (1<<WGM00) | (1<<WGM01) | (1<<COM01) | (1<<CS01);
}
