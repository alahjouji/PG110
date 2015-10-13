#ifndef GAME_H_
#define GAME_H_

// libraries
#include "../include/constant.h"
#include "../include/misc.h"
#include "../include/map.h"
#include "../include/player.h"
#include "../include/level.h"
#include "../include/menu.h"
#include "../include/timer.h"

enum {
	PAUSE_INACTIVE,
	PAUSE_PLAYER,
	PAUSE_TEXT_1,
	PAUSE_TEXT_2,
	PAUSE_WIN,
	PAUSE_SAVED,
	PAUSE_LOOSE
};

// Structure's definition
typedef struct s_game * t_game;

// Game Creation and destruction
extern struct game* 	game_new();
extern void 	game_free(struct game* game);

// Display the game on the window
extern void 	game_display(struct game* game);
extern void 	game_display_pause(struct game* game, SDL_Surface* window);
extern void 	game_display_text(struct game* gamen,int num);

// Game's management
extern void		game_move_object(struct game* game, int direction);
extern int 		game_end(struct game* game);
extern int 		game_get_death(struct game* game);
extern void 	game_set_death(struct game* game,int death);
extern int 		game_update(struct game* game);
extern void 	game_up_level(struct game* game);						// update the level of the player
extern void 	game_up_map(struct game* game,struct map* map);				// update the map of the game
extern void 	game_monster_move(struct game* game);

// Return the element of the current game
extern struct player* game_get_player(struct game* game);
extern struct bomb* 	game_get_bomb(struct game* game);
extern struct menu* 	game_get_menu(struct game* game);
extern struct level* 	game_get_curr_level(struct game* game);

// Manage the game's timer
extern struct timer* 	game_get_timer(struct game* game);
extern void		game_update_timer(struct game* game);

// Monster's management in the game
extern int 		monster_close_to_player(struct game* game);
extern void 	monster_add(struct game* game);

// Bomb's management in the game
extern int 		bomb_close_to_player(struct game* game,struct bomb* bomb);

// Pause
extern void 	game_pause(struct game* game);
extern void 	game_unpause(struct game* game);
extern int 		game_get_pause(struct game* game);

// Manage keyboard
extern int 		game_key_pressed();

// Save of the game for the player
extern void 	game_load(struct game* game);
extern void 	game_save(struct game* game);

#endif /* GAME_H_ */
