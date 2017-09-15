/*
 * main.c
 *
 *  Created on: Sep 7, 2017
 *      Author: Zack
 */
//Fuse settings
//-U lfuse:w:0x3f:m -U hfuse:w:0xd0:m
//#define F_CPU 16000000UL
#include <stdint.h>
#include <stdlib.h>
#define byte uint8_t
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include "io.h"
#include "MAX528.h"

byte DAC_value = 0;

int main(void) {
	//setup
	io_setup();

	while(1) {
		check_buttons();
		for (int x = 0; x < 16;x+=4){ //16 pots
			for (int i = 0; i < 4;i++){ //4 LED columns
				step_poll(); //check if we should change step

				set_LEDs(i);

				_delay_us(100); //wait to display LEDs
				check_pots(i+x);
			}
		}
	}

	return 1;
}
