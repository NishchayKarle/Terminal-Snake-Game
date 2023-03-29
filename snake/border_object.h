#pragma once

// The object representing the border around the playing field.
//
// Use screen_box to draw it.

#include "game_object.h"


typedef struct border_object_data border_object_data;
game_object_t * border_object_create(unsigned int x1, unsigned int y1, 
    unsigned int x2, unsigned int y2);
bool border_object_overlaps_(game_object_t * game, unsigned int x, 
    unsigned y);