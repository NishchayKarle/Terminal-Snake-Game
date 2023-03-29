#include "border_object.h"

#include <stdlib.h>

struct border_object_data {
    unsigned int x1;
    unsigned int y1;
    unsigned int x2;
    unsigned int y2;
};

static void border_object_draw(game_object_t * game, screen_t * screen) {
    border_object_data * d = (border_object_data *) game->data;
    screen_box(screen, d->x1, d->y1, d->x2, d->y2);
}

static const char * border_object_type(game_object_t * game) {
    return "border_object";
}

static bool border_object_overlaps(game_object_t * game, unsigned int x, 
    unsigned y) {
        border_object_data * d = (border_object_data *) game->data;
        return (x <= d->x1 
            || x >= d->x2
            || y <= d->y1
            || y >= d->y2);
}

bool border_object_overlaps_(game_object_t * game, unsigned int x, 
    unsigned y) {
        return border_object_overlaps(game, x, y);
}

static void border_object_destroy(game_object_t * game) {
    border_object_data * d = (border_object_data *) game->data;
    free(d);
    d = NULL;
    free(game->data);
    game->data = NULL;
    free(game);
    game = NULL;
}

static game_object_vtable_t border_object_vtable = {
    .draw = border_object_draw,
    .type = border_object_type,
    .overlaps = border_object_overlaps,
    .destroy = border_object_destroy
};

game_object_t * border_object_create(unsigned int x1, unsigned int y1, 
    unsigned int x2, unsigned int y2) {
        game_object_t * n = (game_object_t *)malloc(sizeof(*n));
        n->vtable = &border_object_vtable;

        border_object_data * d = (border_object_data *)malloc(sizeof(*d));
        d->x1 = x1;
        d->y1 = y1;
        d->x2 = x2;
        d->y2 = y2;
        n->data = d;
        return n;
}