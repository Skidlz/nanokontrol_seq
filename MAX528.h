/*
 * MAX528.h
 *
 *  Created on: Sep 9, 2017
 *      Author: Zack
 */

#ifndef MAX528_H_
#define MAX528_H_

#ifndef byte
#define byte uint8_t
#endif

#define MAXDDR DDRD
#define MAXPORT PORTD
#define MAX_CLK 6
#define MAX_DAT 7
#define MAX_CS 5

void max_init();
void max_wr_byte(byte);
void max_wr_command(byte, byte);

#endif /* MAX528_H_ */
