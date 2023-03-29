#include "snake_game.h"
#include "util.h"

// Our objects
#include "square_wall_object.h"
#include "snake_object.h"
#include "border_object.h"
#include "food_object.h"
#include "../vararray/vararray.h"
#include "worm_hole_object.h"

#include <assert.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

struct snake_game_data_t {
    screen_t * screen;

    game_object_t * snake;

    vararray_handle objects;

    unsigned int score;

    bool show_instructions;
    bool game_over;

    uint64_t next_point;
    uint64_t next_grow_point;
    double point_speed;
};

typedef struct snake_game_data_t snake_game_data_t;

void vararray_test_destroy(void * element, void * user) {
    game_object_t * o = (game_object_t *)element;
    o->vtable->destroy(o);
}

static void snake_game_destroy (game_t * g) {
    snake_game_data_t * d = (snake_game_data_t *) g->data;
    d->snake->vtable->destroy(d->snake);
    free(d->snake);
    d->snake = NULL;
    vararray_destroy(d->objects);
    d->objects = NULL;
    free (d);
    d = NULL;
    free (g);
}

static void snake_game_key(game_t * g, int key) {
    snake_game_data_t * d = (snake_game_data_t *) g->data;

    switch (key)
    {
        case KEY_UP:
            snake_move(d->snake, 'N');
            break;
        case KEY_DOWN:
            snake_move(d->snake, 'S');
            break;
        case KEY_LEFT:
            snake_move(d->snake, 'W');
            break;
        case KEY_RIGHT:
            snake_move(d->snake, 'E');
            break;
        default:
            break;
    }
}

// Uses the current point_speed to calculate when the next point
// should be earned
static uint64_t next_point_time(snake_game_data_t * d) {
    // current time in ms + 1000 ms (1s) / pointer per second
    return time_ms() + (500.0 / d->point_speed);
}

static uint64_t next_grow_point_time(snake_game_data_t * d) {
    return time_ms() + (3000.0); 
}

unsigned int get_random_number(unsigned int x, unsigned int y) {
    x += 5;
    y -= 5;
    unsigned int number = (rand() % (y - x + 1)) + x;
    return number;
}
static bool snake_game_tick (game_t * game) {
    snake_game_data_t * d = (snake_game_data_t *) game->data;
    if (d->game_over) {
        screen_message(d->screen, "Game over!");
        return false;
    }

    if (d->show_instructions) {
        d->show_instructions = false;
        screen_message(d->screen, "Press Space to start. Press q to quit.");
    }

    const uint64_t curtime = time_ms();

    if (curtime >= d->next_point) {

        // increase the rate at which we gain points by 20%
        d->point_speed *= 1.002;
        d->next_point = next_point_time(d);
        const coordinate_t * c = snake_get_head_next(d->snake);
        const coordinate_t * c1 = snake_get_head(d->snake);
        snake_move(d->snake, 'X');
        if (snake_overlaps_(d->snake, c->x, c->y)) {
            screen_message(d->screen, "Game over!");
            return false;
        }

        // check for collisions of snake with objects in vararray
        for (unsigned int i = 0; i < vararray_size(d->objects); i++) {
            game_object_t * o = (game_object_t *) vararray_get(d->objects, i);
            if (strcmp(o->vtable->type(o), "border_object") == 0) {
                if (border_object_overlaps_(o, c->x, c->y)) {
                    d->game_over = true;
                    screen_message(d->screen, "Game over!");
                    return false;
                }
            }
            else if (strcmp(o->vtable->type(o), "food_object") == 0) {
                if (food_object_overlaps_(o, c->x, c->y)) {
                    snake_grow(d->snake);
                    d->score += 5;
                    unsigned int x1 = get_random_number(0, screen_getmaxx(d->screen));
                    unsigned int y1 = get_random_number(0, screen_getmaxy(d->screen));
                    food_object_move(o, x1, y1);
                }
            }

            else if (strcmp(o->vtable->type(o), "worm_hole_object") == 0) {
                if (worm_hole_object_overlaps_(o, c->x, c->y)) {
                    const coordinate_t * exit = worm_hole_exit(o, c->x, c->y);
                    snake_shift(d->snake, exit->x, exit->y);
                }
            }

            else if (strcmp(o->vtable->type(o), "square_wall_object") == 0) {
                if (square_wall_overlaps_(o, c1->x, c1->y)) {
                    d->game_over = true;
                    screen_message(d->screen, "Game over!");
                    return false;
                }
            }
        }

    }

    if (curtime >= d->next_grow_point) {
        d->next_grow_point = next_grow_point_time(d);
        snake_grow(d->snake);
        d->score++;
    }

    return true;
}

// 1) clear the screen
// 2) Draw all objects in the object vararray
// 3) update the score
// 4) refresh the screen
void snake_game_draw(game_t * game) {
    snake_game_data_t * d = (snake_game_data_t *) game->data;
    screen_clear(d->screen);

    // draw objects
    game_object_draw(d->snake, d->screen);
    for (unsigned int i = 0; i < vararray_size(d->objects); i++) 
        game_object_draw(vararray_get(d->objects, i), d->screen);

    // Update the score
    screen_update_score(d->screen, d->score, 0);
    screen_refresh(d->screen);
}

static game_vtable_t demo_game_vtable = {
    .key = snake_game_key,
    .tick = snake_game_tick,
    .draw = snake_game_draw,
    .destroy = snake_game_destroy
};


game_t * create_snake_game(screen_t * screen) {
    // assert((screen_getmaxx(screen) >= 50 && screen_getmaxy(screen) >= 50) == true);
    unsigned int screen_max_x = screen_getmaxx(screen);
    unsigned int screen_max_y = screen_getmaxy(screen);

    snake_game_data_t * d = (snake_game_data_t *) malloc(sizeof(*d));
    game_t * ret = (game_t *) malloc(sizeof(*ret));

    d->screen = screen;
    d->objects = vararray_create_with_destroy(&vararray_test_destroy, NULL);
    d->snake = snake_object_create(screen_max_x/2, screen_max_y/2);

    unsigned int x1, y1;
    for (unsigned int i = 0; i < 3; i++) {
        x1 = get_random_number(0, screen_max_x);
        y1 = get_random_number(0, screen_max_y);
        vararray_push_back(d->objects, food_object_create(x1, y1));
    }
    vararray_push_back(d->objects, food_object_create(screen_max_x/4, 12));
    vararray_push_back(d->objects, food_object_create(3*screen_max_x/4, 13));

    vararray_push_back(d->objects, 
        border_object_create(0, 0, screen_max_x, screen_max_y));

    x1 = get_random_number(0, screen_max_x);
    y1 = get_random_number(0, screen_max_y);
    vararray_push_back(d->objects, worm_hole_object_create(screen_max_x/2, 11, screen_max_x/4, screen_max_y/2 + 5));
    vararray_push_back(d->objects, worm_hole_object_create(screen_max_x/8, 3, 3*screen_max_x/4, screen_max_y-2));
    // x2 = get_random_number(0, screen_max_x);
    // y2 = get_random_number(0, screen_max_y);

    x1 = get_random_number(0, screen_max_x);
    y1 = get_random_number(0, screen_max_y);
    vararray_push_back(d->objects, square_wall_create(2, 5));
    vararray_push_back(d->objects, square_wall_create(2, 16));

    d->score = 1;
    d->show_instructions = true;  // show help first
    d->game_over = false;

    d->point_speed = 2.5 * (screen_max_x * screen_max_y/ (80 * 80));   
    ret->vtable = &demo_game_vtable;
    ret->data = d;

    return ret;
}