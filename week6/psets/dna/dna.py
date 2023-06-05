import csv
import sys


def main():

    # Check for command-line usage
    if len(sys.argv) != 3:
        sys.exit("Usage: python dna.py data.csv sequence.txt")

    # Read database file into a variable
    data_filename = sys.argv[1]
    with open(data_filename) as data_file:
        data_reader = csv.DictReader(data_file)
        database = [item for item in data_reader]

    # Read DNA sequence file into a variable
    sequence_filename = sys.argv[2]
    with open(sequence_filename, "r") as sequence_file:
        sequence = sequence_file.readline().strip()

    # Find longest match of each STR in DNA sequence
    subsequences = list(filter(lambda attribute: attribute != "name", database[0].keys()))
    longest_matches = {subsequence: longest_match(sequence, subsequence) for subsequence in subsequences}

    # Check database for matching profiles
    matching_profiles = list(filter(lambda profile: all(
        [int(profile[subsequence]) == longest_matches[subsequence] for subsequence in subsequences]), database))

    if len(matching_profiles) == 1:
        matching_profile = matching_profiles[0]["name"]
        print(f"{matching_profile}")
    else:
        print("No match")

    return


def longest_match(sequence, subsequence):
    """Returns length of longest run of subsequence in sequence."""

    # Initialize variables
    longest_run = 0
    subsequence_length = len(subsequence)
    sequence_length = len(sequence)

    # Check each character in sequence for most consecutive runs of subsequence
    for i in range(sequence_length):

        # Initialize count of consecutive runs
        count = 0

        # Check for a subsequence match in a "substring" (a subset of characters) within sequence
        # If a match, move substring to next potential match in sequence
        # Continue moving substring and checking for matches until out of consecutive matches
        while True:

            # Adjust substring start and end
            start = i + count * subsequence_length
            end = start + subsequence_length

            # If there is a match in the substring
            if sequence[start:end] == subsequence:
                count += 1

            # If there is no match in the substring
            else:
                break

        # Update most consecutive matches found
        longest_run = max(longest_run, count)

    # After checking for runs at each character in seqeuence, return longest run found
    return longest_run


main()
