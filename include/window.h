#ifndef WINDOW_H_
#define WINDOW_H_

#include <SDL/SDL.h>

SDL_Surface* window_create(int width, int height);
void window_free();

void window_refresh();

// display a SDL_Surface at location (x,y)
void window_display_image(SDL_Surface* surface, int x, int y);

// set every pixel of the window to white
void window_clear();
void window_clear_1();
#endif /* WINDOW_H_ */
