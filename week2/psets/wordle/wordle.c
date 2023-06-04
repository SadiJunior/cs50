#include <cs50.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>

// each of our text files contains 1000 words
#define LISTSIZE 1000

// values for colors and score (EXACT == right letter, right place; CLOSE == right letter, wrong place; WRONG == wrong letter)
#define EXACT 2
#define CLOSE 1
#define WRONG 0

// ANSI color codes for boxed in letters
#define GREEN   "\e[38;2;255;255;255;1m\e[48;2;106;170;100;1m"
#define YELLOW  "\e[38;2;255;255;255;1m\e[48;2;201;180;88;1m"
#define RED     "\e[38;2;255;255;255;1m\e[48;2;220;20;60;1m"
#define RESET   "\e[0;39m"

// user-defined function prototypes
string get_guess(int wordsize);
int check_word(string guess, int wordsize, int status[], string choice);
void print_word(string guess, int wordsize, int status[]);

int main(int argc, string argv[])
{
    if (argc != 2)
    {
        printf("Usage: ./wordle wordsize\n");
        return 1;
    }

    // Converts the wordsize from a string to an integer.
    int wordsize = 0;
    sscanf(argv[1], "%d", &wordsize);

    // Verifies if the size is supported.
    bool is_valid_wordsize =
        wordsize == 5 ||
        wordsize == 6 ||
        wordsize == 7 ||
        wordsize == 8;

    if (!is_valid_wordsize)
    {
        printf("Error: wordsize must be either 5, 6, 7, or 8");
        return 1;
    }

    // open correct file, each file has exactly LISTSIZE words
    char wl_filename[6];
    sprintf(wl_filename, "%i.txt", wordsize);
    FILE *wordlist = fopen(wl_filename, "r");
    if (wordlist == NULL)
    {
        printf("Error opening file %s.\n", wl_filename);
        return 1;
    }

    // load word file into an array of size LISTSIZE
    char options[LISTSIZE][wordsize + 1];

    for (int i = 0; i < LISTSIZE; i++)
    {
        fscanf(wordlist, "%s", options[i]);
    }

    // pseudorandomly select a word for this game
    srand(time(NULL));
    string choice = options[rand() % LISTSIZE];

    // allow one more guess than the length of the word
    int guesses = wordsize + 1;
    bool won = false;

    // print greeting, using ANSI color codes to demonstrate
    printf(GREEN"This is WORDLE50"RESET"\n");
    printf("You have %i tries to guess the %i-letter word I'm thinking of\n", guesses, wordsize);

    // main game loop, one iteration for each guess
    for (int i = 0; i < guesses; i++)
    {
        // obtain user's guess
        string guess = get_guess(wordsize);

        // array to hold guess status, initially set to zero
        int status[wordsize];

        for (int j = 0; j < wordsize; j++)
        {
            status[i] = WRONG;
        }

        // Calculate score for the guess
        int score = check_word(guess, wordsize, status, choice);

        printf("Guess %i: ", i + 1);

        // Print the guess
        print_word(guess, wordsize, status);

        // if they guessed it exactly right, set terminate loop
        if (score == EXACT * wordsize)
        {
            won = true;
            break;
        }
    }

    // Ends the game.
    if (won)
    {
        printf("You won!\n");
    }
    else
    {
        printf("You lost!\nThe word was: %s\n", choice);
    }

    // that's all folks!
    return 0;
}

// Gets the user guesses.
string get_guess(int wordsize)
{
    string guess = "";

    do
    {
        guess = get_string("Input a %i-letter word: ", wordsize);
    }
    while (strlen(guess) != wordsize);

    return guess;
}

// Gets the score of the given guess word.
int check_word(string guess, int wordsize, int status[], string choice)
{
    int score = 0;

    for (int i = 0; i < wordsize; i++)
    {
        char guess_char = guess[i];

        for (int j = 0; j < wordsize; j++)
        {
            char choice_char = choice[j];

            if (guess_char == choice_char)
            {
                if (i == j)
                {
                    status[i] = EXACT;
                }
                else
                {
                    status[i] = CLOSE;
                }

                score += status[i];
                break;
            }
        }
    }

    return score;
}

// Prints the given guess word, with the proper styling.
void print_word(string guess, int wordsize, int status[])
{
    for (int i = 0; i < wordsize; i++)
    {
        char guess_char = guess[i];
        int char_score = status[i];

        switch (char_score)
        {
            case EXACT:
                printf(GREEN"%c"RESET, guess_char);
                break;

            case CLOSE:
                printf(YELLOW"%c"RESET, guess_char);
                break;

            default:
                printf(RED"%c"RESET, guess_char);
                break;
        }
    }

    printf(RESET"\n");
    return;
}
