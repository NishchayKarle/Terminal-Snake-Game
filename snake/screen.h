#pragma once

#include <ncurses.h>
#include <stdbool.h>

typedef int screen_char_t;

#define SCREEN_CHAR_SNAKE ACS_BLOCK
#define SCREEN_CHAR_FOOD ACS_DIAMOND
#define SCREEN_CHAR_WALL ACS_CKBOARD
#define SCREEN_CHAR_BULLET ACS_BULLET

struct screen_t;

typedef struct screen_t screen_t;

/// Show a message on the screen, user needs to press a key to continue
/// (and function doesn't return until a key has been pressed)
void screen_message (screen_t * screen, const char * s);

/// Update the score on the screen
void screen_update_score (screen_t * s,
                          unsigned int score, unsigned int highscore);

/// Create the screen area
/// Also clears the screen (see screen_clear)
///
/// The screen is made up of the playing field, and a information(scoreboard)
/// area.
///
/// The playing field has coordinates (0,0) for the upper left corner,
/// and (getmaxx()-1, getmaxy()-1) for the lower right corner.
///
screen_t * screen_init ();

/// Clear the playing field.
/// (i.e. remove any traces of the snake, message or other content)
///
/// Does not affect the scoreboard.
/// Don't forget to call screen_refresh after you are done making changes
/// to the screen.
void screen_clear (screen_t * scr);

/// Draw a character on the playing field at the specified location
void screen_set (screen_t * scr,
                 unsigned int x, unsigned int y, screen_char_t c);

/// Return the character at the specified location in the playing field.
/// Note: empty space is made of ' '
///
/// YOU SHOULD NOT HAVE TO USE THIS FUNCTION
//screen_char_t screen_get (const screen_t * scr,
//                          unsigned int x, unsigned int y);

/// Return the number of horizontal characters for the playing field
unsigned int screen_getmaxx (const screen_t * scr);

// Draw a box(frame) with lines
void screen_box (screen_t * scr, unsigned int x1, unsigned int y1,
        unsigned int x2, unsigned int y2);

/// Return the number of vertical characters for the playing field
unsigned int screen_getmaxy (const screen_t * scr);

/// Called when the screen can be destroyed.
void screen_done (screen_t * scr);

/// This function needs to be called after changing the screen
/// (adding characters, clearing the screen, ...)
///
/// (This is an optimization to allow making all changes to the screen in
///  one operation)
void screen_refresh (screen_t * scr);

/// In case you want to go beyond the simple functions provided above,
/// use this function to get direct access to the underlying (*in this
/// implementation* ncurses object corresponding to the playing field)
WINDOW * screen_get_mainwin (screen_t * scr);

