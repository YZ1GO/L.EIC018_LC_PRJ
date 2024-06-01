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

/**
 * @brief Updates the health of the player.
 *
 * @param hearts An array of sprites representing the player's full hearts.
 * @param half_heart A sprite representing a half heart.
 * @param health The player's current health.
 */
void updateHealth(sprite_t* hearts[], sprite_t* half_heart, float health);

/**
 * @brief Updates the shots information.
 *
 * @param shotsInfo An array of sprites representing the shots left.
 * @param num_shots The current number of shots used.
 * @param emptyxpm A sprite representing empty shots.
 */
void updateShots(sprite_t* shotsInfo[], int* num_shots, sprite_t* emptyxpm);

/**
 * @brief Checks if there is a collision in the menu.
 *
 * @param el A sprite representing an element in the menu.
 * @param cursor A sprite representing the cursor.
 * @return true if there is a collision, false otherwise.
 */
bool check_collision_menu(sprite_t* el, sprite_t* cursor);

/**
 * @brief Handles the movement of a sprite based on a scancode.
 *
 * @param scancode The scancode representing the key pressed.
 * @param sp The sprite to move.
 * @param is_player A flag indicating whether the sprite is the player.
 */
void handleMovement(uint8_t scancode, sprite_t* sp, int is_player);

/**
 * @brief Handles the movement of a sprite based on mouse input.
 *
 * @param pp A packet representing the mouse input.
 * @param sp The sprite to move.
 */
void handleMovementCursorMouse(struct packet* pp, sprite_t* sp);

/**
 * @brief Handles a keyboard click event.
 *
 * @param scancode The scancode representing the key pressed.
 * @param cursor A sprite representing the cursor.
 * @param play A sprite representing the play button.
 * @param exit A sprite representing the exit button.
 * @param state The current state of the game.
 * @param good A flag indicating whether the game is in a good state.
 * @param game The current game.
 * @param enemies An array of sprites representing the enemies.
 * @param num_shots The current number of shots.
 * @param last_collision_time The time of the last collision.
 * @param explosion_time The time of the last explosion.
 */
void handleClick(uint8_t scancode, sprite_t* cursor, sprite_t* play, sprite_t* exit, int* state, int* good, game_t* game, sprite_t* enemies[], int* num_shots, int* last_collision_time, int* explosion_time);

/**
 * @brief Draws the menu.
 *
 * @param play A sprite representing the play button.
 * @param exit A sprite representing the exit button.
 * @param cursor A sprite representing the cursor.
 * @param logo A sprite representing the logo.
 */
void drawMenu(sprite_t* play, sprite_t* exit, sprite_t* cursor, sprite_t* logo);

/**
 * @brief Draws the retry menu.
 *
 * @param play A sprite representing the play button.
 * @param exit A sprite representing the exit button.
 * @param cursor A sprite representing the cursor.
 * @param textScore A sprite representing the score text.
 * @param score The current score.
 */
void drawRetryMenu(sprite_t* play, sprite_t* exit, sprite_t* cursor, sprite_t* textScore, int score);

/**
 * @brief Calculates a new x position for an enemy.
 *
 * @param enemyA_width The width of enemy A.
 * @param enemyB_x The x position of enemy B.
 * @return The new x position.
 */
int calculate_new_x(int enemyA_width, int enemyB_x);

/**
 * @brief Handles the vertical movement of two enemies.
 *
 * @param verticalEnemy1 The first enemy.
 * @param verticalEnemy2 The second enemy.
 * @param elapsed_time The elapsed time since the last frame.
 */
void handleVerticalMovementEnemy(sprite_t* verticalEnemy1, sprite_t* verticalEnemy2, int elapsed_time);

/**
 * @brief Calculates a new y position for an enemy.
 *
 * @param enemyA_height The height of enemy A.
 * @param enemyB_y The y position of enemy B.
 * @return The new y position.
 */
int calculate_new_y(int enemyA_height, int enemyB_y);

/**
 * @brief Handles the horizontal movement of two enemies.
 *
 * @param leftToRightEnemy The enemy moving from left to right.
 * @param rightToLeftEnemy The enemy moving from right to left.
 * @param elapsed_time The elapsed time since the last frame.
 */
void handleHorizontalMovementEnemy(sprite_t* leftToRightEnemy, sprite_t* rightToLeftEnemy, int elapsed_time);

/**
 * @brief Draws a number at a specific position.
 *
 * @param number The number to draw.
 * @param position_x The x position to draw the number at.
 * @param position_y The y position to draw the number at.
 */
void draw_numbers(int number, int position_x, int position_y);

/**
 * @brief Draws a string representing time at a specific position.
 *
 * @param string The string to draw.
 * @param position_x The x position to draw the string at.
 * @param position_y The y position to draw the string at.
 */
void draw_numbers_time(char* string, int position_x, int position_y);

/**
 * @brief Checks if there is a collision between the player and an enemy.
 *
 * @param player The player sprite.
 * @param explosion The explosion sprite.
 * @param enemy The enemy sprite.
 * @param enemies An array of sprites representing the enemies.
 * @param enemy_index The index of the enemy to check.
 * @param last_collision_time The time of the last collision.
 * @param explosion_time The time of the last explosion.
 * @param elapsed_time The elapsed time since the last frame.
 * @return true if there is a collision, false otherwise.
 */
bool check_player_collision(sprite_t* player, sprite_t* explosion, sprite_t* enemy, sprite_t* enemies[], int enemy_index, int* last_collision_time, int* explosion_time, int elapsed_time);

/**
 * @brief Checks if there is a collision between a shot and an enemy.
 *
 * @param shot The shot sprite.
 * @param explosion The explosion sprite.
 * @param enemy The enemy sprite.
 * @param enemies An array of sprites representing the enemies.
 * @param enemy_index The index of the enemy to check.
 * @param last_collision_time The time of the last collision.
 * @param explosion_time The time of the last explosion.
 * @param elapsed_time The elapsed time since the last frame.
 * @param shots An array of sprites representing the shots.
 * @param num_shots The current number of shots.
 * @param shot_index The index of the shot to check.
 * @return true if there is a collision, false otherwise.
 */
bool check_shot_collision(sprite_t* shot, sprite_t* explosion, sprite_t* enemy, sprite_t* enemies[], int enemy_index, int* last_collision_time, int* explosion_time, int elapsed_time, sprite_t* shots[], int* num_shots, int shot_index);

/**
 * @brief Handles the movement of a shot.
 *
 * @param shots An array of sprites representing the shots.
 * @param index The index of the shot to move.
 */
void handleMovementShot(sprite_t* shots[], int index);

/**
 * @brief Handles the spawning of a shot.
 *
 * @param scancode The scancode representing the key pressed.
 * @param player The player sprite.
 * @param shots An array of sprites representing the shots.
 * @param num_shots The current number of shots.
 */
void handleShotSpawn(uint8_t scancode, sprite_t* player, sprite_t* shots[], int* num_shots);

