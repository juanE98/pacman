/*
 * project.c
 *
 * Main file
 *
 * Author: Peter Sutton. Modified by <YOUR NAME HERE>
 */ 

#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/pgmspace.h>
#include <stdio.h>

#include "ledmatrix.h"
#include "scrolling_char_display.h"
#include "buttons.h"
#include "serialio.h"
#include "terminalio.h"
#include "score.h"
#include "timer0.h"
#include "game.h"

#define F_CPU 8000000L
#include <util/delay.h>

// Function prototypes - these are defined below (after main()) in the order
// given here
void initialise_hardware(void);
void splash_screen(void);
void new_game(void);
void play_game(void);
void handle_level_complete(void);
void handle_game_over(void);

// ASCII code for Escape character
#define ESCAPE_CHAR 27

/////////////////////////////// main //////////////////////////////////
int main(void) {
	// Setup hardware and call backs. This will turn on 
	// interrupts.
	initialise_hardware();
	
	// Show the splash screen message. Returns when display
	// is complete
	splash_screen();
	
	while(1) {
		new_game();
		play_game();
		handle_game_over();
	}
}

void initialise_hardware(void) {
	ledmatrix_setup();
	init_button_interrupts();
	// Setup serial port for 19200 baud communication with no echo
	// of incoming characters
	init_serial_stdio(19200,0);
	
	init_timer0();
	
	// Turn on global interrupts
	sei();
}

void splash_screen(void) {
	// Clear terminal screen and output a message
	clear_terminal();
	move_cursor(10,10);
	printf_P(PSTR("Pac-Man"));
	move_cursor(10,12);
	printf_P(PSTR("CSSE2010 project by <Juan Espares>"));
	move_cursor(10,14); 
	printf_P(PSTR("Student Number: 44317962")) ; 

	// Output the scrolling message to the LED matrix
	// and wait for a push button to be pushed.
	ledmatrix_clear();
	while(1) {
		set_scrolling_display_text("44317962", COLOUR_GREEN);
		// Scroll the message until it has scrolled off the 
		// display or a button is pushed
		while(scroll_display()) {
			_delay_ms(150);
			if(button_pushed() != NO_BUTTON_PUSHED) {
				ledmatrix_clear();
				return;
			}
		}
	}
}

void new_game(void) {
	// Initialise the game and display
	initialise_game();
	
	// Initialise the score
	init_score();
	init_score();
	
	
	// Clear a button push or serial input if any are waiting
	// (The cast to void means the return value is ignored.)
	(void)button_pushed();
	clear_serial_input_buffer();
}

void play_game(void) {
	uint32_t current_time;
	uint32_t pacman_last_move_time;
	uint32_t ghost_last_move_time;
	uint32_t ghost_last_move_time1; 
	uint32_t ghost_last_move_time2; 
	uint32_t ghost_last_move_time3; 
	
	int8_t button;
	char serial_input, escape_sequence_char;
	uint8_t characters_into_escape_sequence = 0;
	
	// Get the current time and remember this as the last time the projectiles
    // were moved.
	current_time = get_current_time();
	pacman_last_move_time = current_time;
	ghost_last_move_time = current_time;
	ghost_last_move_time1 = current_time; 
	ghost_last_move_time2 = current_time; 
	ghost_last_move_time3 = current_time;
	 
	
	// We play the game until it's over
	while(!is_game_over()) {
		// Check for input - which could be a button push or serial input.
		// Serial input may be part of an escape sequence, e.g. ESC [ D
		// is a left cursor key press. At most one of the following three
		// variables will be set to a value other than -1 if input is available.
		// (We don't initalise button to -1 since button_pushed() will return -1
		// if no button pushes are waiting to be returned.)
		// Button pushes take priority over serial input. If there are both then
		// we'll retrieve the serial input the next time through this loop
		serial_input = -1;
		escape_sequence_char = -1;
		button = button_pushed();
		
		if(button == NO_BUTTON_PUSHED) {
			// No push button was pushed, see if there is any serial input
			if(serial_input_available()) {
				// Serial data was available - read the data from standard input
				serial_input = fgetc(stdin);
				// Check if the character is part of an escape sequence
				if(characters_into_escape_sequence == 0 && serial_input == ESCAPE_CHAR) {
					// We've hit the first character in an escape sequence (escape)
					characters_into_escape_sequence++;
					serial_input = -1; // Don't further process this character
				} else if(characters_into_escape_sequence == 1 && serial_input == '[') {
					// We've hit the second character in an escape sequence
					characters_into_escape_sequence++;
					serial_input = -1; // Don't further process this character
				} else if(characters_into_escape_sequence == 2) {
					// Third (and last) character in the escape sequence
					escape_sequence_char = serial_input;
					serial_input = -1;  // Don't further process this character - we
										// deal with it as part of the escape sequence
					characters_into_escape_sequence = 0;
				} else {
					// Character was not part of an escape sequence (or we received
					// an invalid second character in the sequence). We'll process 
					// the data in the serial_input variable.
					characters_into_escape_sequence = 0;
				}
			}
		}
		
		// Process the input. 
		if(button==3 || escape_sequence_char=='A') {
			// Button 3 pressed OR left cursor key escape sequence completed 
			// Attempt to move left
			change_pacman_direction(DIRN_LEFT);
		} else if(button==2 || escape_sequence_char=='W') {
			// Button 2 pressed or up cursor key escape sequence completed
			// Attempt to move up 
			change_pacman_direction(DIRN_UP);
		} else if(button==1 || escape_sequence_char=='S') {
			// Button 1 pressed OR down cursor key escape sequence completed
			// Attempt to move down
			change_pacman_direction(DIRN_DOWN); 
		} else if(button==0 || escape_sequence_char=='D') {
			// Button 0 pressed OR right cursor key escape sequence completed 
			// Attempt to move right
			change_pacman_direction(DIRN_RIGHT);
		} else if(serial_input == 'p' || serial_input == 'P') {
			// Unimplemented feature - pause/unpause the game until 'p' or 'P' is
			// pressed again
		} 
		else if (button == 'n' || button == 'N'){
			//New Game		
			new_game(); 
			
		}
		
		// else - invalid input or we're part way through an escape sequence -
		// do nothing
		
		current_time = get_current_time();
		if(!is_game_over() && current_time >= pacman_last_move_time + 400) {
			// 400ms (0.4 second) has passed since the last time we moved 
			// the pac-man - move it.
			move_pacman();
			pacman_last_move_time = current_time;
			
			// Check if the move finished the level - and restart if so
			if(is_level_complete()) {
				handle_level_complete();	// This will pause until a button is pushed
				initialise_game_level();
				// Update our timers since we have a pause above
				pacman_last_move_time = ghost_last_move_time = get_current_time();
			}
		}
		if(!is_game_over() && current_time >= ghost_last_move_time + 420) {
			// 500ms (0.5 second) has passed since the last time we moved the
			// ghosts - move them
			move_ghost(0);
			ghost_last_move_time = current_time;
		}
		if (!is_game_over() && current_time >= ghost_last_move_time1 + 450){
			move_ghost(1); 
			ghost_last_move_time1 = current_time; 
		}
		if (!is_game_over() && current_time >= ghost_last_move_time2 + 500){
			move_ghost(2); 
			ghost_last_move_time2 = current_time; 
		}
		if (!is_game_over() && current_time >= ghost_last_move_time3 + 570){
			move_ghost(3); 
			ghost_last_move_time3 = current_time; 
		}
		
		
		
		
	}
	// We get here if the game is over.
}

void handle_level_complete(void) {
	move_cursor(35,10);
	printf_P(PSTR("Level complete"));
	move_cursor(35,11);
	printf_P(PSTR("Push a button or key to continue"));
	// Clear any characters in the serial input buffer - to make
	// sure we only use key presses from now on.
	clear_serial_input_buffer();
	while(button_pushed() == NO_BUTTON_PUSHED && !serial_input_available()) {
		; // wait
	}
	// Throw away any characters in the serial input buffer
	clear_serial_input_buffer();

}

void handle_game_over(void) {
	move_cursor(35,14);
	printf_P(PSTR("GAME OVER"));
	move_cursor(35,16);
	printf_P(PSTR("Press a button to start again"));
	while(button_pushed() == NO_BUTTON_PUSHED) {
		; // wait
	}
	
}

