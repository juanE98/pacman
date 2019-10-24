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
static uint8_t prev_dir =0; 
static uint32_t prev_time; 

void initialise_joystick(void){
	//Setup ADC
	ADMUX = (1 <<REFS0); 
	ADCSRA= (1<<ADEN) | (1<<ADPS2) | (1<<ADPS1) ; 

}
static void get_ADCval(void){
	//x axis 
	ADMUX &= ~1; 
	//convert ADC 
	ADCSRA |= (1<<ADSC) ; 
	while(ADCSRA & (1<<ADSC)){
		; //wait conversion. 
	}
	adc_x = ADC;  //read value 
	
	//read y axis 
	ADMUX |= 1; 
	ADCSRA |= (1<<ADSC); 
	while (ADCSRA & (1<<ADSC)){
		; //wait
	}
	adc_y = ADC;  //read value 
}

uint8_t joystick_dir(void){
	// 1=up , 2=down , 3= left, 4= right, -1= middle 
	uint8_t direction; 
	uint8_t current_time; 
	get_ADCval(); 
	if (adc_x >768) {
		direction = 4 ; 
	} else if (adc_x < 257){
		direction = 3; 
	}else if (adc_y >768){
		direction =1; 
	}else if (adc_y <257){
		direction = 2; 
	}else {
		if (prev_dir == 3){
			// if adc_x < 386 then direction = direction, otherwise direction = middle
			direction = (adc_x <386 ? 3:-1) ; 
		}
		else if(prev_dir == 1){
			direction = (adc_y > 641 ? 1:-1); 
		}else if(prev_dir ==2){
			direction = (adc_y < 386 ? 2:-1); 
		}else if (prev_dir == 4) {
			direction =(adc_x > 641 ? 4:-1); 
		}else{
			direction = prev_dir ; 
		}
	}
	if (direction >0){
		if(prev_dir == direction){
			current_time = get_current_time(); 
			if (current_time < prev_time + 300){
				return -1 ; 
			}
		}
		prev_time = get_current_time() ; 
		prev_dir = direction ; 
	}
	return direction ; 


}