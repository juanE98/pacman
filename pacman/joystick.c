/*
 * joystick.c
 *
 * Created: 10/23/2019 8:15:46 PM
 *  Author: juloe
 */ 

#include <avr/io.h>
#include <avr/interrupt.h>

#include "joystick.h"
#include "timer0.h"

static uint16_t adc_x, adc_y; 


void initialise_joystick(void){
	ADMUX = (1 <<REFS0); 
	ADCSRA= (1<<ADEN) | (1<<ADPS2) | (1<<ADPS1) ; 

}
static void get_adc(void){
	//x axis 
	ADMUX &= ~1; 
	//convert ADC 
	ADCSRA |= (1<<ADSC) ; 
	while(ADCSRA & (1<<ADSC)){
		
	}
	adc_x = ADC; 
	//y axis 
	
}

