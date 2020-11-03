from csv import reader, DictReader
from sys import argv
import re

if len(argv) < 3:
    print("Usage: python dna.py data.csv sequence.txt")
    exit()

# open database file
with open(argv[2], "r") as txt_file:
    # store contents of file as string
    dna = txt_file.read()

# initialize a dictionary
sequences = {}

# open person's file
with open(argv[1], "r") as csv_file:
    csv_reader = reader(csv_file)
    for row in csv_reader:
        person_list = row
        # remove name column
        person_list.pop(0)
        # stop at first row
        break

# store sequence names as keys
for item in person_list:
    sequences[item] = 0

# find longest repetitions of each nucleotide
for key in sequences:
    if len(re.findall(r'(({})\2*)'.format(key), dna)) > 0:
        repeats = max(re.findall(r'(({})\2*)'.format(key), dna), key=lambda k: k[0])[0]
        if repeats.count(key) > 0:
            sequences[key] = repeats.count(key)
        else:
            sequences[key] = 0
    else:
        sequences[key] = 0


with open(argv[1], "r") as people_file:
    people = DictReader(people_file)
    for person in people:
        match = 0
        for key in sequences:
            if sequences[key] == int(person[key]):
                match += 1
            # must match all nucleotides in sequence
            if match == len(sequences):
                print(person['name'])
                exit()
    print("No match")
