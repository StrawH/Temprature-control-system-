/*
 * main.c
 *
 *  Created on: Feb 25, 2022
 *  Project title: Fan Speed Controller with Temperature
 *  In this project I used :
 *  LCD 2*16 to display fan status and temperature degree
 *  LM35 temperature sensor
 *  DC motor to control fan
 *  Author: Omar E. Yousef
 */

/* include drivers libraries */

/* delay library */
#include "util/delay.h"

/* LCD library */
#include "lcd.h"

/* ADC library */
#include "adc.h"

/* LM35 sensor library */
#include "lm35_sensor.h"

/* DC Motor library */
#include "dcmotor.h"

/* Standard Data types library */
#include "std_types.h"


/* Application Main function */
int main(void) {

	/* Initialize LCD */
	LCD_init();

	/* Initialize ADC
	 * internal Vref = 2.56
	 * Channel 2 ADC2 */

	/* Vref options
	 * 0 : AREF , Vref turned off
	 * 1 : Avcc = 5
	 * 3 : internal vref = 2.56
	 */
	ADC_con.ADC_ReferenceVolatge = 3;

	/* Prescaler options
	 * 0 : 2
	 * 1 : 2
	 * 2 : 4
	 * 3 : 8
	 * 4 : 16
	 * 5 : 32
	 * 6 : 64
	 * 7 : 128
	 */
	ADC_con.prescaler = 3;

	ADC_init(&ADC_con);
	//ADC_init();

	/* Initialize DC Motor
	 * Default status is off
	 * PB0, PB1 motor pins
	 * PB3 for speed control */
	DcMotor_Init();

	/* temperature sensor reading degree */
	uint8 temp_value = 0;

	/* fan status flag */
	boolean fan_state = TRUE;

	// wait 10 ms for system stabilization
	_delay_ms(10);

	LCD_displayString("Welcome");
	_delay_ms(1000);
	LCD_clearScreen();

	/* Application super loop */
	while (1) {

		/* read temperature sensor value */
		temp_value = LM35_getTemperature();

		/* control fan motor according to sensor temperature value */
		if (temp_value < 30) {

			/* stop motor */
			DcMotor_Rotate(CW, 0);
			fan_state = FALSE;

		} else if (temp_value >= 30 && temp_value < 60) {

			/* Turn on fan with 25 % of it's speed */
			DcMotor_Rotate(CW, 25);
			fan_state = TRUE;

		} else if (temp_value >= 60 && temp_value < 90) {

			/* Turn on fan with 50 % of it's speed */
			DcMotor_Rotate(CW, 50);
			fan_state = TRUE;

		} else if (temp_value >= 90 && temp_value < 120) {

			/* Turn on fan with 75 % of it's speed */
			DcMotor_Rotate(CW, 75);
			fan_state = TRUE;

		} else if (temp_value >= 120) {

			/* Turn on fan with 100 % of it's speed */
			DcMotor_Rotate(CW, 100);
			fan_state = TRUE;
		}

		/******* Display on LCD Fan status and temperature value *********/

		/* Move Lcd cursor to first row and second column */
		LCD_moveCursor(0, 0);

		/* display fan status */
		LCD_displayString("Fan is ");
		if (fan_state == FALSE) {
			LCD_displayString("OFF");
		} else {
			LCD_displayString(" ON");
		}

		/* Move Lcd cursor to second row and second column */
		LCD_moveCursor(1, 1);

		/* display temperature value */
		LCD_displayString("Temp = ");
		LCD_intgerToString(temp_value);
		LCD_displayString(" C");
	}

	/* end of super loop */
	return 0;
}

