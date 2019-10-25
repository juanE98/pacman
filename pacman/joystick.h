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
void initialise_joystic(void); 

//Current joystick direction 
uint8_t joystick_dir(void); 




#endif /* JOYSTICK_H_ */