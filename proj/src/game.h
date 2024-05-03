#include "sprite.h"
#include <stdint.h>

typedef struct {
    int health;
    int score;
} game_t;

bool check_collision_menu(sprite_t* el, sprite_t* cursor);
void handleMoviment(uint8_t scancode, sprite_t* sp, int is_player);
void handleMovimentCursorMouse(struct packet* pp, sprite_t* sp);
void handleClick(uint8_t scancode, sprite_t* cursor, sprite_t* play, sprite_t* exit, int* state, int* good);
void drawMenu(sprite_t* play, sprite_t* exit, sprite_t* cursor, sprite_t* logo);
void handleVerticalMovementEnemy(sprite_t* object, sprite_t* object2, int elapsed_time);
void handleHorizontalMovementEnemy(sprite_t* object, sprite_t* object2, int elapsed_time);
void draw_numbers(int number, int position_y);
bool check_collision(sprite_t* player, sprite_t* object);
