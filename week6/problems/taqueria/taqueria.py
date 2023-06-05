# Defines the Taqueria Menu
menu = \
{
    "Baja Taco": 4.00,
    "Burrito": 7.50,
    "Bowl": 8.50,
    "Nachos": 11.00,
    "Quesadilla": 8.50,
    "Super Burrito": 8.50,
    "Super Quesadilla": 9.50,
    "Taco": 3.00,
    "Tortilla Salad": 8.00
}

cost: float = 0

# Entry point of the application
while True:
    try:
        item = input("Item: ")
        # Normalizes the item input
        item = item.strip().title()
    # If the user hits the Cntl-D
    except EOFError:
        print()
        break

    # If the item exists in the Menu, sums it to the total cost
    if item in menu:
        cost += menu[item]

        print(f"Price: ${cost:.2f}")