#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <stddef.h>
#include "SDL/SDL_image.h"

#include "../include/game.h"
#include "../include/misc.h"
#include "../include/monster.h"
#include "../include/list.h"
#include "../include/map.h"

struct player {
	int x, y;
	int level;
	int map_progression;
	int map_up;
	int win;
	int nb_key;
	int nb_life;
	int nb_bomb;
	int bomb_range;
	unsigned long int blink; // unsigned long int in order not to have overflow when the game last to long
	enum way current_way;
	struct list* list_bomb;
};

void player_set_nb_life(struct player* player, int nb_life)
{
	assert(player);
	player->nb_life=nb_life;
}

void player_set_nb_key(struct player* player, int nb_key)
{
	assert(player);
	player->nb_key=nb_key;
}

struct player* player_init(int bomb_number, int life_number) {
	struct player* player = malloc(sizeof(*player));
	if (!player)
	{
		printf("Cannot malloc player in player_init\n");
		error("Memory error");
	}

	player->current_way = SOUTH;
	player->nb_key = 0;
	player->nb_bomb = bomb_number;
	player->nb_life = life_number;

	player->blink = 0;

	player->win=0;

	player->map_progression=0;
	player->map_up=0;

	player->level = 0;

	player->bomb_range=BOMB_RANGE_MIN;
	player->list_bomb = list_new(bomb_free);
	for (int i=1 ; i<=player->nb_bomb ; i++)
		list_add_tail(player->list_bomb,bomb_init()) ;

	return player;
}

void player_free(struct player* player) {
	assert(player);
	list_free(player->list_bomb);
	free(player);
}

int player_get_x(struct player* player) {
	assert(player != NULL);
	return player->x;
}

int player_get_y(struct player* player) {
	assert(player != NULL);
	return player->y;
}

void player_set_blink(struct player* player, unsigned long int blink) {
	assert(player);
	player->blink=blink;
}

unsigned long int player_get_blink(struct player* player) {
	assert(player);
	return player->blink;
}

void player_set_current_way(struct player* player, enum way way) {
	assert(player);
	player->current_way = way;
}

enum way 	player_get_current_way(struct player* player)
{
	assert(player);
	return player->current_way;
}

int player_get_nb_bomb(struct player* player) {
	assert(player);
	return player->nb_bomb;
}

int player_dec_nb_life(struct player* player){
	assert(player);
	player->nb_life-=1;
	return player->nb_life;
}

void player_inc_nb_life(struct player* player){
	if(player->nb_life != 9)
	player->nb_life++;
}

int player_get_nb_life(struct player* player) {
	assert(player);
	return player->nb_life;
}

int	player_map_is_updated(struct player* player)
{
	assert(player);
	return player->map_up;
}

void player_map_untrig(struct player* player)
{
	assert(player);
	player->map_up=0;
}

void player_map_update(struct player* player)
{
	assert(player);
	player->map_up=1;
}

void player_set_map(struct player* player,char door)
{
	assert(player);
	player->map_progression=(door & 112)>>4;
	player_map_update(player);
}

int player_get_map(struct player* player)
{
	assert(player);
	return player->map_progression;
}

void player_up_level(struct player* player)
{
	assert(player);
	player->level++;
}

int player_get_level(struct player* player)
{
	assert(player);
	return player->level;
}

void player_inc_nb_bomb(struct player* player) {
	assert(player);
	if(player->nb_bomb < 9)
	{
		list_add_tail(player->list_bomb,bomb_init());
		player->nb_bomb++;
	}
}

void player_dec_nb_bomb(struct player* player)
{
	assert(player);
	if(player->nb_bomb > 1)
	{
		player->nb_bomb --;
		list_free_element_tail(player->list_bomb);
	}
}

int player_get_key(struct player* player){
	assert(player);
	return player->nb_key;
}

void player_from_map(struct player* player, struct map* map) {
	int i, j;
	for (i = 0; i < map_get_width(map); i++) {
		for (j = 0; j < map_get_height(map); j++) {
			if (map_get_cell_type(map, i, j) == CELL_PLAYER) {
				player->x = i;
				player->y = j;
				return;
			}
		}
	}
}

void player_inc_y(struct player* player, struct map* map){
	assert(player);
	map_set_cell_type(map, player->x, player->y, CELL_EMPTY);
	player->y--;
	map_set_cell_type(map, player->x, player->y, CELL_PLAYER);
}

void player_inc_x(struct player* player, struct map* map){
	assert(player);
	map_set_cell_type(map, player->x, player->y, CELL_EMPTY);
	player->x--;
	map_set_cell_type(map, player->x, player->y, CELL_PLAYER);
}

void player_dec_y(struct player* player, struct map* map){
	assert(player);
	map_set_cell_type(map, player->x, player->y, CELL_EMPTY);
	player->y++;
	map_set_cell_type(map, player->x, player->y, CELL_PLAYER);
}

void player_dec_x(struct player* player, struct map* map){
	assert(player);
	map_set_cell_type(map, player->x, player->y, CELL_EMPTY);
	player->x++;
	map_set_cell_type(map, player->x, player->y, CELL_PLAYER);
}

void player_set_win(struct player* player,int win)
{
	assert(player);
	player->win = win;
}

int player_get_win(struct player* player)
{
	assert(player);
	return player->win;
}

void player_get_bonus(struct player* player, struct map* map, int x, int y){
	switch (map_gebonus_type_t(map, x, y)) {
	case BONUS_BOMB_RANGE_INC:
		player_inc_bomb_range(player);
		break;

	case BONUS_BOMB_RANGE_DEC:
		player_dec_bomb_range(player);
		break;

	case BONUS_BOMB_NB_INC:
		player_inc_nb_bomb(player);
		break;

	case BONUS_BOMB_NB_DEC:
		player_dec_nb_bomb(player);
		break;

	case BONUS_LIFE:
		player_inc_nb_life(player);
		break;
	case BONUS_MONSTER:
		break;
	}
}


static int player_move_aux(struct player* player, struct map* map, int x1, int y1) {


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
		player_get_bonus(player, map, x1, y1);
		break;

	case CELL_GOAL:
		if ( player->level == 2)
			player_set_win(player,1);
		player_up_level(player);
		return 1;
		break;

	case CELL_DOOR:
		player_set_map(player,map_get_door_type(map,x1,y1));
		return 0;
		break;

	case CELL_CLOSED_DOOR:
		if(player_get_key(player) == 1)
		{
			player_set_map(player,map_get_door_type(map,x1,y1));
			player->nb_key -- ;
			return 0;
		}
		else
		{
			return 0;
		}
		break;

	case CELL_MONSTER:
	case CELL_beast:
		return 0;
		break;

	case CELL_PLAYER:
		return 0;
		break;

	case CELL_BOMB:
		return 0;
		break;

	case CELL_KEY:
		player->nb_key ++ ;
		return 1;
		break;

	default:
		break;
	}

	// Player has moved
	return 1;
}

int player_move(struct player* player, struct map* map) {
	int x = player->x;
	int y = player->y;
	int move = 0;

	switch (player->current_way) {
	case NORTH:
		if (player_move_aux(player, map, x, y - 1)) {
			player->y--;
			move = 1;
		}
		break;

	case SOUTH:
		if (player_move_aux(player, map, x, y + 1)) {
			player->y++;
			move = 1;
		}
		break;

	case WEST:
		if (player_move_aux(player, map, x - 1, y)) {
			player->x--;
			move = 1;
		}
		break;

	case EAST:
		if (player_move_aux(player, map, x + 1, y)) {
			player->x++;
			move = 1;
		}
		break;
	}

	if (move) {
		map_set_cell_type(map, x, y, CELL_EMPTY);
		map_set_cell_type(map, player->x, player->y, CELL_PLAYER);
	}
	return move;
}

void player_display(struct player* player,struct map* map,int map_nb) {
	assert(player);

	struct bomb* bomb;
	window_display_image(sprite_get_player(player->current_way),
				player->x * SIZE_BLOC, player->y * SIZE_BLOC);
	for(int i=1;i<=list_get_length(player->list_bomb);i++)
	{
		bomb = list_get_value(player->list_bomb,i);
		if (bomb_get_display(bomb) == 1)
			if(bomb_on_map(bomb , map_nb))
				bomb_display(bomb,map,player->bomb_range);
	}
}

void player_set_bomb(struct player* player, struct map* map,int map_nb)
{
	if (player->nb_bomb>0)
	{
		struct bomb* bomb;
		for (int i=1 ; i<= list_get_length(player->list_bomb) ; i++)
		{
			bomb = list_get_value(player->list_bomb,i);
			if (bomb_get_display(bomb) == 0)
			{
				bomb_set_display(bomb,TRUE);
				bomb_set_position(bomb,player_get_x(player),player_get_y(player),map_nb);
				bomb_set_timer(bomb,SDL_GetTicks());
				player->nb_bomb--;
				return;
			}
		}
	}
}

struct list* player_get_list_bomb(struct player* player)
{
	assert(player);
	return player->list_bomb;
}

struct bomb* player_get_bomb(struct player* player, int bomb_number)
{
	assert(player);
	return list_get_value(player->list_bomb,bomb_number);
}

void player_set_bomb_range(struct player* player,int range)
{
	assert(player);
	if (range < BOMB_RANGE_MIN)
		range = BOMB_RANGE_MIN;
	if (range > BOMB_RANGE_MAX)
		range = BOMB_RANGE_MAX;
	player->bomb_range=range;
}

void player_inc_bomb_range(struct player* player)
{
	assert(player);
	int	range=player_get_bomb_range(player);
	range++;
	player_set_bomb_range(player,range);
}

void player_dec_bomb_range(struct player* player)
{
	assert(player);
	int	range=player_get_bomb_range(player);
	range--;
	player_set_bomb_range(player,range);
}

int  player_get_bomb_range(struct player* player)
{
	assert(player);
	return player->bomb_range;
}

void player_set_variables(struct player* player, int nb_bomb,int bomb_range,int nb_key,int nb_life,int map_progression, int level)
{
	assert(player);
	player->nb_life=nb_life;
	player->nb_key=nb_key;
	player->nb_bomb=nb_bomb;
	player->bomb_range=bomb_range;
	player->map_progression=map_progression;
	player->level=level;
}
