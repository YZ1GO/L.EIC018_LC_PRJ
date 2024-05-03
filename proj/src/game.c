#include "graphic.h"
#include "mouse.h"
#include "game.h"
#include <stdint.h>
#include <stdlib.h>

extern xpm_row_t ZERO_xpm[];
extern xpm_row_t ONE_xpm[];
extern xpm_row_t TWO_xpm[];
extern xpm_row_t THREE_xpm[];
extern xpm_row_t FOUR_xpm[];
extern xpm_row_t FIVE_xpm[];
extern xpm_row_t SIX_xpm[];
extern xpm_row_t SEVEN_xpm[];
extern xpm_row_t EIGHT_xpm[];
extern xpm_row_t NINE_xpm[];

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

void handleMoviment(uint8_t scancode, sprite_t* sp, int is_player) {
    if (is_player) {
        switch (scancode) {
            case 0x17: if (sp->y - 35 > 0) sp->y = sp->y - 35; break;
            case 0x26: if (sp->x + 35 + sp->w < 646) sp->x = sp->x + 35; break;
            case 0x25: if (sp->y + 35 + sp->h < 768) sp->y = sp->y + 35; break;
            case 0x24: if (sp->x - 35 > 0) sp->x = sp->x - 35; break;
        }
    }
    else {
        switch (scancode) {
            case 0x17: if (sp->y - 35 > 0) sp->y = sp->y - 35; break;
            case 0x26: if (sp->x + 35 + sp->w < 1024) sp->x = sp->x + 35; break;
            case 0x25: if (sp->y + 35 + sp->h < 768) sp->y = sp->y + 35; break;
            case 0x24: if (sp->x - 35 > 0) sp->x = sp->x - 35; break;
        }
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
        case 0x39: 
        if (check_collision_menu(play, cursor)) {
            *state = 1;
            vg_draw_rectangle(0, 0, 1024, 768, BLACK);
        }
        if (check_collision_menu(exit, cursor)) {
            *good = 0;
        } break;
    }
}

void drawMenu(sprite_t* play, sprite_t* exit, sprite_t* cursor, sprite_t* logo) {
    sprite_set_pos(cursor, cursor->x, cursor->y);
    sprite_draw(logo);
    sprite_draw(play);
    sprite_draw(exit);
    sprite_draw(cursor);
}

void handleMovimentEnemy(sprite_t* object, sprite_t* object2, int elapsed_time) {
    if (elapsed_time <= 10) {
        object->y = object->y + 3;
    }
    else if (elapsed_time > 10 && elapsed_time <= 20) {
        object->y = object->y + 5;

    }
    else if (elapsed_time > 20 && elapsed_time <= 30) {
        object->y = object->y + 7;
    }
    else if (elapsed_time > 30 && elapsed_time <= 40) {
        object->y = object->y + 9;
    }
    else {
        object->y = object->y + 11;
    }
    if (object->y + object->h > 768) {
        int new_x = 5 + rand() % (646 - object->w + 1);
        if (new_x == object2->x) {
            new_x += object->w;
            if (new_x > 646 - object->w) {
                new_x -= 2 * object->w;
            }
        }
        object->x = new_x;
        object->y = 5; 
    }
}

void draw_numbers(int number, int position_y) {
    int position_x = 950;
    sprite_t* n;
    while (number > 0) {
        int digit = number % 10;
        switch(digit) {
            case 0: vg_draw_rectangle(position_x, position_y, 50, 50, BLACK);
                n = sprite_ctor(ZERO_xpm); sprite_set_pos(n, position_x, position_y); sprite_draw(n); break;
            case 1: vg_draw_rectangle(position_x, position_y, 50, 50, BLACK);
                n = sprite_ctor(ONE_xpm); sprite_set_pos(n, position_x, position_y); sprite_draw(n); break;
            case 2: vg_draw_rectangle(position_x, position_y, 50, 50, BLACK);
                n = sprite_ctor(TWO_xpm); sprite_set_pos(n, position_x, position_y); sprite_draw(n); break;
            case 3: vg_draw_rectangle(position_x, position_y, 50, 50, BLACK);
                n = sprite_ctor(THREE_xpm); sprite_set_pos(n, position_x, position_y); sprite_draw(n); break;
            case 4: vg_draw_rectangle(position_x, position_y, 50, 50, BLACK);
                n = sprite_ctor(FOUR_xpm); sprite_set_pos(n, position_x, position_y); sprite_draw(n); break;
            case 5: vg_draw_rectangle(position_x, position_y, 50, 50, BLACK);
                n = sprite_ctor(FIVE_xpm); sprite_set_pos(n, position_x, position_y); sprite_draw(n); break;
            case 6: vg_draw_rectangle(position_x, position_y, 50, 50, BLACK);
                n = sprite_ctor(SIX_xpm); sprite_set_pos(n, position_x, position_y); sprite_draw(n); break;
            case 7: vg_draw_rectangle(position_x, position_y, 50, 50, BLACK);
                n = sprite_ctor(SEVEN_xpm); sprite_set_pos(n, position_x, position_y); sprite_draw(n); break;
            case 8: vg_draw_rectangle(position_x, position_y, 50, 50, BLACK);
                n = sprite_ctor(EIGHT_xpm); sprite_set_pos(n, position_x, position_y); sprite_draw(n); break;
            case 9: vg_draw_rectangle(position_x, position_y, 50, 50, BLACK);
                n = sprite_ctor(NINE_xpm); sprite_set_pos(n, position_x, position_y); sprite_draw(n); break;
            default: break;
        }
        position_x = position_x - 50;
        number /= 10;
    }
}

bool check_collision(sprite_t* player, sprite_t* object) {
    if (player->x <= object->x && player->x + player->w >= object->x) {
        if (player->y <= object->y && player->y + player->h >= object->y) {
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
