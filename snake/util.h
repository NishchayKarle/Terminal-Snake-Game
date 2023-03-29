#pragma once

#include <stdint.h>

// General utility functions

// The time in ms since the UNIX epoch (starting point not important).
// You can use this to measure time intervals
// (by calling this function at the start and the end of the interval
// and subtracting the two)
uint64_t time_ms();


// Sleep for the specified number of milliseconds
//   Implemented as sleep_until(ms_time() + s)
void sleep_for_ms(uint64_t s);

// Sleep until the specified time (in ms)
void sleep_until(uint64_t s);

