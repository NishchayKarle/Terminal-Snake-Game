#pragma once
// Object representing food for the snake

#include "game_object.h"

typedef struct food_object_data food_object_data;
game_object_t * food_object_create(unsigned int x1, unsigned int y1);
bool food_object_overlaps_(game_object_t * game, unsigned int x,
    unsigned int y);
void food_object_move(game_object_t * game, unsigned int x, 
    unsigned int y);