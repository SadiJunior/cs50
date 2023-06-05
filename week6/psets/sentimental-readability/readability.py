from cs50 import get_string
import re


# Main function
def main():
    text = get_string("Text: ").lower()

    # Gets the count of the list of all alphanumeric chars
    letters = [char for char in text if char.isalnum()]
    letter_count = len(letters)

    # Applies a regex for getting all words and counts it
    words = [word for word in re.split(r"[ \n\0]", text) if len(word) > 0]
    word_count = len(words)

    # Applies a regex for getting all sentences and counts it
    sentences_enders = re.findall(r"[.!?]+", text)
    sentence_count = len(sentences_enders)

    l = (letter_count / word_count) * 100.0
    s = (sentence_count / word_count) * 100.0

    # Performs Coleman-Liau Index
    coleman_liau_index = 0.0588 * l - 0.296 * s - 15.8
    grade = round(coleman_liau_index)

    if grade < 1:
        print("Before Grade 1")
    elif grade >= 16:
        print("Grade 16+")
    else:
        print(f"Grade {grade}")


# Entry point
main()