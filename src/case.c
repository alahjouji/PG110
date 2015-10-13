#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <stddef.h>
#include "SDL/SDL_image.h"

#include "../include/game.h"
#include "../include/misc.h"
#include "../include/case.h"
#include "../include/player.h"
#include "../include/map.h"



void case_inc_y(struct player* player, struct map* map){
	assert(player);
	map_set_cell_type(map, player_get_x(player), player_get_y(player)-1, CELL_EMPTY);
	map_set_cell_type(map, player_get_x(player), player_get_y(player)-2, CELL_CASE);
}

void case_dec_y(struct player* player, struct map* map){
	assert(player);
	map_set_cell_type(map, player_get_x(player), player_get_y(player)+1, CELL_EMPTY);
	map_set_cell_type(map, player_get_x(player), player_get_y(player)+2, CELL_CASE);
}

void case_inc_x(struct player* player, struct map* map){
	assert(player);
	map_set_cell_type(map, player_get_x(player)-1, player_get_y(player), CELL_EMPTY);
	map_set_cell_type(map, player_get_x(player)-2, player_get_y(player), CELL_CASE);
}

void case_dec_x(struct player* player, struct map* map){
	assert(player);
	map_set_cell_type(map, player_get_x(player)+1, player_get_y(player), CELL_EMPTY);
	map_set_cell_type(map, player_get_x(player)+2, player_get_y(player), CELL_CASE);
}

int case_move_aux(struct map* map, int x, int y) {

	if (!map_is_inside(map, x, y))
		return 0;

	switch (map_get_cell_type(map, x, y)) {
	case CELL_SCENERY:
		return 0;
		break;

	case CELL_CASE:
		return 0;
		break;

	case CELL_BONUS:
		break;

	case CELL_GOAL:
		return 0;
		break;

	case CELL_DOOR:
		return 0;
		break;

	case CELL_CLOSED_DOOR:
			return 0;
		break;

	case CELL_MONSTER:
		return 0;
		break;

	case CELL_PLAYER:
		return 0;
		break;

	case CELL_BOMB:
		return 0;
		break;

	case CELL_KEY:
		return 0;
		break;

	default:
		break;
	}

	// Case has moved case
	return 1;
}



