#ifndef monster_H_
#define monster_H_

// Library
#include <stdio.h>
#include <stdlib.h>
#include "SDL/SDL_image.h"
#include "../include/constant.h"




// Creates a new monster with a given number of available bombs
extern struct monster* monster_init(int bomb_number);
extern void 	monster_free(struct monster* monster);

// Monster's position
extern int 		monster_get_x(struct monster* monster);
extern int 		monster_get_y(struct monster* monster);
extern void 	monster_set_position ( struct monster* monster, int x, int y);

// Monster's way
extern void 	monster_set_current_way(struct monster* monster, enum way way);
extern int 		monster_get_current_way(struct monster* monster);

// Set, Increase, Decrease the number of bomb that monster can put
extern int 		monster_get_nb_bomb(struct monster* monster);
extern void 	monster_inc_nb_bomb(struct monster* monster);
extern void 	monster_dec_nb_bomb(struct monster* monster);

// Get and decrease the number of life
extern int 		monster_get_nb_life(struct monster* monster);
extern void 	monster_dec_nb_life(struct monster* monster);
extern void 	monster_set_nb_life(struct monster* monster,int nb_life);

// Load the monster position from the map
extern void 	monster_from_map(struct monster* monster, struct map* map,int map_nb);
extern int 		monster_on_map(struct monster* monster,int map);
extern int		monster_get_map(struct monster* monster);

// Move the monster according to the current direction
extern int 		monster_move(struct monster* monster, struct map* map);

// Manage the monster blink when he loses life
extern void monster_set_blink(struct monster* monster, unsigned long int blink);
extern unsigned long int monster_get_blink(struct monster* monster);

// Display the monster on the window
extern SDL_Rect	monster_display(struct monster* monster);

extern int 		monster_get_pop(struct monster* monster);
extern void 	monster_set_pop(struct monster* monster, int pop);

extern int 		monster_get_type(struct monster* monster);
extern void 	monster_set_type(struct monster* monster, int type);

#endif /* monster_H_ */
