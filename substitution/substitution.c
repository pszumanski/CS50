#include <cs50.h>
#include <ctype.h>
#include <stdio.h>
#include <string.h>

string cipher(string plaintext, string key);
string s;

int main(int argc, string argv[])
{
    // Checks if 1 command given
    if (argc != 2)
    {
        printf("Usage: ./substitution key\n");
        return 1;
    }
    // Checks if key is of a set length
    else if (strlen(argv[1]) != 26)
    {
        printf("Key must contain 26 characters\n");
        return 1;
    }
    else
    {
        // Checks if key is made of letters only
        for (int i = 0; i < 26; i++)
        {
            if (!isalpha(argv[1][i]))
            {
                printf("Key must contain letters only\n");
                return 1;
            }
        }
        // Converts all letters to lowercase
        for (int i = 0; i < 26; i++)
        {
            if (isupper(argv[1][i]))
            {
                argv[1][i] += 32;
            }
        }
        // Checks if each letter is unique
        for (int i = 0; i < 26; i++)
        {
            int j = 25 - i;
            while (j > 0)
            {
                if (argv[1][i] == argv[1][i + j])
                {
                    printf("Each character must be unique\n");
                    return 1;
                }
                j--;
            }
        }
        // Gets string from user
        s = get_string("plaintext: ");
        printf("ciphertext: %s", cipher(s, argv[1]));
        printf("\n");
        return 0;

    }

}

// Uses text given and key to cipher the text
string cipher(string plaintext, string key)
{
    int length = strlen(plaintext);
    for (int i = 0; i < length; i++)
    {
        if (isalpha(plaintext[i]))
        {
            if (isupper(plaintext[i]))
            {
                plaintext[i] = key[plaintext[i] - 65] - 32;
            }
            else
            {
                plaintext[i] = key[plaintext[i] - 97];
            }
        }

    }
    return plaintext;
}