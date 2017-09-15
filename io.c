/*
 * io.c
 *
 *  Created on: Oct 20, 2014
 *      Author: Owner
 */

//#define F_CPU 16000000UL

#include <stdint.h>
#include <stdlib.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/pgmspace.h>
#include <util/delay.h>
#include "io.h"
#include "MAX528.h"

byte LEDS[4] = {0xff,0,0,0xff};
byte buttons[4] = {0,0,0,0};
int pots[16];
byte step_flag = 0;
int step_num = 0;


void io_setup() {
	cli();

	// Setup IO Pins
	DDRB = 0x0; // Button Input
	PORTB = 0x00;

	DDRD = 0b11111010; //INT0 in
	/*	PD0 MIDI In
		PD1 MIDI Out
		PD2-6 USB chip (IO)
		PD7 LED Serial Data (out)*/
	DDRA = 0b00001111;
	/* 	PA0-2 select pot from Mux (out)
		PA3 LED Shift Register clock (out)
		PA4 & PA6 Mux Output (in)
		PA5 & PA7 Single pots (in)*/
	DDRC = 0b11111111;
	/*	PC0-3 Button columns
		PC4 Button Row Buffer Enable
		PC5-7 LED Row Mux */
	PORTC = 0x00;

	//Analog in
	ADMUX = (0 << REFS0) | (1 << ADLAR); //VCC used as Voltage Reference, disconnected from PB0 (AREF)
	ADCSRA = (1 << ADEN) | (1 << ADPS2) | (0 << ADPS1) | (1 << ADPS0); //div 32

	GICR = (1<<INT0); //enable INT0
	MCUCR = (1<<ISC01)|(1<<ISC00); //rising edge interrupt

	max_init(); //setup DAC

	sei();
}

ISR(INT0_vect){
	step_flag = 1;//set flag to step
}

void step_poll(){
	if(step_flag){
		step_flag = 0;

		LEDS[1] = 0xff;
		LEDS[2] = 0xff;
		//set LEDs
		if (step_num < 4){ //0-3
			LEDS[1] = ~(1<<(step_num*2));
		} else if (step_num < 8){ //4-7
			LEDS[2] = ~(1<<((step_num-4)*2));
		} else if (step_num < 12){ //8-11
			LEDS[1] = ~(1<<((step_num-8)*2+1));
		} else{ //12-15
			LEDS[2] = ~(1<<((step_num-12)*2+1));
		}

		max_wr_command(0xFF, pots[step_num]);//DAC_value++);

		step_num++;
		step_num %= 16;
	}
}

void check_io(){

}

void check_pots(byte mux) {
	POT_MUX_PORT &= ~((1<<POT_MUX0)|(1<<POT_MUX1)|(1<<POT_MUX2)); //clear mux
	POT_MUX_PORT |= mux & ((1<<POT_MUX0)|(1<<POT_MUX1)|(1<<POT_MUX2)); //set mux

	int pin = POT_X1;
	if(mux >= 8)pin = POT_X0; //mux 2 is on another pin
	ADMUX = (pin & 0b111) | (1 << ADLAR)  | (1<<REFS0);
	ADCSRA |= (1 << ADSC);		//start
	while (!(ADCSRA & (1 << ADIF))){//wait for ADC
		step_poll(); //check if we should change step
	}
	ADCSRA |= (1 << ADIF); //stop
	pots[mux] = ADCH; //store value
}

void check_buttons() {
	byte button = 0;

	BUT_COL_PORT &= ~(1<<BUT_EN); //enable button buffer
	for (int i=0;i<4;i++){
		BUT_COL_PORT |= ((1<<BUT_COL0)|(1<<BUT_COL1)|(1<<BUT_COL2)|(1<<BUT_COL3));//clear
		BUT_COL_PORT &= ~(1<<i); //pull column low
		_delay_us(500);
		button = ~PINB;
		if(button != buttons[i]){
			if((button & 0b10) && i == 0){ //start button
				step_num = 0; //reset pattern
			}
		}
	}
}

void set_LEDs(byte col){
	byte led_col = LEDS[col];

	LED_MUX_PORT |= (1<<LED_MUX2); //set mux to non-visible
	LED_MUX_PORT &= ~((1<<LED_MUX0)|(1<<LED_MUX1)); //clear low bits of mux

	for (int x=0;x<8;x++){ //write byte to shift register
		LED_CLK_PORT &= ~(1<<LED_CLK); //clear clock
		LED_DATA_PORT &= ~(1<<LED_DATA); //clear data
		LED_DATA_PORT |= (led_col << x) & (1<<LED_DATA); //output data
		_delay_us(1);
		LED_CLK_PORT |= (1<<LED_CLK); //set clock(write)
	}

	LED_MUX_PORT |= (col<<LED_MUX0) & ((1<<LED_MUX0)|(1<<LED_MUX1)); //set mux column
	LED_MUX_PORT &= ~((1<<LED_MUX2)); //set mux to visible
}
