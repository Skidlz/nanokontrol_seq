/*
 * MAX528.c
 *
 *  Created on: Sep 9, 2017
 *      Author: Zack
 */
#ifndef byte
#define byte uint8_t
#endif
#include <stddef.h>
#include <stdlib.h>
#include <avr/io.h>
#include <util/delay.h>
#include "MAX528.h"

void max_init(){
	MAXDDR |= (1<<MAX_DAT)|(1<<MAX_CLK)|(1<<MAX_CS);
	max_wr_command(0x00,0xff); //buffer all outs
}

void max_wr_byte(byte out_byte){
	for(int i = 0;i<8;i++){
	  MAXPORT &= ~(1<<MAX_CLK); //clock low
	  MAXPORT &= ~(1<<MAX_DAT); //clear data
	  MAXPORT |= (((out_byte&(0x80>>i))!=0)<<MAX_DAT); //write data
	  _delay_us(1);
	  MAXPORT |= (1<<MAX_CLK); //clock high
	}
}

void max_wr_command(byte addr, byte data){
	MAXPORT &= ~(1<<MAX_CS); //enable cs
	max_wr_byte(addr);
	max_wr_byte(data);
	MAXPORT |= (1<<MAX_CS); //disable cs
}
