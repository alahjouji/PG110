#ifndef SPRITE_H_
#define SPRITE_H_

#include <SDL/SDL.h>
#include "../include/map.h"
#include "../include/constant.h"

// load game's sprites
void		 sprite_load();

SDL_Surface* sprite_get_player(enum way direction);
SDL_Surface* sprite_get_bonus(bonus_type_t bonus_type);
SDL_Surface* sprite_get_flag();
SDL_Surface* sprite_get_bomberwoman();
SDL_Surface* sprite_get_tree();
SDL_Surface* sprite_get_box();
SDL_Surface* sprite_get_key();
SDL_Surface* sprite_get_stone();
SDL_Surface* sprite_get_door();
SDL_Surface* sprite_get_closed_door();
SDL_Surface* sprite_get_pause();
SDL_Surface* sprite_get_monster(enum way direction);
SDL_Surface* sprite_get_beast(enum way direction);
SDL_Surface* sprite_get_text(int num);

SDL_Surface* sprite_get_number(short number);
SDL_Surface* sprite_get_banner_life();
SDL_Surface* sprite_get_banner_bomb();
SDL_Surface* sprite_get_banner_line();
SDL_Surface* sprite_get_banner_range();

#endif /* SPRITE_H_ */
