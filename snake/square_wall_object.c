#include "square_wall_object.h"

#include <stdlib.h>

struct square_wall_object_data
{
    unsigned int x;
    unsigned int y;
};


static void square_wall_draw (game_object_t * game, screen_t * screen)
{
    square_wall_object_data * d = (square_wall_object_data *) game->data;
    for (unsigned int i = 0; i < screen_getmaxx(screen)-d->x; i++)
        screen_set(screen, i+d->x, d->y, SCREEN_CHAR_WALL);
}

static const char * square_wall_type (game_object_t * game)
{
    return "square_wall_object";
}

static bool square_wall_overlaps (game_object_t * game,
        unsigned int x, unsigned int y)
{
    square_wall_object_data * d = (square_wall_object_data *) game->data;
    return (x >= d->x && y == d->y);
}

bool square_wall_overlaps_(game_object_t * game, 
    unsigned int x, unsigned int y){
        return square_wall_overlaps(game, x, y);
}

static void square_wall_destroy(game_object_t * game)
{
    square_wall_object_data * d = (square_wall_object_data*) game->data;
    free(d);
    d = NULL;
    free(game->data);
    game->data = NULL;
    free(game);
    game = NULL;
}

static game_object_vtable_t square_wall_vtable =
{
    .draw = square_wall_draw,
    .type = square_wall_type,
    .overlaps = square_wall_overlaps,
    .destroy = square_wall_destroy
};


game_object_t * square_wall_create(unsigned int x1, unsigned int y1) {
    game_object_t * n = (game_object_t *) malloc(sizeof(*n));
    n->vtable = &square_wall_vtable;

    square_wall_object_data * d = (square_wall_object_data*) malloc(sizeof(*d));
    d->x = x1;
    d->y = y1;
    n->data = d;
    return n;
}

