import sys
import argparse
from pyfiglet import Figlet
from random import randint

def main():
    parser = argparse.ArgumentParser()
    parser.add_argument("-f", "--format", help="set the format")

    try:
        args = parser.parse_args()
    except SystemExit:
        print("Invalid usage")
        sys.exit(1)

    figlet = Figlet()

    figlet_fonts = figlet.getFonts()

    if args.format is None:
        figlet_font = figlet_fonts[randint(0, len(figlet_fonts) - 1)]
    else:
        figlet_font = args.format

    if figlet_font not in figlet_fonts:
        print("Invalid usage")
        sys.exit(1)

    text = input("Input: ")

    figlet.setFont(font=figlet_font)
    print(figlet.renderText(text))

    sys.exit(0)

main()