// Check that a password has at least one lowercase letter, uppercase letter, number and symbol
// Practice iterating through a string
// Practice using the ctype library

#include <cs50.h>
#include <stdio.h>
#include <string.h>

bool valid(string password);

int main(void)
{
    string password = get_string("Enter your password: ");
    if (valid(password))
    {
        printf("Your password is valid!\n");
    }
    else
    {
        printf("Your password needs at least one uppercase letter, lowercase letter, number and symbol\n");
    }
}

// TODO: Complete the Boolean function below
bool valid(string password)
{
    int length = strlen(password);
    bool lowercase, uppercase, number, symbol = false;
    for (int i = 0; i < length; i++)
    {
        if (password[i] > 96 && password[i] < 123)
        {
            lowercase = true;
        }
        else if (password[i] > 64 && password[i] < 91)
        {
            uppercase = true;
        }
        else if (password[i] >= '0' && password[i] <= '9')
        {
            number = true;
        }
        else if (password[i] > 32 && password[i] < 48)
        {
            symbol = true;
        }
    }
    if (lowercase == true && uppercase == true && number == true && symbol == true)
    {
        return true;
    }
    return false;
}
