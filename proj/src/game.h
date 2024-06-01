#include "sprite.h"
#include <stdint.h>

#define GRAPHICS_WIDTH 1152
#define GRAPHICS_HEIGHT 864
#define COOLDOWN_PERIOD 1
#define ARENA_HEIGHT 794
#define ARENA_WIDTH 650
#define PLAYER_X 300
#define PLAYER_Y 660
#define PLAYER_STEP 36
#define V_ENEMY1_X 200
#define V_ENEMY1_Y 5
#define V_ENEMY2_X 400
#define V_ENEMY2_Y -100
#define LR_ENEMY_X 5
#define LR_ENEMY_Y 300
#define RL_ENEMY_X 610
#define RL_ENEMY_Y 100
#define V_ENEMIES_SPEED_CAP 100
#define H_ENEMIES_SPEED_CAP 80
#define MAX_SHOTS 10
#define SHOT_SPEED 5
#define INITIAL_HEALTH 3.0
#define MAX_HEALTH 5
#define I_KEY 0x17
#define K_KEY 0x25
#define J_KEY 0x24
#define L_KEY 0x26
#define SPACE_KEY 0x39

typedef struct {
    float health;
    int score;
} game_t;

void updateHealth(sprite_t* hearts[], sprite_t* half_heart, float health);
bool check_collision_menu(sprite_t* el, sprite_t* cursor);
void handleMovement(uint8_t scancode, sprite_t* sp, int is_player);
void handleMovementCursorMouse(struct packet* pp, sprite_t* sp);
void handleClick(uint8_t scancode, sprite_t* cursor, sprite_t* play, sprite_t* exit, int* state, int* good, game_t* game, sprite_t* enemies[], int* num_shots, int* last_collision_time, int* explosion_time);
void drawMenu(sprite_t* play, sprite_t* exit, sprite_t* cursor, sprite_t* logo);
void drawRetryMenu(sprite_t* play, sprite_t* exit, sprite_t* cursor, sprite_t* textScore, int score);
int calculate_new_x(int enemyA_width, int enemyB_x);
void handleVerticalMovementEnemy(sprite_t* verticalEnemy1, sprite_t* verticalEnemy2, int elapsed_time);
int calculate_new_y(int enemyA_height, int enemyB_y);
void handleHorizontalMovementEnemy(sprite_t* leftToRightEnemy, sprite_t* rightToLeftEnemy, int elapsed_time);
void draw_numbers(int number, int position_x, int position_y);
void draw_numbers_time(char* string, int position_x, int position_y);
bool check_player_collision(sprite_t* player, sprite_t* explosion, sprite_t* enemy, sprite_t* enemies[], int enemy_index, int* last_collision_time, int* explosion_time, int elapsed_time);
bool check_shot_collision(sprite_t* shot, sprite_t* explosion, sprite_t* enemy, sprite_t* enemies[], int enemy_index, int* last_collision_time, int* explosion_time, int elapsed_time, sprite_t* shots[], int* num_shots, int shot_index);
void handleMovementShot(sprite_t* shots[], int index);
void handleShotSpawn(uint8_t scancode, sprite_t* player, sprite_t* shots[], int* num_shots);

