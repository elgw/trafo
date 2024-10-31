#include <stdio.h>
#include <stdlib.h>
#include <trafo.h>

int main(int argc, char ** argv)
{
    printf("trafo version: %d.%d.%d\n",
           TRAFO_VERSION_MAJOR,
           TRAFO_VERSION_MINOR,
           TRAFO_VERSION_PATCH);

    return EXIT_SUCCESS;
}
