#include "sprite.h"
#include <stdint.h>

#define COOLDOWN_PERIOD 1
#define ARENA_HEIGHT 768
#define ARENA_WIDTH 646
#define PLAYER_X 200
#define PLAYER_Y 500
#define V_ENEMY1_X 200
#define V_ENEMY1_Y 5
#define V_ENEMY2_X 400
#define V_ENEMY2_Y -100
#define LR_ENEMY_X 5
#define LR_ENEMY_Y 300
#define RL_ENEMY_X 610
#define RL_ENEMY_Y 100

typedef struct {
    int health;
    int score;
} game_t;

bool check_collision_menu(sprite_t* el, sprite_t* cursor);
void handleMoviment(uint8_t scancode, sprite_t* sp, int is_player);
void handleMovimentCursorMouse(struct packet* pp, sprite_t* sp);
void handleClick(uint8_t scancode, sprite_t* cursor, sprite_t* play, sprite_t* exit, int* state, int* good, game_t* game, int* last_collision_time, int* explosion_time);
void drawMenu(sprite_t* play, sprite_t* exit, sprite_t* cursor, sprite_t* logo);
void drawRetryMenu(sprite_t* play, sprite_t* exit, sprite_t* cursor, sprite_t* textScore, int score);
int calculate_new_x(int enemyA_width, int enemyB_x);
void handleVerticalMovementEnemy(sprite_t* verticalEnemy1, sprite_t* verticalEnemy2, int elapsed_time);
int calculate_new_y(int enemyA_height, int enemyB_y);
void handleHorizontalMovementEnemy(sprite_t* leftToRightEnemy, sprite_t* rightToLeftEnemy, int elapsed_time);
void draw_numbers(int number, int position_y, int position_x);
void draw_numbers_time(char* string, int position_y, int position_x);
bool check_collision(sprite_t* player, sprite_t* explosion, sprite_t* enemy, sprite_t* enemies[], int enemy_index, int* last_collision_time, int* explosion_time, int elapsed_time);
