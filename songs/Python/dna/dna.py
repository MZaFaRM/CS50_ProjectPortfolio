import csv/workspaces/98420006/Python/dna/sequences
import sys


def main():

    # Check for command-line usage
    if len(sys.argv) != 3:
        print("Usage: python dna.py data.csv sequence.txt")

    csv_file = sys.argv[1]
    text_file = sys.argv[2]

    # Read database file into a variable
    DNA_database = []
    with open(csv_file) as database:
        reader = csv.DictReader(database)
        for row in reader:
            DNA_database.append(row)

    # converts all the read value to int *if possible (Try) in the DNA_database
    for person in DNA_database:
        for k, v in person.items():
            try:
                v = int(v)
            except ValueError:
                pass
            person[k] = v

    # Read DNA sequence file into a variable
    DNA_reader = open(text_file, 'r')
    DNA_sequence = DNA_reader.read()

    # creates a person dictionary with placeholders for their DNA values
    person = {}
    person = DNA_database[0].copy()
    del person["name"]

    # Find longest match of each STR in DNA sequence
    for k, v in person.items():
        v = longest_match(DNA_sequence, k)
        person[k] = v

    # Check database for matching profiles
    for people in DNA_database:
        name = people['name']
        del people['name']
        if people == person:
            print(name)
            exit()
    print("No match")
    exit()


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
