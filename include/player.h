#ifndef PLAYER_H_
#define PLAYER_H_

// Libraries
#include <stdio.h>
#include <stdlib.h>
#include "SDL/SDL_image.h"
#include "../include/constant.h"
#include "../include/bomb.h"
#include "../include/list.h"
#include "../include/map.h"



// Creates a new player with a given number of available bombs
extern struct player* player_init(int bomb_number, int life_number);
extern void player_free(struct player* player);
extern void player_set_variables(struct player* player, int nb_bomb,int bomb_range,int nb_key,int nb_life,int map_progression, int level);

// Returns the current position of the player
extern int 	player_get_x(struct player* player);
extern int 	player_get_y(struct player* player);

//
extern void player_inc_y(struct player* player, struct map* map);
extern void player_inc_x(struct player* player, struct map* map);
extern void player_dec_y(struct player* player, struct map* map);
extern void player_dec_x(struct player* player, struct map* map);

// Set the direction of the next move of the player
extern void 		player_set_current_way(struct player* player, enum way way);
extern enum way 	player_get_current_way(struct player* player);

//Get the life number and dec it
extern int 	player_get_nb_life(struct player* player);
extern int 	player_dec_nb_life(struct player* player);
extern void player_inc_nb_life(struct player* player);
extern void player_set_nb_life(struct player* player, int nb_life);

// Manage the player win
extern void player_set_win(struct player* player,int win);
extern int 	player_get_win(struct player* player);

// Get the case if the player gets a key
extern int 	player_get_key(struct player* player);
extern void player_set_nb_key(struct player* player, int nb_key);

// Manage the progression in the level ie on the different map
extern void player_set_map(struct player* player,char door);
extern int 	player_get_map(struct player* player);
extern int	player_map_is_updated(struct player* player);
extern void player_map_untrig(struct player* player);
extern void player_map_update(struct player* player);

// Manage the player's level progression
extern void player_up_level(struct player* player);
extern int 	player_get_level(struct player* player);

// Load the player position from the map
extern void player_from_map(struct player* player, struct map* map);

// Move the player according to the current direction
extern int 	player_move(struct player* player, struct map* map);

// Display the player on the window
extern void player_display(struct player* player,struct map* map,int map_nb);

// Manage the player blink when he loses life
extern void player_set_blink(struct player* player, unsigned long int blink);
extern unsigned long int player_get_blink(struct player* player);

// Bomb Management
	// Set, Increase, Decrease the number of bomb that player can put
	extern int 	player_get_nb_bomb(struct player* player);
	extern void player_inc_nb_bomb(struct player* player);
	extern void player_dec_nb_bomb(struct player* player);
	/*extern void player_set_nb_bomb(struct player* player, int nb_bomb);*/

	// Manage a bomb
	extern void player_set_bomb(struct player* player, struct map* map,int map_nb);
	extern struct bomb* player_get_bomb(struct player* player, int bomb_number);

	// Manage bomb's range
	extern void player_set_bomb_range(struct player* player,int range);
	extern void player_inc_bomb_range(struct player* player);
	extern void player_dec_bomb_range(struct player* player);
	extern int  player_get_bomb_range(struct player* player);

	// Manage bomb's list
	extern struct list* player_get_list_bomb(struct player* player);

#endif /* PLAYER_H_ */
