/*
 * score.h
 * 
 * Author: Peter Sutton
 */

#ifndef SCORE_H_
#define SCORE_H_

#include <stdint.h>

void init_score(void);


void add_to_score(uint16_t value);
uint32_t get_score(void);
uint32_t get_highscore(void); 
void set_highscore(uint32_t value); 

#endif /* SCORE_H_ */