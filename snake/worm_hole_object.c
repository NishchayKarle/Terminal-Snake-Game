#include "worm_hole_object.h"
#include <stdlib.h>

struct worm_hole_object_data {
    unsigned int x1;
    unsigned int y1;
    unsigned int x2;
    unsigned int y2;
};

static void worm_hole_object_draw(game_object_t * game, screen_t * screen) {
    worm_hole_object_data * d = (worm_hole_object_data *) game->data;
    screen_set(screen, d->x1, d->y1, 'O');
    screen_set(screen, d->x2, d->y2, 'O');
}

static const char * worm_hole_object_type(game_object_t * game) {
    return "worm_hole_object";
}

static bool worm_hole_object_overlaps(game_object_t * game,
    unsigned int x, unsigned int y) {
        worm_hole_object_data * d = (worm_hole_object_data *) game->data;
        return ((d->x1 == x && d->y1 == y)
            || (d->x2 == x && d->y2 == y));
}

bool worm_hole_object_overlaps_(game_object_t * game,
    unsigned int x, unsigned int y) {
        return worm_hole_object_overlaps(game, x, y);
}

const coordinate_t * worm_hole_exit(game_object_t * game, 
    unsigned int x, unsigned int y) {
        worm_hole_object_data * d = (worm_hole_object_data *) game->data;
        coordinate_t * c = (coordinate_t *)malloc(sizeof(*c));
        if (d->x1 == x && d->y1 == y) {
            c->x = d->x2;
            c->y = d->y2;
        }
        
        else {
            c->x = d->x1;
            c->y = d->y1;
        }

        return c;
}

static void worm_hole_object_destroy(game_object_t * game) {
    worm_hole_object_data * d = (worm_hole_object_data *)game->data;
    free(d);
    d = NULL;
    free(game->data);
    game->data = NULL;
    free(game);
    game = NULL;
}

static game_object_vtable_t worm_hole_object_vtable = {
    .draw = worm_hole_object_draw,
    .type = worm_hole_object_type,
    .overlaps = worm_hole_object_overlaps,
    .destroy = worm_hole_object_destroy
};

game_object_t * worm_hole_object_create(unsigned int x1, unsigned int y1, 
    unsigned int x2, unsigned int y2) {
        game_object_t * n = (game_object_t *)malloc(sizeof(*n));
        n->vtable = &worm_hole_object_vtable;

        worm_hole_object_data * d = (worm_hole_object_data *)malloc(sizeof(*d));
        d->x1 = x1;
        d->x2 = x2;
        d->y1 = y1;
        d->y2 = y2;
        n->data = d;
        return n;
}