#include "graphic.h"
#include "mouse.h"
#include "game.h"
#include <stdint.h>

bool check_collision_menu(sprite_t* el, sprite_t* cursor) {
    if (el->x <= cursor->x && el->x + el->w >= cursor->x) {
        if (el->y <= cursor->y && el->y + el->h >= cursor->y) {
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

void handleMovimentCursor(uint8_t scancode, sprite_t* sp) {
    switch (scancode) {
        case 0x17: sp->y = sp->y - 20; break;
        case 0x26: sp->x = sp->x + 20; break;
        case 0x25: sp->y = sp->y + 20; break;
        case 0x24: sp->x = sp->x - 20; break;
    }
}

void handleMovimentCursorMouse(struct packet* pp, sprite_t* sp) {
    uint8_t b[3];
    for (int i = 0; i < 3; i++) {
       b[i] = pp->bytes[i];
    }
    int16_t x = pp->delta_x;
    int16_t y = pp->delta_y;
    if (b[0] & MSB_X_DELTA) {
        x |= 0xFF00; 
    }
    if (b[0] & MSB_Y_DELTA) {
        y |= 0xFF00;
    }
    sp->x = sp->x + (int8_t)x;
    sp->y = sp->y - (int8_t)y;
}

void handleClick(uint8_t scancode, sprite_t* cursor, sprite_t* play, sprite_t* exit, int* state, int* good) {
    switch (scancode) {
        case 0x81: *good = 0; break;
        case 0x39: if (check_collision_menu(play, cursor)) {
            *state = 1;
        }
        if (check_collision_menu(exit, cursor)) {
            *good = 0;
        } break;
    }
}

void drawMenu(sprite_t* play, sprite_t* exit, sprite_t* cursor) {
    sprite_set_pos(cursor, cursor->x, cursor->y);
    sprite_set_pos(play, 450, 300);
    sprite_set_pos(exit, 450, 400);
    sprite_draw(play);
    sprite_draw(exit);
    sprite_draw(cursor);
}
