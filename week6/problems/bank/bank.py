import re

greeting = input("Greeting: ").lower().strip()

word_re = r"[^\w]"
first_word = re.split(word_re, greeting)[0]

tax: float

if first_word == "hello":
    tax = 0
elif first_word[0] == "h":
    tax = 20
else:
    tax = 100

print(f"${tax}")