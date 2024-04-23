#include "sprite.h"
#include <stdint.h>

typedef struct {
    sprite_t* sprite;
    int position_x;
    int position_y;
    int height;
    int width;
    int health;
    int score;
} player_t;

typedef struct {
    sprite_t* sprite;
    int position_x;
    int position_y;
    int height;
    int width;
    int velocity;
} element_t;

bool check_collision_menu(sprite_t* el, sprite_t* cursor);
void increase_score(int elapsed_time, player_t* p);
void decrease_health(player_t* p);
int get_x_player(player_t* p);
int get_y_player(player_t* p);
int get_score_player(player_t* p);
int get_health_player(player_t* p);
void draw_player(player_t p);
void handleMovimentCursor(uint8_t scancode, sprite_t* sp);
void handleMovimentCursorMouse(struct packet* pp, sprite_t* sp);
void handleClick(uint8_t scancode, sprite_t* cursor, sprite_t* play, sprite_t* exit, int* state, int* good);
void drawMenu(sprite_t* play, sprite_t* exit, sprite_t* cursor);
