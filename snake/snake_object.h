#pragma once

#include "game_object.h"
#include "../cfifo/cfifo.h"

// *** NOTE: You will use the cfifo code to keep track of the positions the
// snake occupies, as well as where the head and tail of the snake is.


// snake_object_create:
// The pointer returned by this function can be cast to a game_object_t *
//
// Creates a snake of length 1 (with head at position headx, heady)
// and the specified intended length
//
// COMPLETE ME
typedef struct snake_object_data snake_object_data;

game_object_t * snake_object_create(unsigned int headx, unsigned int heady);
void snake_move(game_object_t * game, int direction);
void snake_shift(game_object_t * game, unsigned int x, unsigned int y);
void snake_grow(game_object_t * game);
bool snake_overlaps_(game_object_t * game, unsigned int x, 
    unsigned int y);
const coordinate_t * snake_get_head(game_object_t * game);
const coordinate_t * snake_get_head_next(game_object_t * game);