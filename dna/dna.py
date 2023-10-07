import csv
import sys


def main():

    # TODO: Check for command-line usage
    if not len(sys.argv) == 3:
        print("Error, usage: dna.py <csv database> <txt dna sequence to identify>")
        sys.exit(1)

    # TODO: Read database file into a variable
    people = []
    with open(sys.argv[1]) as file:
        reader = csv.DictReader(file)
        for temp in reader:
            people.append(temp)
        # print(people)

    # TODO: Read DNA sequence file into a variable
    with open(sys.argv[2]) as file:
        dna = file.read()
        # print(dna)

    # TODO: Find longest match of each STR in DNA sequence
    # Extract every STR
    STRS = []
    with open(sys.argv[1]) as file:
        reader = csv.reader(file)
        for temp in reader:
            STRS = temp
            del STRS[0]
            break
    # print(STRS)
    # Calculate longest match for every STR
    MATCH = {}
    for STR in STRS:
        MATCH[STR] = longest_match(dna, STR)
    # print(MATCH)

    # TODO: Check database for matching profiles
    # Iterate over each person
    for person in people:
        # Reset match counter
        match = 0
        # Check each longest sequence
        for STR in STRS:
            if MATCH[STR] == int(person[STR]):
                match += 1
            # If all longest sequences match, stop and identify the person
            if match == len(STRS):
                print(person["name"])
                return
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
