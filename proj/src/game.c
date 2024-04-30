#include "graphic.h"
#include "mouse.h"
#include "game.h"
#include <stdint.h>
#include <stdlib.h>

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

void handleMovimentEnemy(sprite_t* object, int elapsed_time) {
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
        object->y = 5;
    }
}

void get_each_digit(int n, int* array) {
    int it = 0;
    while (n > 0) {
        int digit = n % 10;  
        array[it] = digit;
        it++;
        n /= 10;             
    } 
}

void draw_numbers(int* array, int position_y) {
    int size = sizeof(array) / sizeof(array[0]);
    int position_x = 750;
    sprite_t* n;
    for (int i = 0; i < size; i++) {
        switch(array[i]) {
            case 0: n = sprite_ctor(NULL); sprite_set_pos(n, position_x, position_y); sprite_draw(n); break;
            case 1: n = sprite_ctor(NULL); sprite_set_pos(n, position_x, position_y); sprite_draw(n); break;
            case 2: n = sprite_ctor(NULL); sprite_set_pos(n, position_x, position_y); sprite_draw(n); break;
            case 3: n = sprite_ctor(NULL); sprite_set_pos(n, position_x, position_y); sprite_draw(n); break;
            case 4: n = sprite_ctor(NULL); sprite_set_pos(n, position_x, position_y); sprite_draw(n); break;
            case 5: n = sprite_ctor(NULL); sprite_set_pos(n, position_x, position_y); sprite_draw(n); break;
            case 6: n = sprite_ctor(NULL); sprite_set_pos(n, position_x, position_y); sprite_draw(n); break;
            case 7: n = sprite_ctor(NULL); sprite_set_pos(n, position_x, position_y); sprite_draw(n); break;
            case 8: n = sprite_ctor(NULL); sprite_set_pos(n, position_x, position_y); sprite_draw(n); break;
            case 9: n = sprite_ctor(NULL); sprite_set_pos(n, position_x, position_y); sprite_draw(n); break;
        }
        position_x = position_x - 50;
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
