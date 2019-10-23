/*
 * score.c
 *
 * Written by Peter Sutton
 */

#include "score.h"

uint32_t score;
uint32_t high_score;
 


void init_score(void) {
	score = 0;
}


void add_to_score(uint16_t value) {
	score += value;
}

uint32_t get_score(void) {
	return score;
}

void save_highscore (void) {
	if (score > high_score) {
		high_score = score ; 
	}
	
}

uint32_t get_highscore(void){
		return high_score ; 	
}