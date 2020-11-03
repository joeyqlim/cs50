from cs50 import get_int

num = get_int("input a number: \n")

while num < 1 or num > 8:
    num = get_int("input a number: \n")

for i in range(num):
    for j in range (num):
        if (i + j >= num - 1):
            print("#", end = "")
        else:
            print(" ", end = "")
    print()