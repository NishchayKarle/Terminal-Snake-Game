#pragma once

// The interface for game objects.
// You are allowed to change this by *adding* new methods to the table.

#include "screen.h"

struct game_object_t;
typedef struct game_object_t game_object_t;

typedef struct game_object_vtable_t
{
    // Called when the object should be drawn on the screen
    void (*draw) (game_object_t * o, screen_t * screen);

    // Returns a pointer to a string (which will not be freed by the called)
    // indicating the type of object this is.
    // Used for debugging only. (Though it can be used to have the snake
    // react differently when touching this object for example)
    const char * (*type) (game_object_t * o);

    // Returns true if this object occupies the specified position
    bool (*overlaps) (game_object_t * o, unsigned int x, unsigned int y);

    // Called when the object is destroyed.
    void (*destroy) (game_object_t * o);


} game_object_vtable_t;

typedef struct game_object_t
{
    game_object_vtable_t * vtable;
    void * data;
} game_object_t;

static inline void game_object_destroy(game_object_t * o)
{
    // note that the object's destroy function *also* frees o
    o->vtable->destroy(o);
}

static inline bool game_object_overlaps(game_object_t * o, unsigned int x, unsigned int y)
{
    return o->vtable->overlaps(o, x, y);
}

static inline const char * game_object_type(game_object_t * o)
{
    return o->vtable->type(o);
}

static inline void game_object_draw(game_object_t * o, screen_t * s)
{
    o->vtable->draw(o, s);
}

