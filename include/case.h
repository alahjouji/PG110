#ifndef CASE_H_
#define CASE_H_

// Libraries
#include <stdio.h>
#include <stdlib.h>
#include "SDL/SDL_image.h"
#include "../include/constant.h"
#include "../include/map.h"


void case_inc_y(struct player* player, struct map* map);
void case_dec_y(struct player* player, struct map* map);
void case_inc_x(struct player* player, struct map* map);
void case_dec_x(struct player* player, struct map* map);

int case_move_aux(struct map* map, int x, int y);

#endif /* CASE_H_ */
