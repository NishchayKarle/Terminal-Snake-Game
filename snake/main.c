// Main program file for the game.
//
// !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
// !!! YOU SHOULD NOT MODIFY THIS FILE !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
// !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
//
#include "screen.h"
#include "util.h"

// Modify game_selection.h to switch from demo game to your snake game
#include "game_selection.h"
#include "game.h"

#include <stdlib.h>
#include <stdio.h>

#include <sys/types.h>
#include <unistd.h>

int main (int argc, char ** args)
{
    // Initialize random generator
    srand (getpid()+getppid());

    screen_t * scr = screen_init ();

    game_t * game = create_game(scr);

    // Main event loop
    //
    // - checks for key presses
    // - calls game_tick once every ms
    // - redraws the screen approx 60 times per second

    uint64_t next_frame = time_ms();
    const uint64_t frame_interval = 1000/80;
    uint64_t last_tick = 0;
    bool game_over = false;
    while (!game_over)
    {
        const uint64_t curtime = time_ms();

        if (last_tick != curtime)
        {
            game_over = !game_tick (game);
            last_tick = curtime;
        }

        // Check for key presses
        int key = getch();
        switch (key)
        {
            // Pass the following to the game
            case KEY_UP:
            case KEY_DOWN:
            case KEY_LEFT:
            case KEY_RIGHT:
                game_key(game, key);
                break;
                // terminate
            case 'Q':
            case 'q':
                game_over = true;
                break;
            default:
                break;
        }

        // Check if we should redraw
        if (curtime >= next_frame)
        {
            game_draw(game);
            next_frame += frame_interval;
        }
    }

    game_destroy (game);

    screen_done (scr);

    return EXIT_SUCCESS;
}
