#include "cfifo.h"

#include <stdlib.h>
#include <stdio.h>
#include <assert.h>

static bool visit(const coordinate_t * c, void * userdata)
{
    printf("x=%i, y=%i\n", c->x, c->y);
    return (bool) userdata;
}

int main(int argc, char ** args)
{
    cfifo_handle h = cfifo_create();

    bool b = cfifo_empty(h);
    // b is true
    assert(b);

    coordinate_t c;
    c.x = 1;
    c.y = 2;

    cfifo_push(h, &c);

    c.x = 2;
    c.y = 3;

    cfifo_push(h, &c);

    b = cfifo_empty(h);
    assert(!b);

    unsigned int s = cfifo_size(h);
    assert(s == 2);

    const coordinate_t * cp = cfifo_head(h);
    assert(cp->x == 2);
    assert(cp->y == 3);

    printf("First visit:\n");
    // will output x=1, y=2
    unsigned int count = cfifo_visit(h, visit, (void*) 0);
    assert(count == 1);

    printf("Second visit:\n");
    // will output:
    //   x=1, y=2
    //   x=2, y=3
    count = cfifo_visit(h, visit, (void*) 1);
    assert(count == 2);

    cfifo_destroy(h);

    return EXIT_SUCCESS;
}
