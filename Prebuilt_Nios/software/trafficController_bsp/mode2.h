/*
 * mode2.h
 *
 *  Created on: 16/09/2020
 *      Author: Jacob
 */

#ifndef MODE2_H_
#define MODE2_H_

extern unsigned int pedNS, pedEW, NSHandled, EWHandled;


void init_buttons_pio();
void pedestrian_tlc(void* timerContext, unsigned int state);
void NSEW_ped_isr(int* temp);

#endif /* MODE2_H_ */
