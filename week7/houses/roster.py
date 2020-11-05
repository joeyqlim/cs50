from cs50 import SQL
from sys import argv, exit

# Check command line args and file type
if len(argv) < 2:
    print("Usage: python roster.py houseName")
    exit()

# Open database
db = SQL("sqlite:///students.db")
students = db.execute("SELECT * FROM students WHERE house = (?) ORDER BY last ASC, first ASC", argv[1])

# Print student information
for student in students:
    if student['middle'] == None:
        print(f"{student['first']} {student['last']}, born {student['birth']}")
    else:
        print(f"{student['first']} {student['middle']} {student['last']}, born {student['birth']}")