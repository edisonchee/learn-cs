from cs50 import get_float

# ask user for change required
while True:
    x = get_float("How much change is owed: ")
    if x > -1:
        break

# round up x, i.e. cast user input to cents
xRounded = int(x * 100 + 0.5)

c = 0

# greedy algorithm for figuring out how much change

while xRounded != 0:
    while xRounded > 24:
        xRounded -= 25
        c += 1
    while xRounded > 9:
        xRounded -= 10
        c += 1
    while xRounded > 4:
        xRounded -= 5
        c += 1
    while xRounded > 0:
        xRounded -= 1
        c += 1

# print required change

print(f"{c}")
