from cs50 import get_int


def main():
    height = 0

    while height <= 0 or height > 8:
        height = get_int("Height: ")

    print_tower(height)


# Prints the tower with given height
def print_tower(height: int):
    # Fills tower line with spaces
    tower = " " * height

    # Prints the tower
    for i in range(1, height + 1):
        # Adds spaces and "#", equivalent to the line it is
        tower = " " * (height - i) + "#" * i
        # Prints the left and right side tower
        print(f"{tower}  {tower.strip()[::-1]}")


main()