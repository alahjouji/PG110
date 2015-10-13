#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <time.h>
#include <SDL/SDL_image.h>
#include "../include/window.h"
#include "../include/constant.h"
#include "../include/bomb.h"
#include "../include/game.h"

int main(int argc, char *argv[]) {
	SDL_Surface *window;

	struct game* game = game_new();
	struct map* map = level_get_curr_map(game_get_curr_level(game));

	window=window_create(SIZE_BLOC * 12,
				SIZE_BLOC * 12 + BANNER_HEIGHT + LINE_HEIGHT);

	SDL_EnableKeyRepeat(SDL_DEFAULT_REPEAT_DELAY, SDL_DEFAULT_REPEAT_INTERVAL);
	game_pause(game);




	// to obtain the DEFAULT_GAME_FPS, we have to reach a loop duration of (1000 / DEFAULT_GAME_FPS) ms
	uint ideal_speed = 1000 / DEFAULT_GAME_FPS;
	Uint32 timer, execution_speed;

	SDL_Event event;

	// game loop with fixed time rate implementation
	int done = 0;
	while (! done)
	{
		timer = SDL_GetTicks();
			if (game_get_death(game) != 1)
			{
				if (!game_get_pause(game))
				{
					done = game_update(game);
					game_display(game);
					if (player_map_is_updated(game_get_player(game)) == 1)
						game_up_map(game,map);
				}
				else
				{
					if (game_get_pause(game) == PAUSE_PLAYER)
						game_display_pause(game,window);
					else
					{
						if (game_get_pause(game) == PAUSE_TEXT_1)
							game_display_text(game,PAUSE_TEXT_1);
						if (game_get_pause(game) == PAUSE_TEXT_2)
							game_display_text(game,PAUSE_TEXT_2);
						if (game_get_pause(game) == PAUSE_WIN)
							game_display_text(game,PAUSE_WIN);
						SDL_Delay(2*ONE_SECOND);
					}
					game_update_timer(game);
					while (SDL_PollEvent(&event))
					{
						switch (event.type)
						{
						case SDL_QUIT:
							return 1;
						case SDL_KEYDOWN:
							switch (event.key.keysym.sym)
							{
							case SDLK_ESCAPE:
								return 1;
							case SDLK_c:  // "c" to continue
							case SDLK_p : // "p" to get off the pause
							case SDLK_b : // "u" for unpause
								game_unpause(game);
								break;
							case SDLK_u :
								game_load(game);
								game_unpause(game);
								break;
							default:
								break;
							}
							break;
						}
					}
				}
			}
			else
			{
				// Game over
				game_display_text(game,PAUSE_LOOSE);
				SDL_Delay(2*ONE_SECOND);
				game_free(game);
				game_new();
				map = level_get_curr_map(game_get_curr_level(game));
				game_display(game);
				game_pause(game);
			}

		execution_speed = SDL_GetTicks() - timer;
		if (execution_speed < ideal_speed)
			SDL_Delay(ideal_speed - execution_speed); // we are ahead of ideal time. let's wait.
	}

	window_free();
	game_free(game);
	SDL_FreeSurface(window);
	SDL_Quit();

	return EXIT_SUCCESS;
}
