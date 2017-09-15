/*
 * io.h
 *
 *  Created on: Oct 20, 2014
 *      Author: Owner
 */
#include <stdint.h>
#ifndef byte
#define byte uint8_t
#endif
#ifndef IO_H_
#define IO_H_

//PORTB
#define BUT_PORT PORTB
#define BUT_PIN PINB
//PORTD
#define LED_DATA_PORT PORTD
#define LED_DATA PD7
//PORTA
#define POT_MUX_PORT PORTA
#define LED_CLK_PORT PORTA
#define POT_MUX0 PA0
#define POT_MUX1 PA1
#define POT_MUX2 PA2
#define LED_CLK PA3

#define POT_X0 PA4
#define POT0 PA5
#define POT_X1 PA6
#define POT1 PA7

//PORTC
#define BUT_COL_PORT PORTC
#define LED_MUX_PORT PORTC
#define BUT_COL0 PC0
#define BUT_COL1 PC1
#define BUT_COL2 PC2
#define BUT_COL3 PC3
#define BUT_EN PC4

#define LED_MUX0 PC5
#define LED_MUX1 PC6
#define LED_MUX2 PC7


void io_setup();
void check_io();
void check_pots(byte);
void check_buttons();
void set_LEDs(byte col);
void step_poll();


#endif /* IO_H_ */
