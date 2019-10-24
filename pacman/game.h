/*
** game.h
**
** Written by Peter Sutton
**
** Function prototypes for those functions available externally.
*/

#ifndef GAME_H_
#define GAME_H_

#include <inttypes.h>

// The game field is 31 rows in size by 31 columns.
// The row number (y) ranges from 0 (top) to 30 (bottom)
// The column number (x) ranges from 0 (left) to 30 (right)
#define FIELD_HEIGHT 31
#define FIELD_WIDTH 31

// Number of ghosts in the game
#define NUM_GHOSTS 4

#define NUM_DIRECTION_VALUES 4
// Arguments that can be passed to change_pacman_direction() below
#define DIRN_LEFT 0
#define DIRN_UP 1
#define DIRN_RIGHT 2
#define DIRN_DOWN 3

// Arguments that can be passed to 

// Initialise the game and output the initial display.
void initialise_game(void); 

// Initialise the game level - re-outputs the game field
// and restores all positions to their original values. This 
// function is called by initialise_game() above and only 
// needs to be called again if a new level is started.
void initialise_game_level(void);

// Attempt to move the pacman in its current direction. Returns 1 if successful, 
// 0 otherwise (e.g. there is a wall in the way, or the pacman would move into
// a ghost). Nothing happens if the game is over. (0 is returned.)
int8_t move_pacman(void);

// Attempt to change the direction of the pacman. Returns 1 if successful (i.e. 
// next call to move_pacman() would succeed, 0 otherwise (e.g. there is a wall
// in the way.)
// The "direction" argument takes on one of the values above.
// Nothing happens if the game is over (0 is returned.)
int8_t change_pacman_direction(int8_t direction);

// Attempt to move a ghost (ghostnum is 0 to NUM_GHOSTS - 1).
// The direction is chosen based on the location of the ghost
// and the location of the pacman and which ghost this is.
// (Different ghosts have different behaviours.)
// Nothing happens if the game is over.
void move_ghost(int8_t ghostnum);

// Returns 1 if the game is over, 0 otherwise
// Must only be called after initialise_game().
int8_t is_game_over(void);

// Returns 1 if the level is complete (all pac-dots eaten), 0 otherwise
// Must only be called after initialise_game().
int8_t is_level_complete(void);

//Return number of lives pacman has left
uint8_t get_lives(void); 

//Change pacman's lives
void set_lives(int8_t num); 

//Reset lives to MAX 
void reset_lives(void); 



#endif
