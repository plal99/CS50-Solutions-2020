#include <stdio.h>
#include <cs50.h>                                                   // header files
#include <math.h>

int sum1(long long num, char order);

int main()
{
    int total, count1 = 0;
    long long num = get_long_long("Number :"), temp1, x, y, z;                      // input
    temp1 = num;
    total = sum1(num, 't');
    total += sum1(num, 'o');
    while (temp1 > 0)
    {
        temp1 /= 10;
        count1++;
    }
    x = floor(pow(10, count1));
    y = floor(pow(10, count1 - 1));
    z = floor(pow(10, count1 - 2));
    if (total % 10 == 0)                                                    // to know the card 
    {
        if (count1 == 15 && (num % x) / y == 3)
        {
            if ((num % y) / z == 4 || (num % y) / z == 7)
            {
                printf("AMEX\n");                                       // amex
            }
            else
            {
                printf("INVALID\n");
            }
        }
        else if ((num % x) / y == 4 && count1 >= 13)
        {
            printf("VISA\n");                                           // visa
        }
        else if ((num % x) / y == 5)
        {
            if ((num % y) / z == 1 || (num % y) / z == 2 || (num % y) / z == 3 || (num % y) / z == 4 || (num % y) / z == 5)
            {
                printf("MASTERCARD\n");                             // mastercard
            }
            else
            {
                printf("INVALID\n");
            }
        }
        else
        {
            printf("INVALID\n");
        }
    }
    else
    {
        printf("INVALID\n");
    }

    return 0;
}

int sum1(long long num, char order)                                  // function to calculate the checksum
{
    int sum = 0, a, i;
    long long a2, a1, count = 0;
    long long temp = num;
    while (temp > 0)
    {
        temp /= 10;
        count++;                                                        // check number of digits
    }

    if (order == 't')                                                   // taking th sum of digits in even places
    {
        i = 2;
        while (i <= count)
        {
            a2 = floor(pow(10, i));
            a1 = floor(pow(10, i - 1));
            a = floor((num % a2) / a1);
            if (((a * 2) % 100) / 10 > 0)
            {
                a = ((a * 2) % 100) / 10 + (a * 2) % 10;                    // calculations
                sum += a;
                i += 2;
                continue;
            }
            sum += a * 2;
            i += 2;
        }
    }
    if (order == 'o')                                           // taking the sum of digits in odd places
    {
        i = 1;
        while (i <= count)
        {
            a2 = floor(pow(10, i));
            a1 = floor(pow(10, i - 1));
            a = floor((num % a2) / a1);
            sum += a;
            i += 2;
        }
    }

    return sum;
}