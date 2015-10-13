#include <SDL/SDL_image.h> // IMG_Load
#include <assert.h>

#include "../include/misc.h"
#include "../include/constant.h"

SDL_Surface* window;

SDL_Surface* window_create(int width, int height) {
	assert(width > 0 && height > 0);

	if (SDL_Init(SDL_INIT_EVERYTHING) == -1) {
		error("Can't init SDL:  %s\n", SDL_GetError());
		exit(EXIT_FAILURE);
	}

	SDL_WM_SetCaption(WINDOW_NAME, NULL );
	window = SDL_SetVideoMode(width, height, 0, // If bits-per-pixel is 0, it is treated as the current display bits per pixel.
			SDL_HWSURFACE);

	if (window == NULL ) {
		error("Can't set video mode: %s\n", SDL_GetError());
		exit(EXIT_FAILURE);
	}
	return window;
}

void window_free() {
	assert(window);
	SDL_FreeSurface(window);
}

void window_display_image(SDL_Surface* sprite, int x, int y) {
	assert(window);
	assert(sprite);

	SDL_Rect place;
	place.x = x;
	place.y = y;

	SDL_BlitSurface(sprite, NULL, window, &place);
}

void window_clear() {
	assert(window);
	SDL_FillRect(window, NULL, SDL_MapRGB(window->format, 255, 255, 255));
}

void window_clear_1() {
	assert(window);
	SDL_FillRect(window, NULL, SDL_MapRGB(window->format, 48, 224, 0));
}

void window_refresh() {
	assert(window);
	SDL_Flip(window);
}
