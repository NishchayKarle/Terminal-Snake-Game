#include "food_object.h"

#include "game_object.h"

#include <assert.h>
#include <stdlib.h>

struct food_object_data {
    unsigned int x;
    unsigned int y;
};

static void food_object_draw (game_object_t * game, screen_t * screen) {
    food_object_data * d = (food_object_data *) game->data;
    screen_set(screen, d->x, d->y, SCREEN_CHAR_FOOD);
}

static const char * food_object_type (game_object_t * game)
{
    return "food_object";
}

static bool food_object_overlaps (game_object_t * game,
        unsigned int x, unsigned int y) {
    food_object_data * d = (food_object_data *) game->data;
    return (x == d->x && y == d->y);
}

bool food_object_overlaps_(game_object_t * game, unsigned int x,
    unsigned int y) {
        return food_object_overlaps(game, x, y);
}

static void food_object_destroy(game_object_t * game) {
    food_object_data * d = (food_object_data*)game->data;
    free(d);
    d = NULL;
    free(game->data);
    game->data = NULL;
    free(game);
    game = NULL;
}

void food_object_move(game_object_t * game, unsigned int x, unsigned int y) {
    food_object_data * d = (food_object_data *) game->data;
    d->x = x;
    d->y = y;
}

static game_object_vtable_t food_object_vtable = {
    .draw = food_object_draw,
    .type = food_object_type,
    .overlaps = food_object_overlaps,
    .destroy = food_object_destroy
};

game_object_t * food_object_create(unsigned int x1, unsigned int y1) {
    game_object_t * n = (game_object_t *) malloc(sizeof(*n));
    n->vtable = &food_object_vtable;

    food_object_data * d = (food_object_data*) malloc(sizeof(*d));
    d->x = x1;
    d->y = y1;
    n->data = d;
    return n;
}
