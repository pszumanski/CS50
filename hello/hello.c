#include <cs50.h>
#include <stdio.h>

int main(void)
{
    //says hello to the user
    string name = get_string("What's your name? ");
    printf("hello, %s\n", name);
}