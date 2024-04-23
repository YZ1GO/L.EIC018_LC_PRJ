#include "graphic.h"
#include "game.h"
#include <stdint.h>

bool check_collision(element_t el, player_t p) {
    if (el.position_x <= p.position_x && el.position_x + el.width >= p.position_x) {
        if (el.position_y <= el.position_y && el.position_y + el.height >= p.position_y) {
            return true;
        }
        else {
            return false;
        }
    }
    else {
        return false;
    }
}

void increase_score(int elapsed_time, player_t* p) {
    p->score += elapsed_time * 1.5 + 100;
}

void decrease_health(player_t* p) {
    p->health -= 30;
}

int get_x_player(player_t* p) {
    return p->position_x;
}

int get_y_player(player_t* p) {
    return p->position_y;
}

int get_score_player(player_t* p) {
    return p->score;
}

int get_health_player(player_t* p) {
    return p->health;
}

void draw_player(player_t p) {
    sprite_draw(p.sprite);
}

void handleMoviment(uint8_t scancode, sprite_t* sp) {
    vg_draw_rectangle(sp->x,sp->y,sp->w,sp->h, BLACK);
    switch(scancode) {
        case 0x17: sp->y = sp->y - 20; break;
        case 0x26: sp->x = sp->x + 20; break;
        case 0x25: sp->y = sp->y + 20; break;
        case 0x24: sp->x = sp->x - 20; break;
    }
    sprite_draw(sp);
}
