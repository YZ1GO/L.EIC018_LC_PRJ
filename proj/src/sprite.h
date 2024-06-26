#include <lcom/lcf.h>

#ifndef SPRITE_H_INCLUDED
#define SPRITE_H_INCLUDED

struct sprite_t{
    int x, y;
    int w, h;
    int vx, vy;
    uint8_t *map;
};

typedef struct sprite_t sprite_t;

sprite_t* sprite_ctor(const xpm_map_t xpm);
void      sprite_dtor(sprite_t *p);

void sprite_set_x(sprite_t *p, int x);
void sprite_set_y(sprite_t *p, int y);
void sprite_set_pos(sprite_t *p, int x, int y);

int sprite_get_h(const sprite_t *p);
int sprite_get_w(const sprite_t *p);

void sprite_set_vx(sprite_t *p, int vx);
void sprite_set_vy(sprite_t *p, int vy);

void sprite_draw(const sprite_t *p);

#endif //SPRITE_H_INCLUDED
