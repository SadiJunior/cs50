import csv
import requests


def main():
    # Read NYTimes Covid Database
    download = requests.get(
        "https://raw.githubusercontent.com/nytimes/covid-19-data/master/us-states.csv"
    )
    decoded_content = download.content.decode("utf-8")
    file = decoded_content.splitlines()
    reader = csv.DictReader(file)

    # Construct 14 day lists of new cases for each states
    new_cases = calculate(reader, 14)

    # Create a list to store selected states
    states = []
    print("Choose one or more states to view average COVID cases.")
    print("Press enter when done.\n")

    while True:
        state = input("State: ")
        if state in new_cases:
            states.append(state)
        if len(state) == 0:
            break

    print(f"\nSeven-Day Averages")

    # Print out 7-day averages for this week vs last week
    comparative_averages(new_cases, states)


# Creates a dictionary to store 14 most recent days of new cases by state
def calculate(reader, days):
    # Reverses the reader, for getting the newest data first
    reader_ordered = reversed(list(reader))
    all_cases_by_state: {str: list} = {}

    # Gets a list of cases in the last days for each city
    for row in reader_ordered:
        state_name = row["state"]
        state_cases = row["cases"]

        if all_cases_by_state.get(state_name) is None:
            all_cases_by_state[state_name] = []

        if len(all_cases_by_state.get(state_name)) < days:
            all_cases_by_state[state_name].append(state_cases)
        else:
            if all(len(all_cases) == days for all_cases in all_cases_by_state.values()):
                break

    # Gets the daily cases by state (cases are cumulative)
    daily_cases_by_state: {str: list} = {}
    for state, all_cases in all_cases_by_state.items():
        initial_case = int(all_cases[-1])
        ordered_all_cases = all_cases[::-1]
        all_daily_cases = [int(cases) - initial_case for cases in ordered_all_cases]
        all_daily_cases = [all_daily_cases[i] - all_daily_cases[i-1] if i != 0 else all_daily_cases[i] for i in range(len(all_daily_cases))]

        daily_cases_by_state[state] = all_daily_cases

    return daily_cases_by_state


# Calculates and prints out seven day average for given state
def comparative_averages(new_cases, states):
    for state in states:
        cases = new_cases[state]
        first_week = cases[0:7]
        second_week = cases[7:14]

        first_average = sum(first_week) / len(first_week)
        second_average = sum(second_week) / len(second_week)

        difference = round(second_average - first_average)
        decrease = round((difference / first_average) * 100)

        print(f"{state} had a 7-day average of {second_average:.0f} and a decrease of {decrease:.0f}%.\n")


main()
