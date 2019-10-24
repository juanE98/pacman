/*
 * Joystick.h
 *
 * Created: 10/23/2019 8:13:50 PM
 *  Author: Juan Espares
 */ 


#ifndef INCFILE1_H_
#define INCFILE1_H_

#include <stdint.h>

// ADC Setup 
void init_joystick(void); 

//Current joystick direction 
uint8_t dir_joystick(void); 




#endif /* JOYSTICK_H_ */