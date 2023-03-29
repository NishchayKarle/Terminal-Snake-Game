#include "vararray.h"

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>

static char * mystrdup(const char * s)
{
    char * ret = malloc(strlen(s) + 1);
    strcpy(ret, s);
    printf("Allocating new string: '%s' at address %p\n",
            ret, ret);
    return ret;
}

static void string_cleanup(void * element, void * user)
{
    // Check that the value passed is correct
    assert(user == (void *) 1234);

    printf("Freeing pointer %p\n", element);
    free(element);
}

static bool write_to_file(unsigned int idx, void * element,
        void * visitdata)
{
    FILE * output = (FILE *) visitdata;

    fprintf(output, "Visiting array: index=%u element=%p ('%s')\n",
            idx, element, (const char *) element);

    // Return true so we continue to visit the array
    return true;
}

int main (int argc, char ** args)
{
    // we're not using the user void * pointer here; Passing 1234
    // just to demonstrate how the value is passed to string_cleanup
    vararray_handle va = vararray_create_with_destroy(string_cleanup,
            (void*) 1234);

    for (int i=1; i<argc; ++i)
    {
        printf("Inserting element %p ('%s')...\n", args[i],
                (const char *) args[i]);
        vararray_push_back(va, mystrdup(args[i]));
    }

    printf("The size of the vararray is now: %u\n",
            vararray_size(va));

    printf("Visiting the vararray:\n");

    // We're passing FILE * (stdout) as the void * visitdata argument
    // since we want write_to_file to write to stdout in this case.
    vararray_visit(va, write_to_file, (void *) stdout);

    puts("About to call vararray_destroy");

    // NOTE: no memory leak since vararray_destroy will call string_cleanup
    // for each element in the array
    vararray_destroy(va);

    return EXIT_SUCCESS;
}
