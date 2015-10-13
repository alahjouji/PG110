#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <SDL/SDL_image.h>

#include <time.h>
#include "../include/constant.h"
#include "../include/map.h"
#include "../include/misc.h"

// Structure's definition
struct map {
	int width;
	int height;
	int is_free;

	int monster_pop;
	int monster_x;
	int monster_y;

	cell_type_t_* grid;

};

#define CELL(i,j) (i +  map->width * j)

struct map* map_new(int width, int height) {
	assert(width > 0 && height > 0);

	struct map* map = malloc(sizeof (struct map));
	if (map == NULL )
	{
		printf("Cannot malloc map");
		exit(EXIT_FAILURE);
	}
	map->width = width;
	map->height = height;
	map->monster_pop=0;

	map->grid = (cell_type_t_ *) malloc(height * width * sizeof(cell_type_t_));
	if (map->grid == NULL )
	{
		printf("Cannot malloc map->grid");
		exit(EXIT_FAILURE);
	}
	// Grid cleaning
	int i, j;
	for (i = 0; i < width; i++) {
		for (j = 0; j < height; j++) {
			map->grid[CELL(i,j)] = CELL_EMPTY;
		}
	}


	return map;
}

int map_is_inside(struct map* map, int x, int y) {

	assert(map);
	if(x>=map_get_width(map) || x<0 || y>=map_get_height(map) || y<0)
		return 0;
	return 1;
}

void map_free(struct map* map) {

	if (map == NULL )
		return;

	free(map->grid);


	free(map);

}

int map_sdl_is_free(struct map* map)
{
	return map->is_free;
}

void map_sdl_free(struct map* map)
{
	map->is_free=1;
}

int map_get_width(struct map* map) {
	assert(map != NULL);
	return map->width;
}

int map_get_height(struct map* map) {
	assert(map != NULL);
	return map->height;
}

cell_type_t map_get_cell_type(struct map* map, int x, int y) {
	assert(map && map_is_inside(map, x, y));
	return map->grid[CELL(x,y)] & 15;
}

cell_type_t map_get_entire_cell_type(struct map* map, int x, int y) {
	assert(map && map_is_inside(map, x, y));
	return map->grid[CELL(x,y)] ;
}

void map_set_cell_type(struct map* map, int x, int y, cell_type_t type) {
	assert(map && map_is_inside(map, x, y));
	map->grid[CELL(x,y)] = type;
}

bonus_type_t map_gebonus_type_t(struct map* map, int x, int y){
	assert(map && map_is_inside(map, x, y));
	return map->grid[CELL(x,y)] >> 4;
}

void display_bonus(struct map* map, int x, int y, cell_type_t_ type)
{
	// bonus is encoded with the 4 most significant bits
	switch (type >> 4) {
	case BONUS_BOMB_RANGE_INC:
		window_display_image(sprite_get_bonus(BONUS_BOMB_RANGE_INC), x, y);
		break;

	case BONUS_BOMB_RANGE_DEC:
		window_display_image(sprite_get_bonus(BONUS_BOMB_RANGE_DEC), x, y);
		break;

	case BONUS_BOMB_NB_INC:
		window_display_image(sprite_get_bonus(BONUS_BOMB_NB_INC), x, y);
		break;

	case BONUS_BOMB_NB_DEC:
		window_display_image(sprite_get_bonus(BONUS_BOMB_RANGE_DEC), x, y);
		break;
	case BONUS_LIFE:
		window_display_image(sprite_get_bonus(BONUS_LIFE), x, y);
		break;
	}
}

void display_scenery(struct map* map, int x, int  y, cell_type_t_ type)
{
	switch (type >> 4) {
	case SCENERY_STONE:
		window_display_image(sprite_get_stone(), x, y);
		break;

	case SCENERY_TREE:
		window_display_image(sprite_get_tree(), x, y);
		break;
	}
}

void map_display(struct map* map)
{
	assert(map != NULL);
	assert(map->height > 0 && map->width > 0);

	int x, y;
	for (int i = 0; i < map->width; i++) {
		for (int j = 0; j < map->height; j++) {
			x = i * SIZE_BLOC;
			y = j * SIZE_BLOC;

			char type = map->grid[CELL(i,j)];

			switch (type & 15) { // type is encoded with 4 bits, 15 (1111) is a mask to keep the four less significant bits
			case CELL_GOAL:
				if(type==1)
					window_display_image(sprite_get_flag(), x, y);
				if(type==17)
					window_display_image(sprite_get_bomberwoman(), x, y);
				break;
			case CELL_SCENERY:
				display_scenery(map, x, y, type);
				break;
			case CELL_CASE:
				window_display_image(sprite_get_box(), x, y);
				break;
			case CELL_BONUS:
				display_bonus(map, x, y, type);
				break;
			case CELL_KEY:
				window_display_image(sprite_get_key(), x, y);
				break;
			case CELL_DOOR:
				window_display_image(sprite_get_door(), x, y);
				break;
			case CELL_CLOSED_DOOR:
				window_display_image(sprite_get_closed_door(), x, y);
				break;
			}
		}

	}
}

int map_get_nb_particular_cell ( struct map* map , cell_type_t type )
{
	int i, j,nb=0;
	for (i = 0; i < map_get_width(map); i++)
		for (j = 0; j < map_get_height(map); j++)
			if (map_get_cell_type(map, i, j) == type)
				nb++;

	return nb;
}

void map_display_random(struct map* map,int x,int y)
{
	assert(map);


	unsigned char choice = rand() % 7 ; //random number;
	if (choice == 0 ) // Case CELL_EMPTY
	{
		map_set_cell_type(map,x,y,CELL_EMPTY);
	}
	else
	{
		choice = choice << 4 ;
		choice += 5 ;
		if (choice!= 101)
			map_set_cell_type(map,x,y,choice);
		else
			map_set_cell_type_bonus_monster(map,x,y);
	}
}

void map_set_cell_type_bonus_monster(struct map* map,int x,int y)
{
	assert(map);
	map->grid[CELL(x,y)] = CELL_EMPTY;
	map->monster_pop=1;
	map->monster_x=x;
	map->monster_y=y;
}

int map_monster_pop(struct map* map)
{
	assert(map);
	return map->monster_pop;
}

void map_monster_pop_untrig(struct map* map)
{
	assert(map);
	map->monster_pop=0;
}

int map_get_monster_coor(struct map* map, int coor)
{
	assert(map);
	if (coor==0)
		return map->monster_x;
	else
		return map->monster_y;
}
struct map* map_get_map (int level_id , int map_id)
{
	switch(level_id)
	{
	case 0:
		switch(map_id)
		{
		case 0:
			return map_load("map_0_0");
			break;
		case 1:
			return map_load("map_1_0");
			break;
		case 2:
			return map_load("map_2_0");
			break;
		case 3:
			return map_load("map_3_0");
			break;
		case 4:
			return map_load("map_4_0");
			break;
		case 5:
			return map_load("map_5_0");
			break;
		default:
			printf("[ERROR] map_get_map : MAP N#\n");
			break;
		}
		break;
	case 1:
			switch(map_id)
			{
			case 0:
				return map_load("map_0_1");
				break;
			case 1:
				return map_load("map_1_1");
				break;
			case 2:
				return map_load("map_2_1");
				break;
			case 3:
				return map_load("map_3_1");
				break;
			case 4:
				return map_load("map_4_1");
				break;
			case 5:
				return map_load("map_5_1");
				break;
			default:
				printf("[ERROR] map_get_map : MAP N#\n");
				break;
			}
			break;
	case 2:
			switch(map_id)
			{
			case 0:
				return map_load("map_0_2");
				break;
			case 1:
				return map_load("map_1_2");
				break;
			case 2:
				return map_load("map_2_2");
				break;
			case 3:
				return map_load("map_3_2");
				break;
			case 4:
				return map_load("map_4_2");
				break;
			case 5:
				return map_load("map_5_2");
				break;
			default:
				printf("[ERROR] map_get_map : MAP N#\n");
				break;
			}
		break;
		case 3:
			switch(map_id)
			{
			case 0:
				return map_load("map_0");
				break;
			case 1:
				return map_load("map_1");
				break;
			case 2:
				return map_load("map_2");
				break;
			case 3:
				return map_load("map_3");
				break;
			case 4:
				return map_load("map_4");
				break;
			case 5:
				return map_load("map_5");
				break;
			default:
				printf("[ERROR] map_get_map : MAP N#\n");
				break;
			}
			break;
	default:
		printf("[ERROR] map_get_map : LEVEL N#\n");
		break;

	}
	return 0;
}

struct map* map_load(char * map_name )
{
	int x, y, read;
	char file_name[20];
	file_name[0]='\0';

	strcat(file_name,"map/");
	strcat(file_name,map_name);
	strcat(file_name,".txt");

	FILE* file = fopen(file_name, "r");
	if (!file)
		printf("[ERROR] map_load : map name error\n");
	assert(file);


	fscanf(file, "%d,%d\n", &x, &y);


	struct map* map = map_new(x, y);
	cell_type_t_ themap[x*y];

	for(int i = 0; i < x; i++) {
	for(int j = 0; j < y; j++) {
	  fscanf(file, "%d ", &read);
	  themap[i*x + j] =  read;
	}

	fscanf(file, "\n");
	}

	for (int i = 0; i < 144; i++) {
		map->grid[i] = themap[i];
	}

	return map;
}

char map_get_door_type(struct map* map,int x, int y)
{
	assert(map && map_is_inside(map, x, y));
	return map->grid[CELL(x,y)];
}

void map_save(struct map* map, int map_nb)
{
	assert(map);
	FILE* f = NULL;

	switch(map_nb)
	{
	case 0:
		f=fopen("map/map_0.txt", "w");
		break;
	case 1:
		f=fopen("map/map_1.txt", "w");
		break;
	case 2:
		f=fopen("map/map_2.txt", "w");
		break;
	case 3:
		f=fopen("map/map_3.txt", "w");
		break;
	case 4:
		f=fopen("map/map_4.txt", "w");
		break;
	default:
		printf("[ERROR] map_save : map_nb\n");
		break;
	}
	assert(f);

	int x,y;
	x=map_get_width(map);
	y=map_get_height(map);
	fprintf(f, "%d,%d\n", x,y);

	for(int i = 0; i < x; i++)
	{
		for(int j = 0; j < y; j++)
			fprintf(f, "%d ", map->grid[i*x + j]);
		fprintf(f, "\n");
	}
	fprintf(f, "\n");
	fclose(f);
}

