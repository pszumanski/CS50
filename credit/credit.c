#include <cs50.h>
#include <stdio.h>
#include <stdbool.h>
// Checks if card is AMEX, VISA, MASTERCARD or INVALID by using Luhn's Algorithm

bool check(long number);
bool first(long number);


int main(void)
{
    // Gets the number
    long number;
    do
    {
        number = get_long("Number: ");
    }
    while (number < 999999999 || number > 9999999999999999);
    // Checks if such number could be valid
    if (check(number))
    {
        printf("INVALID\n");
    }
    // If it is then chooses
    else if (number / 10000000000000 == 34 || number / 10000000000000 == 37)
    {
        printf("AMEX\n");
    }
    else if (number / 100000000000000 > 50 && number / 100000000000000 < 56)
    {
        printf("MASTERCARD\n");
    }
    else if (first(number))
    {
        printf("VISA\n");
    }
    else
    {
        printf("INVALID\n");
    }
    //printf("%li", number / 10000000000000);

}

// Checks if number is valid
bool check(long number)
{
    long n;
    long odd = 0;
    int sum_odd = 0;
    int sum_even = 0;
    // Extracts odd numbers starting from second to last number
    for (long i = 100; (number * 10) / i > 1; i *= 100)
    {
        n = number % i - number % (i / 10);
        //printf("%li\n", n);
        odd += n;
    }
    //printf("%li", odd);
    odd *= 2;
    //printf("\n%li\n", odd);
    // Calculates sum of the products' digits
    for (long i = 10; (odd * 100) / i > 1; i *= 10)
    {
        n = (odd % i - odd % (i / 10)) / (i / 10);
        //printf("%li", n);
        sum_odd += n;
    }
    // Calculates sum of even digits
    //printf("\n");
    for (long i = 10; number * 10 / i > 1; i *= 100)
    {
        n = (number % i - number % (i / 10)) / (i / 10);
        //printf("%li", n);
        sum_even += n;
    }
    //printf("\n%i, %i\n", sum_odd, sum_even);
    if ((sum_even + sum_odd) % 10 == 0)
    {
        return false;
    }
    else
    {
        return true;
    }

}

// Checks if first number is 4
bool first(long number)
{
    long j;
    for (long i = 1000000000000; number * 10 / i > 1; i *= 10)
    {
        j = (number % i - number % (i / 10)) / (i / 10);
    }
    if (j == 4)
    {
        return true;
    }
    else
    {
        return false;
    }
}