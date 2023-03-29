#pragma once

#include <stdbool.h>

typedef struct game_t game_t;

typedef struct
{
    /// Called when a key was pressed; The following keys are recognized:
    ///  
    ///   KEY_UP
    ///   KEY_LEFT
    ///   KEY_RIGHT
    ///   KEY_DOWN
    ///
    /// Return 0 if delay before calling game_tick again should remain unmodified
    /// If non-zero, the return
    /// value is taken as the delay before game_tick should be called again
    /// (ignoring any remaining delay that remained before the key was pressed)
    ///
    /// Using the return value from this function and the return value
    /// from game_tick() you can control how long to sleep (i.e. when nothing
    /// needs to happen in the game).
    void (*key) (game_t * g, int key);

    /// Called at regular intervals;
    /// The function should return true if the game is still continuing,
    /// otherwise false (in which case game_destroy will be called).
    bool (*tick) (game_t * g);


    // Draw all objects on the screen and call refresh
    void (*draw) (game_t * g);

    /// Called when the game is over. Free any internal state here.
    /// as well as freeing g
    void (*destroy) (game_t * g);

} game_vtable_t;


struct game_t
{
    game_vtable_t * vtable;
    void * data;
};


static inline void game_key(game_t * g, int key)
{
    g->vtable->key(g, key);
}

static inline void game_draw(game_t * g)
{
    g->vtable->draw(g);
}

static inline bool game_tick(game_t * g)
{
    return g->vtable->tick(g);
}

static inline void game_destroy(game_t * g)
{
    g->vtable->destroy(g);
}
