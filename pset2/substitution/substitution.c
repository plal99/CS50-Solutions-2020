#include <stdio.h>
#include <cs50.h>
#include <string.h>
#include <ctype.h>

int main(int argc, string argv[])
{
    if (argc != 2)
    {
        printf("Usage: ./substitution key\n");                                      // checks whether the input arguments consist of only one key
        return 1;
    }
    else
    {
        if (strlen(argv[1]) != 26)
        {
            printf("Key must contain 26 characters.\n");                                // checks for the number of charachters in the key
            return 1;
        }
        for (int k = 0; k < 26; k++)
        {
            if (isalpha(argv[1][k]))
            {
                continue;
            }
            else
            {
                printf("Some charachters in your key is not an alphabet.\n");           // cheks if the key has all the alphabet
                return 1;
            }
        }
        string alpha = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";                                        // initializing the alphabet for comparison
        int count = 0;
        for (int k = 0; k < 26; k++)
        {
            for (int l = 0; l < 26; l++)
            {
                if (toupper(argv[1][l]) == alpha[k])
                {
                    count++;                                                        // To check if duplicates are observed in the key
                    break;
                }
            }
        }
        //printf("%i",count);
        if (count != 26)
        {
            printf("Duplicates are observed\n");
            return 1;
        }
        string plaintext = get_string("plaintext: ");
        int n_plain = strlen(plaintext);
        int n_key = 26;
        for (int i = 0; i < n_plain; i++)
        {
            if (isupper(plaintext[i]))                                              // Case1 : Uppercase
            {
                for (int j = 0; j < n_key; j++)
                {
                    if (alpha[j] == plaintext[i])
                    {

                        plaintext[i] = toupper(argv[1][j]);
                        break;
                    }
                }
            }
            else if (islower(plaintext[i]))                                         // Case2 : Lowercase
            {
                for (int j = 0; j < n_key; j++)
                {
                    if (tolower(alpha[j]) == plaintext[i])
                    {

                        plaintext[i] = tolower(argv[1][j]);
                        break;
                    }
                }
            }
            else
            {
                continue;                                                       // when the plain text has spaces and other charachters in between the plain text
            }
        }
        printf("ciphertext: ");
        for (int i = 0; i < n_plain; i++)
        {
            printf("%c", plaintext[i]);                                                 // printing the encypted plain text
        }
        printf("\n");
    }

    return 0;
}