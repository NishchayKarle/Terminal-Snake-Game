#pragma once

#include "game_object.h"
#include "../cfifo/coordinate.h"
#include <stdbool.h>

typedef struct worm_hole_object_data worm_hole_object_data;

game_object_t * worm_hole_object_create(unsigned int x1, unsigned int y1,
    unsigned int x2, unsigned int y2);

bool worm_hole_object_overlaps_(game_object_t * game,
    unsigned int x, unsigned int y); 

const coordinate_t * worm_hole_exit(game_object_t * game, 
    unsigned int x, unsigned int y);