// DO NOT CHANGE THIS FILE
//
// This is a very simple example which shows how everything fits together.
// Make your own version (snake_game.h and snake_game.c) based on this,
// and modify game_selection.h to instead return an instance of your 
// own game (snake_game).
//
#include "demo_game.h"
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

// Private data for our demo game.
// We only have a single wall on the screen and we increment
// our score every second.
// typedef struct demo_game_data_t
// {
//     screen_t * screen;
//     game_object_t * wall;
//     unsigned int score;
//     bool show_instructions;
//     bool game_over;

//     // The absolute time at which we earned another point!
//     uint64_t next_point;

//     // The number of points per second we are getting
//     double point_speed;
// } demo_game_data_t;

struct snake_game_data_t {
    screen_t * screen;

    game_object_t * snake;

    // game_object_t * border;
    // game_object_t * food;
    // game_object_t * worm_hole;
    // game_object_t * obst;

    vararray_handle objects;

    unsigned int score;

    bool show_instructions;
    bool game_over;

    uint64_t next_point;
    uint64_t next_grow_point;
    double point_speed;
};

typedef struct snake_game_data_t snake_game_data_t;


static void demo_game_destroy (game_t * g)
{
    snake_game_data_t * d = (snake_game_data_t *) g->data;
    free (d);
    free (g);
}

static void demo_game_key(game_t * g, int key)
{
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
static uint64_t next_point_time(snake_game_data_t * d)
{
    // current time in ms + 1000 ms (1s) / pointer per second
    return time_ms() + (500.0 / d->point_speed);
}

static uint64_t next_grow_point_time(snake_game_data_t * d)
{
    // current time in ms + 1000 ms (1s) / pointer per second
    return time_ms() + (3000.0); 
}

unsigned int get_random_number(unsigned int x, unsigned int y) {
    x += 5;
    y -= 5;
    unsigned int number = (rand() % (y - x + 1)) + x;
    return number;
}
static bool demo_game_tick (game_t * game)
{
    snake_game_data_t * d = (snake_game_data_t *) game->data;
    if (d->game_over)
    {
        screen_message(d->screen, "Game over!");
        return false;
    }

    // Check if level_start is true, if so,
    // display a message
    if (d->show_instructions)
    {
        d->show_instructions = false;
        screen_message(d->screen, "Earn a point every second and for "
                "every arrow key pressed but don't hit UP!");
    }

    const uint64_t curtime = time_ms();

    // Check if we earned a point
    // if (d->score >= 5) {
    //     screen_clear(d->screen);
    //     demo_game_draw(game);
    //     return true;
    // }

    if (curtime >= d->next_point)
    {

        // increase the rate at which we gain points by 20%
        d->point_speed *= 1.002;
        // Schedule our next point in a second! 
        d->next_point = next_point_time(d);
        const coordinate_t * c = snake_get_head_next(d->snake);
        // const coordinate_t * c1 = snake_get_head(d->snake);
        // const coordinate_t * c1 = snake_get_head(d->snake);
        snake_move(d->snake, 'X');
        if (snake_overlaps_(d->snake, c->x, c->y)) {
            screen_message(d->screen, "BLAH BLAH Game over!");
            return false;
        }

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
                    d->score++;
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
                if (square_wall_overlaps_(o, c->x, c->y)) {
                    d->game_over = true;
                    screen_message(d->screen, "Game over!");
                    return false;
                }
            }
        }

        // if (border_object_overlaps_(d->border, c->x, c->y)) {
        //     screen_message(d->screen, "Game over!");
        //     return false;
        // }

        // if (food_object_overlaps_(vararray_get(d->objects, 0), c->x, c->y) ) {
        //     game_object_t * o = (game_object_t *) vararray_get(d->objects, 0);
        //     assert(strcmp(o->vtable->type(o), "food_object") == 0);
        //     snake_grow(d->snake);
        //     d->score++;
        //     unsigned int x1 = get_random_number(0, screen_getmaxx(d->screen));
        //     unsigned int y1 = get_random_number(0, screen_getmaxy(d->screen));
        //     food_object_move(vararray_get(d->objects, 0), x1, y1);
        // }

        // if (worm_hole_object_overlaps_(d->worm_hole, c->x, c->y)) {
        //     const coordinate_t * exit = worm_hole_exit(d->worm_hole, c->x, c->y);
        //     snake_shift(d->snake, exit->x, exit->y);
        // }

        // if (square_wall_overlaps_(d->obst, c->x, c->y)) {
        //     screen_message(d->screen, "Game over!");
        //     return false;
        // }
    }
    if (curtime >= d->next_grow_point) {
        d->next_grow_point = next_grow_point_time(d);
        snake_grow(d->snake);
    }

    // and we keep playing...
    return true;
}

// 1) clear the screen
// 2) Draw all objects in the object vararray
// 3) update the score
// 4) refresh the screen
void demo_game_draw(game_t * game)
{
    snake_game_data_t * d = (snake_game_data_t *) game->data;
    screen_clear(d->screen);

    // draw our wall object
    game_object_draw(d->snake, d->screen);
    for (unsigned int i = 0; i < vararray_size(d->objects); i++) 
        game_object_draw(vararray_get(d->objects, i), d->screen);
    // game_object_draw(d->border, d->screen);
    // game_object_draw(vararray_get(d->objects, 0), d->screen);
    // game_object_draw(d->worm_hole, d->screen);
    // game_object_draw(d->obst, d->screen);

    // Update the score
    screen_update_score(d->screen, d->score, 0);
    screen_refresh(d->screen);
}

static game_vtable_t demo_game_vtable = {
    .key = demo_game_key,
    .tick = demo_game_tick,
    .draw = demo_game_draw,
    .destroy = demo_game_destroy
};

void vararray_test_destroy(void * a, void * b) {

}

game_t * create_demo_game(screen_t * screen)
{
    // assert((screen_getmaxx(screen) >= 50 && screen_getmaxy(screen) >= 50) == true);
    unsigned int screen_max_x = screen_getmaxx(screen);
    unsigned int screen_max_y = screen_getmaxy(screen);

    snake_game_data_t * d = (snake_game_data_t *) malloc(sizeof(*d));
    game_t * ret = (game_t *) malloc(sizeof(*ret));

    d->screen = screen;
    d->objects = vararray_create_with_destroy(&vararray_test_destroy, NULL);
    d->snake = snake_object_create(screen_max_x/2, screen_max_y/2);
    // vararray_push_back(d->objects, snake_object_create(screen_getmaxx(screen)/2, screen_getmaxy(screen)/2));

    unsigned int x1, y1, x2, y2;
    for (unsigned int i = 0; i < 5; i++) {
        x1 = get_random_number(0, screen_max_x);
        y1 = get_random_number(0, screen_max_y);
        // d->food = food_object_create(x1, y1);
        vararray_push_back(d->objects, food_object_create(x1, y1));
    }

    // d->border = border_object_create(0, 0, screen_getmaxx(screen), screen_getmaxy(screen));
    vararray_push_back(d->objects, 
        border_object_create(0, 0, screen_max_x, screen_max_y));

    for (unsigned int i = 0; i < 3; i++) {
        x1 = get_random_number(0, screen_max_x);
        y1 = get_random_number(0, screen_max_y);
        x2 = get_random_number(0, screen_max_x);
        y2 = get_random_number(0, screen_max_y);
        // // d->worm_hole = worm_hole_object_create(x1, y1, x2, y2);
        vararray_push_back(d->objects, worm_hole_object_create(x1, y1, x2, y2));
    }

    x1 = get_random_number(0, screen_max_x);
    y1 = get_random_number(0, screen_max_y);
    // d->obst = square_wall_create(x1, y1, 2);
    vararray_push_back(d->objects, square_wall_create(x1, y1, 2));

    d->score = 1;
    d->show_instructions = true;  // show help first
    d->game_over = false;

    // 1 second from now we earn a point
    d->point_speed = 2.5 * (screen_max_x * screen_max_y/ (80 * 80));   
    ret->vtable = &demo_game_vtable;
    ret->data = d;

    return ret;
}


