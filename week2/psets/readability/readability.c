#include <cs50.h>
#include <stdio.h>
#include <ctype.h>
#include <math.h>

int count_letters(string text);
int count_words(string text);
int count_sentences(string text);
bool is_end_of_sentence(char c);

int main(void)
{
    // Gets the text to be evaluated from the user.
    string text = get_string("Text: ");

    // Gets the text data.
    int letter_count = count_letters(text);
    int word_count = count_words(text);
    int sentence_count = count_sentences(text);

    // Gets L and S from Coleman-Liau Index.
    double l = ((float)letter_count / word_count) * 100.0;
    double s = ((float)sentence_count / word_count) * 100.0;

    // Performs Coleman-Liau Index.
    double coleman_liau_index = 0.0588 * l - 0.296 * s - 15.8;
    int grade = round(coleman_liau_index);

    // Prints grade.
    if (grade < 1)
    {
        printf("Before Grade 1\n");
    }
    else if (grade >= 16)
    {
        printf("Grade 16+\n");
    }
    else
    {
        printf("Grade %d\n", grade);
    }
}

// Gets the number of letters in the given text.
int count_letters(string text)
{
    int letter_count = 0;

    int n = 0;

    // Iterates and counts letters.
    while (text[n] != '\0')
    {
        if (isalpha(text[n]))
        {
            letter_count++;
        }

        n++;
    }

    return letter_count;
}

// Gets the number of words in the given text.
int count_words(string text)
{
    int word_count = 0;

    int n = -1;
    bool has_word = false;

    // Iterates and counts word.
    // Must be a do while because the last word can end with a '\0', so it must iterate until the last char.
    do
    {
        n++;

        char c = text[n];

        if (isalpha(c))
        {
            has_word = true;
        }
        else if ((isspace(c) || c == '\0' || c == '\n') && has_word)
        {
            word_count++;
            has_word = false;
        }
    }
    while (text[n] != '\0');

    return word_count;
}

// Gets the number of sentences in the given text.
int count_sentences(string text)
{
    int sentence_count = 0;

    int n = 0;

    // Iterates and counts sentences.
    while (text[n] != '\0')
    {
        if (is_end_of_sentence(text[n]))
        {
            sentence_count++;
        }

        n++;
    }

    return sentence_count;
}

// Verifies if the given char is an end of sentence char.
bool is_end_of_sentence(char c)
{
    if (c == '.' || c == '!' || c == '?')
    {
        return true;
    }
    else
    {
        return false;
    }
}