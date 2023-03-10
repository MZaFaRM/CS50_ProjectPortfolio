# Counts number of students in houses, using a DictReader

import csv

# Numbers of students in houses
houses = {
    "Gryffindor": 0,
    "Hufflepuff": 0,
    "Ravenclaw": 0,
    "Slytherin": 0
}

# Count votes
with open("hogwarts/hogwarts.csv", "r") as file:
    reader = csv.DictReader(file)
    for row in reader:
        house = row["Timestamp"]
        house = int(house)
        houses[house].value += 1

# Print counts
# for house in houses:
    # print(f"{house}: {houses[house]}")
