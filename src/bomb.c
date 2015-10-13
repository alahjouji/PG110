#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <assert.h>
#include <SDL/SDL_image.h>

#include <time.h>
#include "../include/bomb.h"
#include "../include/constant.h"
#include "../include/misc.h"
#include "../include/map.h"
#include "../include/sprite.h"
#include "../include/window.h"


// Structure's definition
struct bomb{
	int display; // sort of boolean, indicates if the bomb is displayed or not.
	int x, y;
	int state_num;
	int timer;
	int map;
	enum e_state current_state;
};

struct bomb* bomb_init() {
	struct bomb* bomb = malloc(sizeof(*bomb));
	assert(bomb);
	if (!bomb)
		error("Memory error");

	bomb->current_state = FIRST;
	bomb->x=0;
	bomb->y=0;
	bomb->timer = 0;
	bomb->state_num = 0;
	bomb->display=0;
	bomb->map=-1;

	return bomb;
}

void bomb_free(struct bomb* bomb) {
	assert(bomb);

	free(bomb);
}


void bomb_display(struct bomb* bomb,struct map* map,int range) {
	assert(bomb);
	if (bomb->display)
	{
		map_set_cell_type(map, bomb->x, bomb->y, CELL_BOMB);


		if ( bomb->current_state != FIRE )
		{
			window_display_image(sprite_get_bomb(bomb->current_state),
						                bomb->x * SIZE_BLOC, bomb->y * SIZE_BLOC);
		}
		else
		{
			map_set_cell_type(map, bomb->x, bomb->y, CELL_EMPTY);


			int i;
			// Above the bomb
				for ( i=1 ; i <= range ; i++ )
				{
					if(bomb_case_explode(bomb,map,bomb->x,bomb->y-i))
					{
					            window_display_image(sprite_get_bomb(bomb->current_state),
					                bomb->x * SIZE_BLOC, (bomb->y - i) * SIZE_BLOC);
					}
					else
						break;
				}
			// Right side of the bomb
				for ( i=1 ; i <= range ; i++ )
				{
					if(bomb_case_explode(bomb,map,bomb->x+i,bomb->y))
					{
						window_display_image(sprite_get_bomb(bomb->current_state),
					      (bomb->x + i) * SIZE_BLOC, bomb->y * SIZE_BLOC);
					}
					else
						break;
				}
			// Bottom of the bomb
				for ( i=1 ; i <= range ; i++ )
				{
					if(bomb_case_explode(bomb,map,bomb->x,bomb->y+i))
					{
						window_display_image(sprite_get_bomb(bomb->current_state),
					      bomb->x * SIZE_BLOC, (bomb->y + i) * SIZE_BLOC);
					}
					else
						break;
				}
			// Left side of the bomb
				for ( i=1 ; i <= range ; i++ )
				{
					if(bomb_case_explode(bomb,map,bomb->x-i,bomb->y))
					{
						window_display_image(sprite_get_bomb(bomb->current_state),
					     (bomb->x - i) * SIZE_BLOC, bomb->y * SIZE_BLOC);
					}
					else
						break;
				}
				 window_display_image(sprite_get_bomb(bomb->current_state),
						bomb->x * SIZE_BLOC, bomb->y * SIZE_BLOC);


		}
	}
}

int bomb_case_explode(struct bomb* bomb, struct map* map, int x, int y) {
	assert(bomb);
	assert(map);
if (!map_is_inside(map, x, y))
		return 0;

	switch (map_get_cell_type(map, x, y)) {
	case CELL_SCENERY:
	case CELL_GOAL:
	case CELL_DOOR:
	case CELL_CLOSED_DOOR:
	case CELL_KEY:
		return 0;
		break;

	case CELL_BONUS:
		return 1;
		break;

	case CELL_CASE:
		map_display_random(map,x,y);
		return 1;
		break;

	case CELL_MONSTER:
	case CELL_PLAYER:
		return 1;
		break;

	case CELL_BOMB:
		return 0;
		break;

	default:
		break;
	}
	return 1;
}

void bomb_erase(struct bomb* bomb) {
	assert(bomb);
	bomb->display = 0;
}

void bomb_update(struct bomb* bomb){
	assert(bomb);

	if ( bomb->state_num==5 || bomb->current_state == DESTROY)
		bomb_erase(bomb);
	else
	{
		window_display_image(sprite_get_bomb(bomb->current_state),
	      bomb->x * SIZE_BLOC, bomb->y * SIZE_BLOC);
	}
}

void bomb_set_display(struct bomb* bomb,int display)
{
	assert(bomb);
	bomb->display=display;
}

void bomb_set_position(struct bomb* bomb, int x, int y,int map_nb)
{
	assert(bomb);
	bomb->x=x;
	bomb->y=y;
	bomb->map=map_nb;
}

void bomb_set_x(struct bomb* bomb, int x)
{
	assert(bomb);
	bomb->x=x;
}

void bomb_set_y(struct bomb* bomb, int y)
{
	assert(bomb);
	bomb->y=y;
}

int bomb_get_x(struct bomb* bomb)
{
	assert(bomb);
	return bomb->x;
}

int  bomb_get_y(struct bomb* bomb)
{
	assert(bomb);
	return bomb->y;
}

void bomb_inc_y(struct bomb* bomb, struct map* map){
	assert(bomb);
	map_set_cell_type(map, bomb->x, bomb->y, CELL_EMPTY);
	bomb->y--;
	map_set_cell_type(map, bomb->x, bomb->y, CELL_BOMB);
}

void bomb_dec_y(struct bomb* bomb, struct map* map){
	assert(bomb);
	map_set_cell_type(map, bomb->x, bomb->y, CELL_EMPTY);
	bomb->y++;
	map_set_cell_type(map, bomb->x, bomb->y, CELL_BOMB);
}

void bomb_inc_x(struct bomb* bomb, struct map* map){
	assert(bomb);
	map_set_cell_type(map, bomb->x, bomb->y, CELL_EMPTY);
	bomb->x--;
	map_set_cell_type(map, bomb->x, bomb->y, CELL_BOMB);
}

void bomb_dec_x(struct bomb* bomb, struct map* map){
	assert(bomb);
	map_set_cell_type(map, bomb->x, bomb->y, CELL_EMPTY);
	bomb->x++;
	map_set_cell_type(map, bomb->x, bomb->y, CELL_BOMB);
}

void bomb_set_state(struct bomb* bomb, enum e_state state) {
	assert(bomb);
	bomb->current_state = state;
}

int bomb_get_state(struct bomb* bomb){
	assert(bomb);
	return bomb->current_state;
}

int bomb_get_timer(struct bomb* bomb){
	assert(bomb);
	return bomb->timer;
}

void bomb_set_timer( struct bomb* bomb , int timer )
{
	assert(bomb);
	bomb->timer=timer;
}

int bomb_get_display(struct bomb* bomb)
{
	assert(bomb);
	return bomb->display;
}

void bomb_reset_state(struct bomb* bomb)
{
	assert(bomb);
	bomb->state_num=-1 ;
}
void bomb_inc_state(struct bomb* bomb)
{
	assert(bomb);

	if(bomb->state_num < 5)
		bomb->state_num++;
	else
		bomb->state_num=0;

	switch(bomb->state_num)
	{
	case 0:
		bomb->current_state = FIRST;
		break;
	case 1:
		bomb->current_state = SECOND;
		break;
	case 2:
		bomb->current_state = THIRD;
		break;
	case 3:
		bomb->current_state =  FOURTH;
		break;
	case 4:
		bomb->current_state = FIRE;
		break;
	case 5:
		bomb->current_state = DESTROY;
		break;
	default:
		bomb->current_state = ERROR;
		break;
	}
	bomb_update(bomb);
}

int bomb_move_aux(struct bomb* bomb, struct map* map, int x, int y) {


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
		return 0;
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
	case CELL_beast:
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

	// Bomb has moved
	return 1;
}


int bomb_on_map(struct bomb* bomb,int map)
{
	assert(bomb);
	if (map == bomb->map)
		return 1;
	else
		return 0;
}

int bomb_get_map(struct bomb* bomb)
{
	assert(bomb);
	return bomb->map;
}
