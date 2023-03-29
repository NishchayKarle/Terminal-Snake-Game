#pragma once

#include "game_object.h"

typedef struct square_wall_object_data square_wall_object_data;

game_object_t * square_wall_create(unsigned int x1, unsigned int y1);

bool square_wall_overlaps_(game_object_t * game, 
    unsigned int x, unsigned int y);
