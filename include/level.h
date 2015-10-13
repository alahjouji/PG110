#ifndef LEVEL_H_
#define LEVEL_H_

// Library
#include <stdio.h>
#include <stdlib.h>
#include "SDL/SDL_image.h"
#include "../include/constant.h"
#include "../include/map.h"




// return level number num
struct level*  level_get_level(int num);

// return the up of the map
struct level* level_up_map(struct level* level);

// return current map of the level
struct map* level_get_curr_map(struct level* level);
extern void level_set_curr_map(struct level* level,int map_number);
extern int level_get_current_map_nb(struct level* level);
extern int level_get_nb_map(struct level* level);

// return map number num of the level
struct map*  level_get_map(struct level* level, int num);

// free the level
void  level_free(struct level* level);


// display the level
void  level_display(struct level* level);

// free level struct
void   level_free(struct level* level);
extern void level_save(struct level* level,int level_nb);
extern struct level* level_load(int num_level,int current_map);


#endif /* LEVEL_H_ */
