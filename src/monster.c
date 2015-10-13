#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <stddef.h>
#include "SDL/SDL_image.h"

#include "../include/game.h"
#include "../include/misc.h"
#include "../include/monster.h"


struct monster {
	int x, y;
	int nb_life;
	int map;
	int just_pop;
	int type;
	unsigned long int blink;

	enum way current_way;
};

struct monster* monster_init(int nb_life) {
	struct monster* monster = malloc(sizeof(*monster));
	if (!monster)
		error("Memory error");

	monster->current_way = SOUTH;
	monster->nb_life=nb_life;
	monster->just_pop=0;
	monster->type=0;
	monster->blink=0;

	return monster;
}

int monster_get_type(struct monster* monster)
{
	assert(monster);
	return monster->type;
}

void monster_set_type(struct monster* monster, int type)
{
	assert(monster);
	monster->type = type;
}

void monster_free(struct monster* monster) {
	assert(monster);
	free(monster);
}

void monster_set_blink(struct monster* monster, unsigned long int blink) {
	assert(monster);
	monster->blink=blink;
}

unsigned long int monster_get_blink(struct monster* monster) {
	assert(monster);
	return monster->blink;
}

int monster_get_x(struct monster* monster) {
	assert(monster != NULL);
	return monster->x;
}

int monster_get_y(struct monster* monster) {
	assert(monster != NULL);
	return monster->y;
}

void monster_set_position ( struct monster* monster, int x, int y)
{
	assert(monster);
	monster->x=x;
	monster->y=y;
}

void monster_set_current_way(struct monster* monster, enum way way) {
	assert(monster);
	monster->current_way = way;
}

int monster_get_current_way(struct monster* monster) {
	assert(monster);
	return (int)monster->current_way;
}

void monster_from_map(struct monster* monster, struct map* map,int map_nb)
{
	int i, j;
	for (i = 0; i < map_get_width(map); i++)
	{
		for (j = 0; j < map_get_height(map); j++)
		{
			if (   map_get_cell_type(map, i, j) == CELL_MONSTER
				|| map_get_cell_type(map, i, j) == CELL_beast )
			{
				monster->x = i;
				monster->y = j;
				monster->map=map_nb;
				map_set_cell_type(map,i,j,CELL_EMPTY);
				return;
			}
		}
	}
}

static int monster_move_aux(struct monster* monster, struct map* map, int x1, int y1)
{
	if (!map_is_inside(map, x1, y1))
		return 0;

	switch (map_get_cell_type(map, x1, y1)) {
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

	case CELL_MONSTER:
		return 0;
		break;


	case CELL_PLAYER:
		return 0;
		break;

	case CELL_BOMB:
		return 0;
		break;

	case CELL_DOOR:
		return 0;
		break;

	case CELL_CLOSED_DOOR:
		return 0;
		break;

	case CELL_beast:
	case CELL_KEY:
		return 0;
		break;

	default:
		break;
	}

	// monster has moved
	return 1;
}

int monster_move(struct monster* monster, struct map* map) {
	int x = monster->x;
	int y = monster->y;
	int move = 0;

	switch (monster->current_way) {
	case NORTH:
		if (monster_move_aux(monster, map, x, y - 1)) {
			monster->y--;
			move = 1;
		}
		break;

	case SOUTH:
		if (monster_move_aux(monster, map, x, y + 1)) {
			monster->y++;
			move = 1;
		}
		break;

	case WEST:
		if (monster_move_aux(monster, map, x - 1, y)) {
			monster->x--;
			move = 1;
		}
		break;

	case EAST:
		if (monster_move_aux(monster, map, x + 1, y)) {
			monster->x++;
			move = 1;
		}
		break;
	}

	if (move) {
		map_set_cell_type(map, x, y, CELL_EMPTY);
		switch(monster->type)
		{
		case MONSTER:
			map_set_cell_type(map, monster->x, monster->y, CELL_MONSTER);
			break;
		case beast:
			map_set_cell_type(map, monster->x, monster->y, CELL_beast);
			break;

		}

	}
	return move;
}

void monster_dec_nb_life(struct monster* monster){
	assert(monster);
	monster->nb_life-=1;
}

int monster_get_nb_life(struct monster* monster) {
	assert(monster);
	return monster->nb_life;
}

void monster_set_nb_life(struct monster* monster,int nb_life)
{
	assert(monster);
	monster->nb_life=nb_life;
}

int monster_on_map(struct monster* monster,int map)
{
	assert(monster);
	if (map == monster->map)
		return 1;
	else
		return 0;
}

int monster_get_map(struct monster* monster)
{
	assert(monster);
	return monster->map;
}

int monster_get_pop(struct monster* monster)
{
	assert(monster);
	return monster->just_pop;
}

void monster_set_pop(struct monster* monster, int pop)
{
	assert(monster);
	monster->just_pop=pop;
}
