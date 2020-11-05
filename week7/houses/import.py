from cs50 import SQL
from csv import DictReader
from sys import argv, exit

# Check command line args and file type
if len(argv) < 2:
    print("Usage: python import.py characters.csv")
    exit()
else:
    if not (argv[1].endswith(".csv") or argv[1].endswith(".tsv")):
        print("Usage: python import.py characters.csv")
        exit()

# Create database by opening and closing an empty file first
open(f"students.db", "w").close()
db = SQL("sqlite:///students.db")

# Create table called students and specify columns
db.execute("CREATE TABLE students (first TEXT, middle TEXT, last TEXT, house TEXT, birth NUMERIC)")

# Open csv file
with open(argv[1], "r") as students:
    # Create DictReader
    reader = DictReader(students)
    # Iterate over csv file
    for row in reader:
        fullName = row["name"].split()
        # Insert student by substituting values into each ? placeholder
        # Insert None as middle name if non-existent
        if len(fullName) < 3:
            db.execute("INSERT INTO students (first, middle, last, house, birth) VALUES(?, ?, ?, ?, ?)",
                       fullName[0], None, fullName[1], row["house"], row["birth"])
        else:
            db.execute("INSERT INTO students (first, middle, last, house, birth) VALUES(?, ?, ?, ?, ?)",
                       fullName[0], fullName[1], fullName[2], row["house"], row["birth"])
