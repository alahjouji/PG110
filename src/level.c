
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <SDL/SDL_image.h>


#include "../include/game.h"
#include "../include/level.h"
#include "../include/map.h"

struct level{
	struct map** maps; // array of the level's maps
	int nb_maps; // nb maps of the level
	int cur_map; // the current map
};

int level_get_current_map_nb(struct level* level)
{
	assert(level);
	return level->cur_map;
}

int level_get_nb_map(struct level* level)
{
	assert(level);
	return level->nb_maps;
}

struct level* level_get_level(int num)
{
	struct level* level = malloc(sizeof(struct level));
	switch(num)
	{
		case 0 :

			level->nb_maps = 4;
			level->cur_map = 0;
			level->maps = malloc(level->nb_maps * sizeof (*level->maps));
			for (int i=0; i< level->nb_maps ; i++)
				level->maps[i] = map_get_map(0,i);
			return level;
			break;
		case 1 :

			level->nb_maps = 5;
			level->cur_map = 0;
			level->maps = malloc(level->nb_maps *sizeof (*level->maps));
			for (int i=0; i< level->nb_maps ; i++)
				level->maps[i] = map_get_map(1,i);
			return level;
			break;
		case 2 :

			level->nb_maps = 4;
			level->cur_map = 0;
			level->maps = malloc(level->nb_maps *sizeof (*level->maps));
			for (int i=0; i< level->nb_maps ; i++)
				level->maps[i] = map_get_map(2,i);
			return level;
			break;
	}
	return NULL;
}

struct level* level_load(int num_level,int current_map)
{
	struct level* level = malloc(sizeof(struct level));
	switch(num_level)
	{
		case 0 :

			level->nb_maps = 4;
			level->cur_map = current_map;
			level->maps = malloc(level->nb_maps * sizeof (*level->maps));
			for (int i=0; i< level->nb_maps ; i++)
				level->maps[i] = map_get_map(3,i);
			return level;
			break;
		case 1 :

			level->nb_maps = 5;
			level->cur_map = current_map;
			level->maps = malloc(level->nb_maps *sizeof (*level->maps));
			for (int i=0; i< level->nb_maps ; i++)
				level->maps[i] = map_get_map(3,i);
			return level;
			break;
		case 2 :

			level->nb_maps = 4;
			level->cur_map = current_map;
			level->maps = malloc(level->nb_maps *sizeof (*level->maps));
			for (int i=0; i< level->nb_maps ; i++)
				level->maps[i] = map_get_map(3,i);
			return level;
			break;
	}
	return NULL;
}

struct map* level_get_curr_map(struct level* level) {
	return level->maps[level->cur_map];
}

void level_set_curr_map(struct level* level,int map_number)
{
	assert(level);
	level->cur_map=map_number;
}

struct map* level_get_map(struct level* level, int num) {
	assert(num <= level->nb_maps);
	return level->maps[num];
}

void level_free(struct level* level)
{
	for(int i = 0; i < level->nb_maps; i++)
		map_free(level->maps[i]);

	free(level->maps);
}

void level_display(struct level* level) {
	map_display(level->maps[level->cur_map]);
}

void level_save(struct level* level,int level_nb)
{
	assert(level);

	for (int i=0;i<level->nb_maps;i++)
	{
		map_save(level->maps[i],i);
	}
}
