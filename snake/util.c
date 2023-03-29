// This is to get access to the non-portable (or at least not defined by the
// C99 standard) nanosleep function
#define _POSIX_C_SOURCE 199309L
#include <time.h>

#include "util.h"

#include <sys/time.h>

uint64_t time_ms()
{
    struct timeval v;
    gettimeofday(&v, NULL);
    return v.tv_sec * 1000 + (v.tv_usec/1000);
}

void sleep_for_ms(uint64_t s)
{
    struct timespec ts;
    ts.tv_sec = s/1000;
    ts.tv_nsec = (s % 1000) * 1000000;
    nanosleep(&ts, NULL);
}

void sleep_until(uint64_t s)
{
    uint64_t now = time_ms();
    if (s <= now)
        return;
    sleep_for_ms(s-now);
}
