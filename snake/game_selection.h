#pragma once

#include "screen.h"
#include "game.h"
#include "snake_game.h"


static inline game_t * create_game(screen_t * screen) {
    return create_snake_game(screen);
}
