#include <stdio.h>
#include <stdlib.h>

int main ( int argc, char *argv[] )
{
    int i, limit = 10;
    if (argc == 1)
    {
        
    }
    else if (argc == 2)
    {
        limit = atoi(argv[1]);
    }
    for (i = 0; i < limit; i++)
    {
        printf("%d\n", i);
        sleep(1);
    }
}