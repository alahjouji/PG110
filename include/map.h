#ifndef MAP_H_
#define MAP_H_

// Libraries
#include <stdlib.h>
#include <stdio.h>
#include <SDL/SDL_image.h>
#include "../include/bomb.h"

typedef char cell_type_t_;

enum e_state {
	FIRST = 0, SECOND, THIRD, FOURTH, FIRE, DESTROY, ERROR = -1
}bomb_type_t;

typedef enum cell_type{
	CELL_EMPTY=0,
	CELL_GOAL,  	// 1
	CELL_SCENERY, 	// 2
	CELL_PLAYER, 	// 3
	CELL_CASE,   	// 4
	CELL_BONUS, 	// 5
	CELL_MONSTER, 	// 6
	CELL_BOMB, 		// 7
	CELL_KEY,   	// 8
	CELL_DOOR, 		// 9
	CELL_CLOSED_DOOR,//10
	CELL_FIRE, 		// 11
	CELL_beast 	   // 12

} cell_type_t;

typedef enum bonus_type {
	BONUS_BOMB_RANGE_INC=1,
	BONUS_BOMB_RANGE_DEC,
	BONUS_BOMB_NB_INC,
	BONUS_BOMB_NB_DEC,
	BONUS_LIFE,
	BONUS_MONSTER
} bonus_type_t;

typedef enum scenery_type{
	SCENERY_STONE=1, SCENERY_TREE, SCENERY_WATER
} scenery_type_t;

typedef enum compose_type{
	CELL_STONE = CELL_SCENERY | (SCENERY_STONE << 4),
	CELL_TREE = CELL_SCENERY | (SCENERY_TREE << 4),
	CELL_CASE_BOMBINC = CELL_CASE | (BONUS_BOMB_NB_INC << 4),
    CELL_CASE_BOMBDEC = CELL_CASE | (BONUS_BOMB_NB_DEC << 4),
    CELL_CASE_RANGEINC = CELL_CASE | (BONUS_BOMB_RANGE_INC << 4),
    CELL_CASE_RANGEDEC = CELL_CASE | (BONUS_BOMB_RANGE_DEC << 4),
    CELL_CASE_LIFE = CELL_CASE | (BONUS_LIFE << 4),
    CELL_CASE_MONSTER = CELL_CASE | (BONUS_MONSTER << 4),
    CELL_BOMB_TTL4 = CELL_BOMB | (FIRST << 4),
    CELL_BOMB_TTL3 = CELL_BOMB | (SECOND << 4),
    CELL_BOMB_TTL2 = CELL_BOMB | (THIRD << 4),
    CELL_BOMB_TTL1 = CELL_BOMB | (FOURTH << 4),
    CELL_BOMB_FIRE = CELL_BOMB | (FIRE << 4)
} compose_type_t;



// Create a new empty map
extern struct map* 	map_new(int width, int height);
extern void 	map_free(struct map* map);

// Return the height and width of a map
extern int 		map_get_width(struct map* map);
extern int 		map_get_height(struct map* map);

// Return the type of a cell
extern cell_type_t map_get_cell_type(struct map* map, int x, int y);

// Return the type of a bonus
extern bonus_type_t map_gebonus_type_t(struct map* map, int x, int y);

// Set the type of a cell
extern void  	map_set_cell_type(struct map* map, int x, int y, cell_type_t type);

// Test if (x,y) is within the map
extern int 		map_is_inside(struct map* map, int x, int y);

// Load the different map
extern struct map*	map_get_map(int level_id , int map_id);
extern struct map*	map_load(char * map_name );

// Display the map on the window
extern void 	map_display(struct map* map);
extern void 	map_display_random(struct map* map,int x,int y);
extern int 		map_get_nb_particular_cell ( struct map* map , cell_type_t type );

// Free of SDL
extern int 		map_sdl_is_free(struct map* map);
extern void 	map_sdl_free(struct map* map);

// Manage the door in order to make the player move between the different map and level
extern char		map_get_door_type(struct map* map,int x, int y);

// Manage monster pop in the game
extern void 	map_set_cell_type_bonus_monster(struct map* map,int x,int y);
extern int 		map_monster_pop(struct map* map);
extern void 	map_monster_pop_untrig(struct map* map);
extern int 		map_get_monster_coor(struct map* map, int coor);

extern void 	map_save(struct map* map, int map_nb);

#endif /* MAP_H_ */
