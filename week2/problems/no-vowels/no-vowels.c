// Write a function to replace vowels with numbers
// Get practice with strings
// Get practice with command line
// Get practice with switch

#include <cs50.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

string replace(string text);

int main(int argc, string argv[])
{
    if (argc <= 1)
    {
        printf("Usage: ./no-vowels word\n");

        return 1;
    }

    string replaced_text = replace(argv[1]);

    printf("%s\n", replaced_text);

    free(replaced_text);
}

// Replaces the vowels.
string replace(string text)
{
    int text_len = strlen(text);

    string replaced_chars = malloc((text_len + 1) * sizeof(char));
    strcpy(replaced_chars, text);

    for (int i = 0; i < text_len; i++)
    {
        char ch = text[i];

        // Replaces the chars.
        // OBS: 'u' does not get replaced.
        switch (ch)
        {
            case 'a':
                replaced_chars[i] = '6';
                break;

            case 'e':
                replaced_chars[i] = '3';
                break;

            case 'i':
                replaced_chars[i] = '1';
                break;

            case 'o':
                replaced_chars[i] = '0';
                break;

            default:
                replaced_chars[i] = ch;
                break;
        }
    }

    // Marks the end of the string with '\0'.
    replaced_chars[text_len + 1] = '\0';

    return replaced_chars;
}