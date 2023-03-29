#include "snake_object.h"
#include <stdlib.h>

struct snake_object_data {
    cfifo_handle snake;
    unsigned int direction;
    unsigned int length;
};

bool snake_iterate_draw(const coordinate_t * c, void * screen) {
    if (c == NULL || screen == NULL)
        return false;

    unsigned int x = c->x, y = c->y;
    screen_set((screen_t *)screen, x, y, SCREEN_CHAR_SNAKE);
    return true;
}

static void snake_draw(game_object_t * game, screen_t * screen)
{
    snake_object_data * d = (snake_object_data *) game->data;
    cfifo_visit(d->snake, &snake_iterate_draw, screen);
}

static const char * snake_type(game_object_t * game)
{
    return "snake";
}

bool snake_iterate_overlap(const coordinate_t * c, void * head) {
    if (c == NULL || head == NULL)
        return false;

    return (c->x == ((coordinate_t *)head)->x 
            && c->y == ((coordinate_t *)head)->y) ? false : true;
}

static bool snake_overlaps (game_object_t * game,
        unsigned int x, unsigned int y)
{
    snake_object_data * d = (snake_object_data *) game->data;
    coordinate_t * c = (coordinate_t *)malloc(sizeof(*c));
    c->x = x;
    c->y = y;
    return cfifo_visit(d->snake, &snake_iterate_overlap, c) == d->length+1;
}

bool snake_overlaps_(game_object_t * game, unsigned int x, 
    unsigned int y) {
            return snake_overlaps(game, x, y);
}

static void snake_destroy(game_object_t * game)
{
    snake_object_data * d = (snake_object_data *)game->data;
    cfifo_destroy(d->snake);
    free(d);
    d = NULL;
}

void snake_move(game_object_t * game, int direction) {
    snake_object_data * d = (snake_object_data *) game->data;
    coordinate_t * c = (coordinate_t *)malloc(sizeof(*c));
    const coordinate_t * head_c = (coordinate_t *)malloc(sizeof(*head_c));
    head_c = cfifo_head(d->snake);
    if (((direction == 'N' || direction == 'S') && (d->direction == 'N' || d->direction == 'S')) ||
        ((direction == 'E' || direction == 'W') && (d->direction == 'E' || d->direction == 'W')))
        return;

    if (direction == 'X')
        direction = d->direction;
    else
        d->direction = direction;

    if (direction == 'N') {
        *c = cfifo_pop(d->snake);
        c->x = head_c->x;
        c->y = head_c->y - 1;
        cfifo_push(d->snake, c);
    }

    else if (direction == 'W') {
        *c = cfifo_pop(d->snake);
        c->x = head_c->x-1;
        c->y = head_c->y;
        cfifo_push(d->snake, c);
    }

    else if (direction == 'S') {
        *c = cfifo_pop(d->snake);
        c->x = head_c->x;
        c->y = head_c->y + 1;
        cfifo_push(d->snake, c);
    }

    else {
        *c = cfifo_pop(d->snake);
        c->x = head_c->x+1;
        c->y = head_c->y;
        cfifo_push(d->snake, c);
    }
}

void snake_shift(game_object_t * game, unsigned int x, unsigned int y) {
    snake_object_data * d = (snake_object_data *) game->data;
    coordinate_t * c = (coordinate_t *)malloc(sizeof(*c));
    c->x = x;
    c->y = y;
    cfifo_pop(d->snake);
    cfifo_push(d->snake, c);
}

void snake_grow(game_object_t * game) {
    snake_object_data * d = (snake_object_data *) game->data;
    coordinate_t * c1 = (coordinate_t *)malloc(sizeof(*c1));

    const coordinate_t * c = cfifo_head(d->snake);
    c1->x = c->x;
    c1->y = c->y;
    cfifo_push(d->snake, c1);
}

const coordinate_t * snake_get_head_next(game_object_t * game) {
    snake_object_data * d = (snake_object_data *) game->data;
    const coordinate_t * c = cfifo_head(d->snake);
    coordinate_t * c1 = (coordinate_t *)malloc(sizeof(*c1));
    c1->x = c->x;
    c1->y = c->y;
    if (d->direction == 'N')
        c1->y--;
    else if (d->direction == 'S')
        c1->y++;
    else if (d->direction == 'W')
        c1->x--;
    else
        c1->x++;
    return c1;
}

const coordinate_t * snake_get_head(game_object_t * game) {
    snake_object_data * d = (snake_object_data *) game->data;
    const coordinate_t * c = cfifo_head(d->snake);
    coordinate_t * c1 = (coordinate_t *)malloc(sizeof(*c1));
    c1->x = c->x;
    c1->y = c->y;
    return c1;
}

static game_object_vtable_t snake_object_vtable =
{
    .draw = snake_draw,
    .type = snake_type,
    .overlaps = snake_overlaps,
    .destroy = snake_destroy
};

game_object_t * snake_object_create(unsigned int headx, unsigned int heady) {
    game_object_t * n = (game_object_t *) malloc(sizeof(*n));
    n->vtable = &snake_object_vtable;

    snake_object_data * d = (snake_object_data*) malloc(sizeof(*d));
    d->snake = cfifo_create();
    coordinate_t * c = (coordinate_t *)malloc(sizeof(*c));
    coordinate_t * c2 = (coordinate_t *)malloc(sizeof(*c2));
    coordinate_t * c3 = (coordinate_t *)malloc(sizeof(*c3));
    c->x = headx;
    c->y = heady;
    c2->x = headx+1;
    c2->y = heady;
    c3->x = headx+2;
    c3->y = heady;
    cfifo_push(d->snake, c);
    cfifo_push(d->snake, c2);
    cfifo_push(d->snake, c3);
    d->direction = 'E';
    d->length = 1;
    n->data = d;
    return n;
}