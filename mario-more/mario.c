#include <cs50.h>
#include <stdio.h>

void pyramid(int height);
void space(int height);
void hash(int height, int base);

int main(void)
{
    int i;
    // Asks for height
    do
    {
        i = get_int("Height: ");
    }
    while (i < 1 || i > 8);
    pyramid(i);
}

// Creates pyramid
void pyramid(int height)
{
    // Store base value of height
    int base = height;
    while (height > 0)
    {
        space(height);
        hash(height, base);
        printf("  ");
        hash(height, base);
        printf("\n");
        height--;
    }
}
// Creates empty spaces
void space(int height)
{
    for (int j = 1; j < height; j++)
    {
        printf(" ");
    }
}

// Creates building blocks
void hash(int height, int base)
{
    for (int j = base; j - height >= 0; j--)
    {
        printf("#");
    }
}