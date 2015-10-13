#include <assert.h>
#include <time.h>
#include "../include/game.h"
#include "../include/map.h"
#include "../include/player.h"
#include "../include/misc.h"
#include "../include/level.h"
#include "../include/monster.h"
#include "../include/bomb.h"
#include "../include/list.h"
#include "../include/timer.h"
#include "../include/case.h"
#include "../include/sprite.h"
#include "../include/window.h"


struct game {
	struct level* 	curr_level;
	struct player* 	player;
	struct list* 		list_monster;
	struct timer*		timer_game_update;

	int player_death;
	int pause;
	int pause_delay;
	int level;

	SDL_Surface * window_text;
};

static struct game the_game;

struct game* game_new(void) {
	sprite_load();
	srand(time(NULL ));

	the_game.player_death=0;
	the_game.pause=0;
	the_game.pause_delay=0;
	the_game.level=0;

	the_game.curr_level = level_get_level(0); // get maps of the first level
	the_game.player_death = 0;
	the_game.player = player_init(3,5); // 3 bomb, 5 life
	player_from_map(the_game.player, level_get_map(the_game.curr_level, 0)); // get x,y of the player on the first map
	the_game.timer_game_update = timer_new();

	monster_add(&the_game);

	the_game.window_text 	= SDL_LoadBMP("sprite/pause1.bmp");

	return &the_game;
}

struct list* game_get_list_monster(struct game* game) {
	assert(game);
	return game->list_monster;
}

void game_free(struct game* game) {
	assert(game);

	player_free(game->player);
	list_free(game_get_list_monster(game));
	timer_free(game->timer_game_update);
	level_free(game->curr_level);

}

struct player* game_get_player(struct game* game) {
	assert(game);
	return game->player;
}

int game_get_death(struct game* game){
	assert(game);
	return game->player_death;
}

void game_set_death(struct game* game,int death)
{
	assert(game);
	game->player_death = death;
}

int monster_close_to_player(struct game* game)
{
	struct player* player = game_get_player(game);
	struct list* list_monster = game_get_list_monster(game);

	for (int i=1 ; i<=list_get_length(game_get_list_monster(game)) ; i++)
	{
		if(monster_on_map(list_get_value(list_monster,i) , level_get_current_map_nb(game_get_curr_level(game))))
		{
			if(monster_get_x((struct monster*)list_get_value(list_monster,i)) == player_get_x(player)+1 && monster_get_y((struct monster*)list_get_value(list_monster,i)) == player_get_y(player) )
				return 1;
			if(monster_get_x((struct monster*)list_get_value(list_monster,i)) == player_get_x(player)-1 && monster_get_y((struct monster*)list_get_value(list_monster,i)) == player_get_y(player) )
				return 1;
			if(monster_get_x((struct monster*)list_get_value(list_monster,i)) == player_get_x(player) && monster_get_y((struct monster*)list_get_value(list_monster,i)) == player_get_y(player)+1 )
				return 1;
			if(monster_get_x((struct monster*)list_get_value(list_monster,i)) == player_get_x(player) && monster_get_y((struct monster*)list_get_value(list_monster,i)) == player_get_y(player)-1 )
				return 1;
		}
	}
	return 0;
}

int bomb_close_to_player(struct game* game,struct bomb* bomb)
{
	struct player* player = game_get_player(game);
	struct map* map = level_get_curr_map(game_get_curr_level(game));

	if(bomb_get_x(bomb) == player_get_x(player) && bomb_get_y(bomb) == player_get_y(player) )
		return 1;	// On the bomb

	for (int j=1 ; j<=player_get_bomb_range(player) ; j++)
	{
		if( map_is_inside(map,bomb_get_x(bomb),bomb_get_y(bomb)-j) && map_get_cell_type(map,bomb_get_x(bomb),bomb_get_y(bomb)-j) == CELL_SCENERY)
			break;
		if(bomb_get_x(bomb) == player_get_x(player) && bomb_get_y(bomb)-j == player_get_y(player) )
			return 1;	// Above the bomb
	}
	for (int j=1 ; j<=player_get_bomb_range(player) ; j++)
	{
		if( map_is_inside(map,bomb_get_x(bomb)+j,bomb_get_y(bomb)) &&  map_get_cell_type(map,bomb_get_x(bomb)+j,bomb_get_y(bomb)) == CELL_SCENERY)
			break;
		if(bomb_get_x(bomb)+j == player_get_x(player) && bomb_get_y(bomb) == player_get_y(player) )
			return 1;	// Right Side of the bomb
	}
	for (int j=1 ; j<=player_get_bomb_range(player) ; j++)
	{
		if(  map_is_inside(map,bomb_get_x(bomb),bomb_get_y(bomb)+j) && map_get_cell_type(map,bomb_get_x(bomb),bomb_get_y(bomb)+j) == CELL_SCENERY)
			break;
		if(bomb_get_x(bomb) == player_get_x(player) && bomb_get_y(bomb)+j == player_get_y(player) )
			return 1;	// Under the bomb
	}
	for (int j=1 ; j<=player_get_bomb_range(player) ; j++)
	{
		if(  map_is_inside(map,bomb_get_x(bomb)-j,bomb_get_y(bomb)) && map_get_cell_type(map,bomb_get_x(bomb)-j,bomb_get_y(bomb)) == CELL_SCENERY)
			break;
		if(bomb_get_x(bomb)-j == player_get_x(player) && bomb_get_y(bomb) == player_get_y(player) )
			return 1;	// Left Side of the bomb
	}

	return 0;
}

int bomb_close_to_monster(struct game* game,struct monster* monster, struct bomb* bomb, int range)
{
	struct map* map = level_get_curr_map(game_get_curr_level(game));

	if (monster_get_map(monster) == bomb_get_map(bomb))
	{
		if(bomb_get_x(bomb) == monster_get_x(monster) && bomb_get_y(bomb) == monster_get_y(monster) )
			return 1;	// On the bomb
		for (int j=1 ; j<=range ; j++)
		{
			if(  map_is_inside(map,bomb_get_x(bomb),bomb_get_y(bomb)-j) && map_get_cell_type(map,bomb_get_x(bomb),bomb_get_y(bomb)-j) == CELL_SCENERY)
				break;
			if(bomb_get_x(bomb) == monster_get_x(monster) && bomb_get_y(bomb)-j == monster_get_y(monster) )
				return 1;	// Above the bomb
		}
		for (int j=1 ; j<=range ; j++)
		{
			if(  map_is_inside(map,bomb_get_x(bomb)+j,bomb_get_y(bomb)) && map_get_cell_type(map,bomb_get_x(bomb)+j,bomb_get_y(bomb)) == CELL_SCENERY)
				break;
			if(bomb_get_x(bomb)+j == monster_get_x(monster) && bomb_get_y(bomb) == monster_get_y(monster) )
				return 1;	// Right Side of the bomb
		}
		for (int j=1 ; j<=range ; j++)
		{
			if(  map_is_inside(map,bomb_get_x(bomb),bomb_get_y(bomb)+j) && map_get_cell_type(map,bomb_get_x(bomb),bomb_get_y(bomb)+j) == CELL_SCENERY)
				break;
			if(bomb_get_x(bomb) == monster_get_x(monster) && bomb_get_y(bomb)+j == monster_get_y(monster) )
				return 1;	// Under the bomb
		}
		for (int j=1 ; j<=range ; j++)
		{
			if(  map_is_inside(map,bomb_get_x(bomb)-j,bomb_get_y(bomb)) && map_get_cell_type(map,bomb_get_x(bomb)-j,bomb_get_y(bomb)) == CELL_SCENERY)
				break;
			if(bomb_get_x(bomb)-j == monster_get_x(monster) && bomb_get_y(bomb) == monster_get_y(monster) )
				return 1;	// Left Side of the bomb
		}
	}
	return 0;
}

int game_end(struct game* game)
{
	if (player_get_nb_life(game_get_player(game)) == 0)
		return 1;
	return 0;
}


void game_banner_display(struct game* game) {
	assert(game);

	struct map* map = level_get_curr_map(game_get_curr_level(game));

	int y = (map_get_height(map)) * SIZE_BLOC;
	for (int i = 0; i < map_get_width(map); i++)
		window_display_image(sprite_get_banner_line(), i * SIZE_BLOC, y);

	int white_bloc = ((map_get_width(map) * SIZE_BLOC) - 8 * SIZE_BLOC) / 4;
	int x = white_bloc;
	y = (map_get_height(map) * SIZE_BLOC) + LINE_HEIGHT;
	window_display_image(sprite_get_banner_life(), x, y);

	x = white_bloc + SIZE_BLOC;
	window_display_image(sprite_get_number(player_get_nb_life(game_get_player(game))), x, y);

	x = 2 * white_bloc + 2 * SIZE_BLOC;
	window_display_image(sprite_get_banner_bomb(), x, y);

	x = 2 * white_bloc + 3 * SIZE_BLOC;
	if ((player_get_nb_bomb(game_get_player(game)) >= 0)
			& (player_get_nb_bomb(game_get_player(game)) < 10))
		window_display_image(
			sprite_get_number(player_get_nb_bomb(game_get_player(game))), x, y);

	x = 3 * white_bloc + 4 * SIZE_BLOC;
	window_display_image(sprite_get_key(), x, y);

	x = 3 * white_bloc + 5 * SIZE_BLOC;
	window_display_image(sprite_get_number(player_get_key(game_get_player(game))), x, y);


	x = 4 * white_bloc + 6 * SIZE_BLOC;
	window_display_image(sprite_get_banner_range(), x, y);

	x = 4 * white_bloc + 7 * SIZE_BLOC;
	window_display_image(sprite_get_number(player_get_bomb_range(game_get_player(game))), x, y);
}

void monster_add(struct game* game)
{
		game->list_monster = list_new(monster_free);

		int nb_monster=1;
		struct map*  map;
		for (int k=0;k<level_get_nb_map(the_game.curr_level);k++)
		{
			map = level_get_map(the_game.curr_level, k);
			for (int i=0; i<map_get_width(map) ; i++)
				for (int j=0; j<map_get_height(map) ; j++)
				{
					if (map_get_cell_type(map,i,j) == CELL_MONSTER)
					{
						the_game.list_monster = list_add_tail(the_game.list_monster,monster_init(1));
						monster_set_type(list_get_tail_value(the_game.list_monster),MONSTER);
						monster_from_map(list_get_value(the_game.list_monster,nb_monster), level_get_map(the_game.curr_level, k),k);
						nb_monster++;
					}
					if (map_get_cell_type(map,i,j) == CELL_beast)
					{
						the_game.list_monster = list_add_tail(the_game.list_monster,monster_init(5));
						monster_set_type(list_get_tail_value(the_game.list_monster),beast);
						monster_from_map(list_get_value(the_game.list_monster,nb_monster), level_get_map(the_game.curr_level, k),k);
						nb_monster++;
					}
				}
		}
}

void game_up_level(struct game* game)
{
	assert(game);
	level_free(game->curr_level);
	game->level++;
	game->curr_level = level_get_level(game->level);
	player_from_map(game->player, level_get_map(game->curr_level, 0)); // get x,y of the player on the first map
	player_set_nb_key(game->player,0);
	list_free(game_get_list_monster(game));
	monster_add(game);

}


void game_display_text(struct game* map,int num)
{
	// bonus is encoded with the 4 most significant bits
	switch (num) {
	case PAUSE_TEXT_1:
		window_display_image(sprite_get_text(PAUSE_TEXT_1), 0,0);
		break;

	case PAUSE_TEXT_2:
		window_display_image(sprite_get_bonus(PAUSE_TEXT_2), 0,0);
		break;

	case PAUSE_WIN:
		window_display_image(sprite_get_bonus(PAUSE_WIN), 0,0);
		break;

	case PAUSE_SAVED:
		window_display_image(sprite_get_bonus(PAUSE_SAVED), 0,0);
		break;

	case PAUSE_LOOSE:
		window_display_image(sprite_get_bonus(PAUSE_LOOSE), 0,0);
		break;
	}
}

int game_key_pressed()
{
	SDL_Event event;
	while (SDL_PollEvent(&event))
	{
		switch (event.type)
		{
		case SDL_QUIT:
			return 0;
		case SDL_KEYDOWN:
			switch (event.key.keysym.sym)
			{
			case SDLK_ESCAPE:
				return 0;
				break;
			case SDLK_c:
				return 1;
				break;
			default :
				return 0;
				break;
			}
			return 0;
			break;
		default :
			return 0;
			break;
		}
	}
	return 0;
}

int game_object_blink(unsigned long int blink)
{
	unsigned long int sdl_ticks = SDL_GetTicks();
	if ( sdl_ticks - blink < 2 * ONE_SECOND && sdl_ticks > 2 * ONE_SECOND)
	{
		if (  	  (	sdl_ticks - blink < 150 )
				||(	sdl_ticks - blink > 300  && sdl_ticks - blink < 450  )
				||(	sdl_ticks - blink > 600  && sdl_ticks - blink < 750  )
				||(	sdl_ticks - blink > 900  && sdl_ticks - blink < 1050 )
				||(	sdl_ticks - blink > 1200 && sdl_ticks - blink < 1350 )
				||(	sdl_ticks - blink > 1500 && sdl_ticks - blink < 1650 )
				||(	sdl_ticks - blink > 1800 && sdl_ticks - blink < 2000 ) )
		{
			return 0;
		}
		else
		{
			return 1;
		}
	}
	else
	{
		return 1;
	}
}

void game_display(struct game* game)
{
	assert(game);
	window_clear();
	if(player_get_win(game_get_player(game)))
	{
		game_pause(game);
		game->pause=PAUSE_WIN;
	}
	else
	{
		game_banner_display(game);
		int player_level = player_get_level(game_get_player(game));
		if(game->level != player_level)
		{
			game_pause(game);
			if ( player_level == 1 )
				game->pause=PAUSE_TEXT_1;
			else
				game->pause=PAUSE_TEXT_2;
			game_up_level(game);
		}

		level_display(game_get_curr_level(game));
		struct player* player = game_get_player(game);

		player_display(player,level_get_curr_map(game_get_curr_level(game)),level_get_current_map_nb(game_get_curr_level(game)));
		if (game_object_blink(player_get_blink(player)))
			window_display_image(sprite_get_player(player_get_current_way(player)),
				player_get_x(player) * SIZE_BLOC, player_get_y(player) * SIZE_BLOC);
		struct list* list_monster = game_get_list_monster(game);
		struct monster* monster;
		if ( list_get_length(game_get_list_monster(game)) > 0 )
			for (int i=1 ; i<=list_get_length(list_monster) ; i++)
			{
				monster = list_get_value(list_monster,i);
				if(monster_on_map(monster , level_get_current_map_nb(game_get_curr_level(game))))
				{
					if (game_object_blink(monster_get_blink(monster)))
						switch (monster_get_type(monster))
						{
						case MONSTER :
							window_display_image(sprite_get_monster(monster_get_current_way(monster)),
							   monster_get_x(monster) * SIZE_BLOC, monster_get_y(monster) * SIZE_BLOC);
							break;
						case beast :
							window_display_image(sprite_get_beast(monster_get_current_way(monster)),
				    			 monster_get_x(monster) * SIZE_BLOC, monster_get_y(monster) * SIZE_BLOC);
							break;
						}
				}
			}
	}

	window_refresh();
}

void game_display_pause(struct game* game, SDL_Surface* window)
{
	SDL_Rect position;
	SDL_FillRect(window, NULL, SDL_MapRGB(window->format, 48, 224, 0));
	position.x = 0;
	position.y = 0;
	SDL_BlitSurface(game->window_text, NULL, window, &position);
	SDL_Flip(window);
}


struct level* game_get_curr_level(struct game* game) {
	return game->curr_level;
}

void game_move_object(struct game* game, int direction){
	assert(game);


	struct player* player = game_get_player(game);
	struct bomb* bomb;
	struct map* map = level_get_curr_map(game_get_curr_level(game));

	switch(direction)
	{
	case NORTH:
		if(map_is_inside(map, player_get_x(player), player_get_y(player)-1)){
			if(map_get_cell_type(map, player_get_x(player), player_get_y(player)-1) == CELL_CASE && case_move_aux(map, player_get_x(player), player_get_y(player)-2)==1)
			{
				case_inc_y(player, map);
				player_inc_y(player, map);
			}
			else
			{
				for (int i=1 ; i<= list_get_length(player_get_list_bomb(player)) ; i++)
				{
					bomb = list_get_value(player_get_list_bomb(player),i);
					if(bomb_get_display(bomb) && bomb_get_state(bomb) != FIRE )
					{
						if(bomb_get_x(bomb) == player_get_x(player) && (bomb_get_y(bomb)+1) == player_get_y(player) && bomb_move_aux(bomb, map, bomb_get_x(bomb), (bomb_get_y(bomb)-1))==1)
						{
							bomb_inc_y(bomb, map);
							player_inc_y(player, map);
						}
					}
				}
			}
		}
		break;
	case SOUTH:
		if(map_is_inside(map, player_get_x(player), player_get_y(player)+1)){
			if(map_get_cell_type(map, player_get_x(player), (player_get_y(player)+1))==CELL_CASE && case_move_aux(map, player_get_x(player), player_get_y(player)+2)==1)
			{
				case_dec_y(player, map);
				player_dec_y(player, map);
			}
			else
			{
				for (int i=1 ; i<= list_get_length(player_get_list_bomb(player)) ; i++)
				{
					bomb = list_get_value(player_get_list_bomb(player),i);
					if(bomb_get_display(bomb) && bomb_get_state(bomb) != FIRE )
					{
						if(bomb_get_x(bomb) == player_get_x(player) && bomb_get_y(bomb)-1 == player_get_y(player) && bomb_move_aux(bomb, map, bomb_get_x(bomb), (bomb_get_y(bomb)+1))==1)
						{
							bomb_dec_y(bomb, map);
							player_dec_y(player, map);
						}
					}
				}
			}
		}
		break;
	case EAST:
		if(map_is_inside(map, player_get_x(player)+1, player_get_y(player))){
			if(map_get_cell_type(map, player_get_x(player)+1, player_get_y(player)) == CELL_CASE && case_move_aux(map, player_get_x(player)+2, player_get_y(player))==1)
			{
				case_dec_x(player, map);
				player_dec_x(player, map);
			}
			else
			{
				for (int i=1 ; i<= list_get_length(player_get_list_bomb(player)) ; i++)
				{
					bomb = list_get_value(player_get_list_bomb(player),i);
					if(bomb_get_display(bomb) && bomb_get_state(bomb) != FIRE )
					{
						if(bomb_get_x(bomb)-1 == player_get_x(player) && bomb_get_y(bomb) == player_get_y(player) && bomb_move_aux(bomb, map, (bomb_get_x(bomb)+1), bomb_get_y(bomb))==1)
						{
							bomb_dec_x(bomb, map);
							player_dec_x(player, map);
						}
					}
				}
			}
		}
		break;
	case WEST:
		if(map_is_inside(map, player_get_x(player)-1, player_get_y(player))){
			if(map_get_cell_type(map, player_get_x(player)-1, player_get_y(player)) == CELL_CASE && case_move_aux(map, player_get_x(player)-2, player_get_y(player))==1)
			{
				case_inc_x(player, map);
				player_inc_x(player, map);
			}
			else
			{
				for (int i=1 ; i<= list_get_length(player_get_list_bomb(player)) ; i++)
				{
					bomb = list_get_value(player_get_list_bomb(player),i);
					if(bomb_get_display(bomb) && bomb_get_state(bomb) != FIRE )
					{
						if(bomb_get_x(bomb)+1 == player_get_x(player) && bomb_get_y(bomb) == player_get_y(player) && bomb_move_aux(bomb, map, (bomb_get_x(bomb)-1), bomb_get_y(bomb))==1)
						{
							bomb_inc_x(bomb, map);
							player_inc_x(player, map);
						}
					}
				}
			}
		}
		break;
	}
}

short input_keyboard(struct game* game) {
	SDL_Event event;
	struct player* player = game_get_player(game);
	struct map* map = level_get_curr_map(game_get_curr_level(game));

	while (SDL_PollEvent(&event)) {
		switch (event.type) {
		case SDL_QUIT:
			return 1;
		case SDL_KEYDOWN:
			switch (event.key.keysym.sym) {
			case SDLK_ESCAPE:
				return 1;
			case SDLK_UP:
				player_set_current_way(player, NORTH);
				game_move_object(game, NORTH);
				player_move(player, map);
				break;
			case SDLK_DOWN:
				player_set_current_way(player, SOUTH);
				game_move_object(game, SOUTH);
				player_move(player, map);
				break;
			case SDLK_RIGHT:
				player_set_current_way(player, EAST);
				game_move_object(game, EAST);
				player_move(player, map);
				break;
			case SDLK_LEFT:
				player_set_current_way(player, WEST);
				game_move_object(game, WEST);
				player_move(player, map);
				break;
			case SDLK_SPACE:
				player_set_bomb(player,map,level_get_current_map_nb(game_get_curr_level(game)));
				break;
			case SDLK_l :
				game->player_death = 1 ;
				game->pause_delay = SDL_GetTicks();
				break;
			case SDLK_p :
				game_pause(game);
				break;
			case SDLK_s :
				game_save(game);
				break;


			default:
				break;
			}

			break;
		}
	}
	return 0;
}

void game_up_map(struct game* game,struct map* map)
{
	if(level_get_current_map_nb(game_get_curr_level(game)) == 2 && game->level == 2)
		player_set_nb_key(game_get_player(game),0);
	level_set_curr_map(game_get_curr_level(game),player_get_map(game_get_player(game)));
	map = level_get_curr_map(game_get_curr_level(game));
	player_from_map(game_get_player(game), level_get_map(game_get_curr_level(game), player_get_map(game_get_player(game))));
	player_map_untrig(game_get_player(game));
}

struct timer* game_get_timer(struct game* game)
{
	assert(game);
	return game->timer_game_update;
}

void game_update_timer(struct game* game)
{
	assert(game);
	int delete_elapsed_time = timer_get_tick(game_get_timer(game),SDL_GetTicks());
}

int game_update(struct game* game)
{
	if (input_keyboard(game) == 1) {
		return 1;
	}

	if( player_get_nb_life(game_get_player(game)) == 0)
	{
		game->player_death = 1 ;
		return 0;
	}


	if(timer_get_tick(game_get_timer(game),SDL_GetTicks()))
	{
		struct player* player = game_get_player(game);
		if (monster_close_to_player(game) == 1 )
		{
			player_dec_nb_life(player);
			player_set_blink(player,SDL_GetTicks());
			if( player_get_nb_life(game_get_player(game)) == 0)
			{
				game->player_death = 1 ;
				return 0;
			}
		}

		struct map* map = level_get_curr_map(game_get_curr_level(game));
		struct list* list_monster = game_get_list_monster(game);

		struct bomb* bomb;
		for (int i=1 ; i<= list_get_length(player_get_list_bomb(player)) ; i++)
		{
			bomb = list_get_value(player_get_list_bomb(player),i);
			if (bomb_get_display(bomb) == 1)
			{
				if(bomb_get_state(bomb) == FIRE )
				{
					if (bomb_close_to_player(game,bomb) == 1)
					{
						player_dec_nb_life(game_get_player(game));
						player_set_blink(player,SDL_GetTicks());
						if( player_get_nb_life(game_get_player(game)) == 0)
						{
							game->player_death = 1 ;
							return 0;
						}
					}
					for (int j=1 ; j<=list_get_length(list_monster) ; j++)
					{
						if ( bomb_close_to_monster(game,list_get_value(list_monster,j),bomb,player_get_bomb_range(player)) == 1 && monster_get_pop(list_get_value(list_monster,j))==0)
						{
							monster_dec_nb_life(list_get_value(list_monster,j));

							if ( monster_get_nb_life(list_get_value(list_monster,j)) == 0 )
							{
								if (monster_get_type(list_get_value(list_monster,j)) == beast)
									map_set_cell_type(level_get_curr_map(game_get_curr_level(game)),10,10,CELL_EMPTY);
								map_set_cell_type(map,monster_get_x(list_get_value(list_monster,j)),monster_get_y(list_get_value(list_monster,j)),CELL_EMPTY);
								list_free_element(list_monster,j);
							}
							else
								monster_set_blink(list_get_value(list_monster,j),SDL_GetTicks());
						}
					}
					bomb_set_display(bomb,FALSE);
					bomb_reset_state(bomb);
					player_inc_nb_bomb(player);
				}
				if(bomb_get_map(bomb) == level_get_current_map_nb(game_get_curr_level(game)))
					if (bomb_get_timer(bomb)-SDL_GetTicks() >= ONE_SECOND )
					{
						bomb_inc_state(bomb);
						bomb_set_timer(bomb,bomb_get_timer(bomb)+ONE_SECOND);
					}
			}
		}
		game_monster_move(game);

		monster_set_pop(list_get_value(game_get_list_monster(game),list_get_length(game_get_list_monster(game))),0);
		if (map_monster_pop(level_get_curr_map(game_get_curr_level(game))))
		{
			list_add_tail(game_get_list_monster(game),monster_init(1));
			monster_set_position(list_get_value(game_get_list_monster(game),list_get_length(game_get_list_monster(game))),map_get_monster_coor(level_get_curr_map(game_get_curr_level(game)),0),map_get_monster_coor(level_get_curr_map(game_get_curr_level(game)),1));
			map_monster_pop_untrig(level_get_curr_map(game_get_curr_level(game)));
			monster_set_type(list_get_value(game_get_list_monster(game),list_get_length(game_get_list_monster(game))),MONSTER);
		}
	}
	return 0;
}

void game_monster_move(struct game* game)
{
	struct list* list_monster=game_get_list_monster(game);
	struct map* map = level_get_curr_map(game_get_curr_level(game));
	for (int j=1 ; j<=list_get_length(list_monster) ; j++)
	{
		if(monster_on_map(list_get_value(list_monster,j) , level_get_current_map_nb(game_get_curr_level(game))))
		{
			switch(rand()%4)
			{
			case 0:
				monster_set_current_way(list_get_value(list_monster,j), WEST);
				monster_move(list_get_value(list_monster,j), map);
				break;
			case 1:
				monster_set_current_way(list_get_value(list_monster,j), NORTH);
				monster_move(list_get_value(list_monster,j), map);
				break;
			case 2:
				monster_set_current_way(list_get_value(list_monster,j), SOUTH);
				monster_move(list_get_value(list_monster,j), map);
				break;
			case 3:
				monster_set_current_way(list_get_value(list_monster,j), EAST);
				monster_move(list_get_value(list_monster,j), map);
				break;

			}
		}
	}
}

void game_pause(struct game* game)
{
	assert(game);
	game->pause=1;
	timer_pause_set(game_get_timer(game));
}

void game_unpause(struct game* game)
{
	assert(game);
	if ( ! timer_is_init(game_get_timer(game)))
		timer_init(game_get_timer(game),SDL_GetTicks(),ONE_SECOND);
	game->pause=0;
	timer_pause_unset(game_get_timer(game));
	game_update_timer(game);
}

int game_get_pause(struct game* game)
{
	assert(game);
	return game->pause;
}

void game_load(struct game* game)
{
	assert(game);

	int life, bomb_nb, bomb_range, key_nb, current_level, current_map,beast_life;
	char * file_name = "save/save.txt";

	FILE* file = fopen(file_name, "r");
	if (!file)
		printf("[ERROR] game_load_saved : Can't load file\n");
	assert(file);

	fscanf(file, "%d,%d,%d,%d\n", &life, &bomb_nb, &bomb_range ,&key_nb);
	fscanf(file, "%d,%d,%d\n", &current_level, &current_map,&beast_life);

	player_set_variables(game_get_player(game),   bomb_nb,  bomb_range,  key_nb,  life,  current_map,   current_level);

	game->player_death = 0;
	game->level=current_level;
	game->curr_level = level_load(game->level,current_map);
	player_from_map(game->player, level_get_map(game->curr_level, current_map));
	player_set_win(game->player,0);

	list_free(game_get_list_monster(game));

	monster_add(game);
	for (int i=0;i<=list_get_length(game_get_list_monster(game)) ; i++)
		if ( monster_get_type(list_get_value(game_get_list_monster(game),i)) == beast )
		{
			monster_set_nb_life(list_get_value(game_get_list_monster(game),i),beast_life);
			break;
		}

	while(game->pause_delay < SDL_GetTicks())
	{
		game_update_timer(game);
		game->pause_delay++;
	}
}

void game_save(struct game* game)
{
	assert(game);
	struct player* player = game_get_player(game);
	assert(player);
	int beast_life=0;
	struct level* level = game_get_curr_level(game);
	assert(level);
	struct list* list_monster = game_get_list_monster(game);
	assert(list_monster);
	for (int j=0 ; j<level_get_nb_map(level) ; j++)
		for (int i=0;i<=list_get_length(list_monster) ; i++)
			if ( j == monster_get_map(list_get_value(list_monster,i)))
				switch(monster_get_type(list_get_value(list_monster,i)))
				{
				case MONSTER:
					map_set_cell_type(level_get_map(level,j),monster_get_x(list_get_value(list_monster,i)),monster_get_y(list_get_value(list_monster,i)),CELL_MONSTER);
					break;
				case beast:
					map_set_cell_type(level_get_map(level,j),monster_get_x(list_get_value(list_monster,i)),monster_get_y(list_get_value(list_monster,i)),CELL_beast);
					beast_life=monster_get_nb_life(list_get_value(list_monster,i));
					break;
				}

	FILE* f = fopen("save/save.txt", "w");
	assert(f);
	fprintf(f,"%d, %d, %d, %d\n%d, %d, %d\n",player_get_nb_life(player),player_get_nb_bomb(player),player_get_bomb_range(player),player_get_key(player),game->level,level_get_current_map_nb(game_get_curr_level(game)),beast_life);
	fclose(f);

	level_save(game_get_curr_level(game),game->level);
	int begin = SDL_GetTicks();
	while( SDL_GetTicks()-begin < ONE_SECOND)
		game_display_text(game,PAUSE_SAVED);
}
