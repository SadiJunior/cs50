#include <cs50.h>
#include <stdio.h>
#include <string.h>

#define MAX_DIGIT_COUNT 20

typedef enum
{
    AmericanExpress = 0,
    MasterCard,
    Visa,
    Invalid,
} CardProvider;

CardProvider get_card_provider(long card_number);
bool is_valid_card_number(long card_number);
string long_to_string(long value);
string int_to_string(int value);
int digit_to_int(char character);
int sum_digits(string value);
bool array_contains(int *arr, int arr_size, int value);

int main(void)
{
    long card_number = get_long("Type the Credit Card Number: ");

    CardProvider card_provider = get_card_provider(card_number);

    switch (card_provider)
    {
        case AmericanExpress:
            printf("AMEX\n");
            break;
        case MasterCard:
            printf("MASTERCARD\n");
            break;
        case Visa:
            printf("VISA\n");
            break;
        case Invalid:
            printf("INVALID\n");
            break;
    }
}


// Gets the Card Provider from the given Credit Card number.
CardProvider get_card_provider(long card_number)
{
    if (!is_valid_card_number(card_number))
    {
        return Invalid;
    }

    string card_number_text = long_to_string(card_number);

    int digit_count = strlen(card_number_text);

    int first_digit = digit_to_int(card_number_text[0]);
    int second_digit = digit_to_int(card_number_text[1]);

    int first_two_digits = (first_digit * 10) + second_digit;

    int VISA_DIGIT_COUNTS[2] = {13, 16};
    int VISA_INITIAL_DIGITS[1] = {4};

    bool is_visa =
        array_contains(VISA_DIGIT_COUNTS, 2, digit_count) &&
        array_contains(VISA_INITIAL_DIGITS, 1, first_digit);

    if (is_visa)
    {
        return Visa;
    }

    int AMERICA_EXPRESS_DIGIT_COUNTS[1] = {15};
    int AMERICAN_EXPRESS_INITIAL_DIGITS[2] = {34, 37};

    bool is_american_express =
        array_contains(AMERICA_EXPRESS_DIGIT_COUNTS, 1, digit_count) &&
        array_contains(AMERICAN_EXPRESS_INITIAL_DIGITS, 2, first_two_digits);

    if (is_american_express)
    {
        return AmericanExpress;
    }

    int MASTERCARD_DIGIT_COUNTS[1] = {16};
    int MASTERCARD_INITIAL_DIGITS[5] = {51, 52, 53, 54, 55};

    bool is_mastercard =
        array_contains(MASTERCARD_DIGIT_COUNTS, 1, digit_count) &&
        array_contains(MASTERCARD_INITIAL_DIGITS, 5, first_two_digits);

    if (is_mastercard)
    {
        return MasterCard;
    }

    return Invalid;
}

// Verifies if the Card Number is valid, using Luhn's Algorithm.
bool is_valid_card_number(long card_number)
{
    string card_number_text = long_to_string(card_number);

    int card_number_length = strlen(card_number_text);

    int first_sum = 0;
    int second_sum = 0;

    for (int i = 1; i <= card_number_length; i++)
    {
        // There is a bug with the card_number_text string.
        // It was getting resetted in the middle of the for loop.
        // I suspect it is something related to the GC running in the int_to_string method, but I'm not sure.
        // As I could not find the bug, I've reaquired the string in every iteration.
        card_number_text = long_to_string(card_number);

        int reverse_index = card_number_length - i;
        int digit = digit_to_int(card_number_text[reverse_index]);

        if (i % 2 == 0)
        {
            int product = digit * 2;
            first_sum += sum_digits(int_to_string(product));
        }
        else
        {
            second_sum += digit;
        }
    }

    int total_sum = first_sum + second_sum;

    bool is_valid = total_sum % 10 == 0;

    return is_valid;
}

// Converts a long to a string.
string long_to_string(long value)
{
    char value_chars[MAX_DIGIT_COUNT];

    // Copies the card number to a array of chars (in other words, a string).
    sprintf(value_chars, "%ld", value);

    string value_text = value_chars;
    return value_text;
}

// Converts a int to a string.
string int_to_string(int value)
{
    char value_chars[MAX_DIGIT_COUNT];

    // Copies the card number to a array of chars (in other words, a string).
    sprintf(value_chars, "%d", value);

    string value_text = value_chars;
    return value_text;
}

// Gets the digit as an integer.
int digit_to_int(char character)
{
    // The character '0' in ASCII is 48.
    // The following characters are '1' = 49, '2' = 50, '3' = 51... until '9' = 57.
    // As the ASCII is just a binary, and this binary can also be integer, when subtracting
    // '0' (that is 48), you will end up with the binary of the number you want, therefore, converting it o a integer.
    return character - '0';
}

// Sums the digits from the given string.
int sum_digits(string value)
{
    int len = strlen(value);
    int sum = 0;

    for (int i = 0; i < len; i++)
    {
        sum += digit_to_int(value[i]);
    }

    return sum;
}

// Verifies if the given array contains the specified value.
bool array_contains(int *arr, int arr_size, int value)
{
    for (int i = 0; i < arr_size; i++)
    {
        if (arr[i] == value)
        {
            return true;
        }
    }

    return false;
}