#include <cs50.h>
#include <stdio.h>

int main(void)
{
    // TODO: Prompt for start size
    int size;
    do
    {
        size = get_int("Start size: ");
    }
    while (size < 9);
    // TODO: Prompt for end size
    int size_end;
    do
    {
        size_end = get_int("End size: ");
    }
    while (size_end < size);

    // TODO: Calculate number of years until we reach threshold
    int n;
    for (n = 0; size < size_end; n++)
    {
        size = size + size / 3 - size / 4;
    }
    // TODO: Print number of years
    printf("Years: %i\n", n);
}
