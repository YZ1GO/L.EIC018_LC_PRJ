#include "sprite.h"
#include <stdint.h>

#define COOLDOWN_PERIOD 2

typedef struct {
    int health;
    int score;
} game_t;

bool check_collision_menu(sprite_t* el, sprite_t* cursor);
void handleMoviment(uint8_t scancode, sprite_t* sp, int is_player);
void handleMovimentCursorMouse(struct packet* pp, sprite_t* sp);
void handleClick(uint8_t scancode, sprite_t* cursor, sprite_t* play, sprite_t* exit, int* state, int* good);
void drawMenu(sprite_t* play, sprite_t* exit, sprite_t* cursor, sprite_t* logo);
void handleVerticalMovementEnemy(sprite_t* verticalEnemy1, sprite_t* verticalEnemy2, int elapsed_time);
void handleHorizontalMovementEnemy(sprite_t* leftToRightEnemy, sprite_t* rightToLeftEnemy, int elapsed_time);
void draw_numbers(int number, int position_y);
bool check_collision(sprite_t* player, sprite_t* object, int* last_collision_time, int elapsed_time);
