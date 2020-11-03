from cs50 import get_string

text = get_string("Text: ")
letters = 0
words = 1
sentences = 0

for char in text:
    if char.isalpha():
        letters += 1
    if char == " ":
        words += 1
    if char == '.' or char == '!' or char == '?':
        sentences += 1
    if len(text) < 1:
        words = 0

L = (letters / words) * 100
S = (sentences / words ) * 100
grade = 0.0588 * L - 0.296 * S - 15.8

if grade < 16 and grade >= 0:
    print(f"Grade {round(grade)}")
elif grade >= 16:
    print(f"Grade 16+")
else:
    print(f"Before Grade 1")

