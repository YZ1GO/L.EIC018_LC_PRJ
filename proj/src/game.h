#include "sprite.h"

struct player {
    sprite_t* sprite;
    int position_x;
    int position_y;
    int height;
    int width;
    int health;
    int score;
};

typedef struct player player_t;

struct element {
    sprite_t* sprite;
    int position_x;
    int position_y;
    int height;
    int wdith;
    int velocity;
};

typedef struct element element_t;

bool check_collision(element_t el, player_t p);
void increase_score(int elapsed_time, player_t* p);
void decrease_health(player_t* p);
int get_x_player(player_t* p);
int get_y_player(player_t* p);
int get_score_player(player_t* p);
int get_health_player(player_t* p);
void draw_player(player_t p);
