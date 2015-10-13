#ifndef BOMB_H_
#define BOMB_H_

// Library
#include <stdio.h>
#include <stdlib.h>
#include "SDL/SDL_image.h"
#include "../include/constant.h"
#include "../include/map.h"




// Set up the bomb
extern struct bomb* 	bomb_init();
extern void     bomb_free(struct bomb* bomb);
extern void 	bomb_erase(struct bomb* bomb);

// Displaying the bomb
extern void 	bomb_display(struct bomb* bomb, struct map* map,int range);
extern void 	bomb_set_display(struct bomb* bomb, int display);
extern int	 	bomb_get_display(struct bomb* bomb);

// bomb's position
extern void 	bomb_set_position(struct bomb* bomb, int x, int y,int map_nb);
extern int		bomb_get_position(struct bomb* bomb, int x, int y);
extern void		bomb_set_x(struct bomb* bomb, int x);
extern void		bomb_set_y(struct bomb* bomb, int y);
extern int		bomb_get_x(struct bomb* bomb);
extern int		bomb_get_y(struct bomb* bomb);
extern void		bomb_inc_y(struct bomb* bomb, struct map* map);
extern void		bomb_inc_x(struct bomb* bomb, struct map* map);
extern void		bomb_dec_y(struct bomb* bomb, struct map* map);
extern void		bomb_dec_x(struct bomb* bomb, struct map* map);

// bomb's state
extern void 	bomb_set_state(struct bomb* bomb, enum e_state state);
extern int 		bomb_get_state(struct bomb* bomb);
extern void 	bomb_reset_state(struct bomb* bomb);
extern void	 	bomb_inc_state(struct bomb* bomb);
extern void 	bomb_update(struct bomb* bomb);

// bomb's timer
extern int		bomb_get_timer(struct bomb* bomb);
extern void 	bomb_set_timer( struct bomb* bomb , int timer );

// bomb's explosion
extern int 		bomb_case_explode(struct bomb* bomb, struct map* map, int x, int y);

//
extern int bomb_move_aux(struct bomb* bomb, struct map* map, int x, int y);

extern int 		bomb_on_map(struct bomb* bomb,int map);
extern int		bomb_get_map(struct bomb* bomb);

//extern void		bomb_move_up(struct bomb* bomb);
//extern int		bomb_case_explode(struct bomb* bomb, struct map* map, int x, int y);

#endif /* BOMB */
