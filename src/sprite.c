#include <SDL/SDL_image.h>
#include <assert.h>
#include "../include/sprite.h"
#include "../include/misc.h"
#include "../include/map.h"
#include "../include/constant.h"
#include "../include/game.h"

// Scenery goals
#define MAP_FLAG        "sprite/flag.png"
#define MAP_BOMBERWOMAN "sprite/bomberwoman.png"


#define IMG_PAUSE		"sprite/pause1.bmp"


// Sprites general
#define MAP_CASE        "sprite/wood_box.png"
#define MAP_KEY			"sprite/key.png"
#define MAP_DOOR		"sprite/door.png"
#define MAP_CLOSED_DOOR	"sprite/closed_door.png"

// Scenery elements
#define MAP_STONE		"sprite/stone.png"
#define MAP_TREE        "sprite/tree.png"

// Sprites of Banner
#define BANNER_LINE		"sprite/banner_line.png"
#define BANNER_LIFE		"sprite/banner_life.png"
#define BANNER_BOMB		"sprite/bomb3.png"
#define BANNER_RANGE	"sprite/banner_range.png"
#define BANNER_0		"sprite/banner_0.jpg"
#define BANNER_1		"sprite/banner_1.jpg"
#define BANNER_2		"sprite/banner_2.jpg"
#define BANNER_3		"sprite/banner_3.jpg"
#define BANNER_4		"sprite/banner_4.jpg"
#define BANNER_5		"sprite/banner_5.jpg"
#define BANNER_6		"sprite/banner_6.jpg"
#define BANNER_7		"sprite/banner_7.jpg"
#define BANNER_8		"sprite/banner_8.jpg"
#define BANNER_9		"sprite/banner_9.jpg"

// Sprites of Bombs
#define IMG_BOMB_TTL1       "sprite/bomb1.png"
#define IMG_BOMB_TTL2       "sprite/bomb2.png"
#define IMG_BOMB_TTL3       "sprite/bomb3.png"
#define IMG_BOMB_TTL4       "sprite/bomb4.png"
#define IMG_BOMB_FIRE       "sprite/fire.png"


// Sprites of Bonus
#define IMG_BONUS_BOMB_RANGE_INC  "sprite/bonus_bomb_range_inc.png"
#define IMG_BONUS_BOMB_RANGE_DEC  "sprite/bonus_bomb_range_dec.png"
#define IMG_BONUS_BOMB_NB_INC     "sprite/bonus_bomb_nb_inc.png"
#define IMG_BONUS_BOMB_NB_DEC     "sprite/bonus_bomb_nb_dec.png"

// Sprites of Players
#define PLAYER_LEFT     "sprite/player_left.png"
#define PLAYER_UP       "sprite/player_up.png"
#define PLAYER_RIGHT    "sprite/player_right.png"
#define PLAYER_DOWN     "sprite/player_down.png"

// Sprites of monsters
#define MONSTER_LEFT     "sprite/monster_left.png"
#define MONSTER_UP       "sprite/monster_up.png"
#define MONSTER_RIGHT    "sprite/monster_right.png"
#define MONSTER_DOWN     "sprite/monster_down.png"

// Sprites of Players
#define beast_LEFT     "sprite/beast_left.png"
#define beast_UP       "sprite/beast_up.png"
#define beast_RIGHT     "sprite/beast_right.png"
#define beast_DOWN     "sprite/beast_down.png"

// banner
SDL_Surface* numbers[10];
SDL_Surface* banner_life;
SDL_Surface* banner_bomb;
SDL_Surface* banner_range;
SDL_Surface* banner_line;

// map
SDL_Surface* box1;
SDL_Surface* goal;
SDL_Surface* key;
SDL_Surface* door;
SDL_Surface* closed_door;
SDL_Surface* stone;
SDL_Surface* tree;
SDL_Surface* flag;
SDL_Surface* bomberwoman;

SDL_Surface* pause;

// bonus
#define NB_BONUS 6
SDL_Surface* bonus[NB_BONUS];

// bonus
#define NB_BOMB 5
SDL_Surface* bomb[NB_BOMB];

// player
SDL_Surface* player_img[4];

SDL_Surface* text_img[5];

SDL_Surface* monster_img[4];
SDL_Surface* beast_img[4];

void banner_load() {
	// numbers imgs
	numbers[0] = load_image(BANNER_0);
	numbers[1] = load_image(BANNER_1);
	numbers[2] = load_image(BANNER_2);
	numbers[3] = load_image(BANNER_3);
	numbers[4] = load_image(BANNER_4);
	numbers[5] = load_image(BANNER_5);
	numbers[6] = load_image(BANNER_6);
	numbers[7] = load_image(BANNER_7);
	numbers[8] = load_image(BANNER_8);
	numbers[9] = load_image(BANNER_9);

	// other banner sprites
	banner_life = load_image(BANNER_LIFE);
	banner_bomb = load_image(BANNER_BOMB);
	banner_range = load_image(BANNER_RANGE);
	banner_line = load_image(BANNER_LINE);
}

void banner_unload() {
	// numbers imgs
	for (int i = 0; i < 10; i++) {
		SDL_FreeSurface(numbers[i]);
	}

	// other banner sprites
	SDL_FreeSurface(banner_life);
	SDL_FreeSurface(banner_bomb);
	SDL_FreeSurface(banner_range);
	SDL_FreeSurface(banner_line);
}

void map_load_1() {
	// Sprite loading
	tree = load_image(MAP_TREE);
	box1 = load_image(MAP_CASE);
	key = load_image(MAP_KEY);
	stone = load_image(MAP_STONE);
	door = load_image(MAP_DOOR);
	closed_door = load_image(MAP_CLOSED_DOOR);
	flag = load_image(MAP_FLAG);
	bomberwoman = load_image(MAP_BOMBERWOMAN);
}

void map_unload() {
	SDL_FreeSurface(tree);
	SDL_FreeSurface(box1);
	SDL_FreeSurface(goal);
	SDL_FreeSurface(key);
	SDL_FreeSurface(stone);
	SDL_FreeSurface(door);
	SDL_FreeSurface(closed_door);
	SDL_FreeSurface(flag);
	SDL_FreeSurface(bomberwoman);
}

void pause_load() {
	// Sprite loading
	pause = SDL_LoadBMP(IMG_PAUSE);
}

void pause_unload() {
	SDL_FreeSurface(pause);
}

void text_load() {
	text_img[PAUSE_TEXT_1] = SDL_LoadBMP(IMG_PAUSE);
	text_img[PAUSE_TEXT_2] = SDL_LoadBMP(IMG_PAUSE);
	text_img[PAUSE_WIN] = SDL_LoadBMP(IMG_PAUSE);
	text_img[PAUSE_SAVED] = SDL_LoadBMP(IMG_PAUSE);
	text_img[PAUSE_LOOSE] = SDL_LoadBMP(IMG_PAUSE);
}

void text_unload() {
	for (int i = 0; i < 5; i++)
		SDL_FreeSurface(text_img[i]);
}

void monster_load() {
	monster_img[WEST] = load_image(MONSTER_LEFT);
	monster_img[NORTH] = load_image(MONSTER_UP);
	monster_img[EAST] = load_image(MONSTER_RIGHT);
	monster_img[SOUTH] = load_image(MONSTER_DOWN);
}

void monster_unload() {
	for (int i = 0; i < 4; i++)
		SDL_FreeSurface(monster_img[i]);
}

void beast_load() {
	beast_img[WEST] = load_image(beast_LEFT);
	beast_img[NORTH] = load_image(beast_UP);
	beast_img[EAST] = load_image(beast_RIGHT);
	beast_img[SOUTH] = load_image(beast_DOWN);
}

void beast_unload() {
	for (int i = 0; i < 4; i++)
		SDL_FreeSurface(beast_img[i]);
}

void bonus_load() {
	bonus[BONUS_BOMB_RANGE_INC] = load_image(IMG_BONUS_BOMB_RANGE_INC);
	bonus[BONUS_BOMB_RANGE_DEC] = load_image(IMG_BONUS_BOMB_RANGE_DEC);
	bonus[BONUS_BOMB_NB_INC] = load_image(IMG_BONUS_BOMB_NB_INC);
	bonus[BONUS_BOMB_NB_DEC] = load_image(IMG_BONUS_BOMB_NB_DEC);
	bonus[BONUS_LIFE] = load_image(BANNER_LIFE);
	bonus[BONUS_MONSTER] = load_image(MONSTER_LEFT );
}

void bonus_unload() {
	for (int i = 0; i < NB_BONUS; i++)
		SDL_FreeSurface(bonus[i]);
}

void bomb_load() {
	bomb[FIRST] = load_image(IMG_BOMB_TTL4);
	bomb[SECOND] = load_image(IMG_BOMB_TTL3);
	bomb[THIRD] = load_image(IMG_BOMB_TTL2);
	bomb[FOURTH] = load_image(IMG_BOMB_TTL1);
	bomb[FIRE] = load_image(IMG_BOMB_FIRE);


}

void bomb_unload() {
	for (int i = 0; i < NB_BOMB; i++)
		SDL_FreeSurface(bomb[i]);
}

void player_load() {
	player_img[WEST] = load_image(PLAYER_LEFT);
	player_img[EAST] = load_image(PLAYER_RIGHT);
	player_img[NORTH] = load_image(PLAYER_UP);
	player_img[SOUTH] = load_image(PLAYER_DOWN);
}

void player_unload() {
	for (int i = 0; i < 4; i++)
		SDL_FreeSurface(player_img[i]);
}

void sprite_load() {
	map_load_1();
	text_load();
	bomb_load();
	bonus_load();
	banner_load();
	player_load();
	beast_load();
	monster_load();
	pause_load();
}

void sprite_free() {
	map_unload();
	text_unload();
	bomb_unload();
	bonus_unload();
	banner_unload();
	player_unload();
	monster_unload();
	beast_unload();
	pause_unload();
}

SDL_Surface* sprite_get_number(short number) {
	assert(number >= 0 && number < 9);
	return numbers[number];
}

SDL_Surface* sprite_get_player(enum way direction) {
	assert(player_img[direction]);
	return player_img[direction];
}

SDL_Surface* sprite_get_text(int num) {
	assert(player_img[num]);
	return player_img[num];
}

SDL_Surface* sprite_get_banner_life() {
	assert(banner_life);
	return banner_life;
}

SDL_Surface* sprite_get_banner_bomb() {
	assert(banner_bomb);
	return banner_bomb;
}

SDL_Surface* sprite_get_banner_line() {
	assert(banner_line);
	return banner_line;
}

SDL_Surface* sprite_get_banner_range() {
	assert(banner_range);
	return banner_range;
}

SDL_Surface* sprite_get_bonus(bonus_type_t bonus_type) {
	assert(bonus[bonus_type]);
	return bonus[bonus_type];
}
SDL_Surface* sprite_get_bomb(enum e_state bomb_type) {
	assert(bomb[bomb_type]);
	return bomb[bomb_type];
}

SDL_Surface* sprite_get_monster(enum way direction) {
	assert(monster_img[direction]);
	return monster_img[direction];
}

SDL_Surface* sprite_get_beast(enum way direction) {
	assert(beast_img[direction]);
	return beast_img[direction];
}

SDL_Surface* sprite_get_flag() {
	assert(flag);
	return stone;
}

SDL_Surface* sprite_get_bomberwoman() {
	assert(bomberwoman);
	return stone;
}
SDL_Surface* sprite_get_tree() {
	assert(tree);
	return tree;
}

SDL_Surface* sprite_get_box() {
	assert(box1);
	return box1;
}

SDL_Surface* sprite_get_key() {
	assert(key);
	return key;
}

SDL_Surface* sprite_get_stone() {
	assert(stone);
	return stone;
}

SDL_Surface* sprite_get_pause() {
	assert(pause);
	return pause;
}

SDL_Surface* sprite_get_door() {
	assert(door);
	return door;
}

SDL_Surface* sprite_get_closed_door() {
	assert(closed_door);
	return closed_door;
}
