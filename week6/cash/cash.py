from cs50 import get_float

dollars = get_float("Change owed: ")
while dollars <= 0.00:
    dollars = get_float("Change owed: ")

cents = round(dollars * 100)
quarter = 25
dime = 10
nickel = 5
penny = 1
coins = 0

while cents >= quarter:
    cents -= quarter
    coins += 1
while cents >= dime:
    cents -= dime
    coins += 1
while cents >= nickel:
    cents -= nickel
    coins += 1
while cents >= penny:
    cents -= penny
    coins += 1

print(f"{coins}")