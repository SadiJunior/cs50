from cs50 import get_string
from enum import Enum


# Enumeration of Card Providers
class CardProvider(Enum):
    Invalid = 0
    AmericanExpress = 1
    MasterCard = 2
    Visa = 3


# Main function
def main():
    card_number = get_string("Type the Credit Card Number: ")

    card_provider = get_card_provider(card_number)

    match card_provider:
        case CardProvider.AmericanExpress:
            print("AMEX")
        case CardProvider.MasterCard:
            print("MASTERCARD")
        case CardProvider.Visa:
            print("VISA")
        case CardProvider.Invalid:
            print("INVALID")


# Gets the Card Provider from the given Credit Card number.
def get_card_provider(card_number: str) -> CardProvider:
    # Verifies if the Card Number is valid, using Luhn's Algorithm.
    if not is_valid_card_number(card_number):
        return CardProvider.Invalid

    digit_count = len(card_number)

    # Verifies if it is VISA
    VISA_DIGIT_COUNTS = [13, 16]
    VISA_INITIAL_DIGITS = [4]
    is_visa = digit_count in VISA_DIGIT_COUNTS and int(card_number[0]) in VISA_INITIAL_DIGITS

    if is_visa:
        return CardProvider.Visa

    # Verifies if it is AMEX
    AMERICA_EXPRESS_DIGIT_COUNTS = [15]
    AMERICAN_EXPRESS_INITIAL_DIGITS = [34, 37]
    is_american_express = digit_count in AMERICA_EXPRESS_DIGIT_COUNTS and int(card_number[0:2]) in AMERICAN_EXPRESS_INITIAL_DIGITS

    if is_american_express:
        return CardProvider.AmericanExpress

    # Verifies if it is MASTERCARD
    MASTERCARD_DIGIT_COUNTS = [16]
    MASTERCARD_INITIAL_DIGITS = [51, 52, 53, 54, 55]
    is_mastercard = digit_count in MASTERCARD_DIGIT_COUNTS and int(card_number[0:2]) in MASTERCARD_INITIAL_DIGITS

    if is_mastercard:
        return CardProvider.MasterCard

    return CardProvider.Invalid


# Verifies if the Card Number is valid, using Luhn's Algorithm.
def is_valid_card_number(card_number: str) -> bool:
    reverse_card_number = card_number[::-1]

    first_sum = 0
    second_sum = 0

    for i in range(1, len(card_number) + 1):
        digit = int(reverse_card_number[i - 1])
        if i % 2 == 0:
            product = digit * 2
            first_sum += sum([int(char) for char in str(product)])
        else:
            second_sum += digit

    total_sum = first_sum + second_sum
    is_valid = total_sum % 10 == 0

    return is_valid


# Entry point
main()