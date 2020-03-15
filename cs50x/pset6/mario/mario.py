from cs50 import get_int

# ask user for height
while True:
    h = get_int("height: ")
    if h > -1 and h < 24:
        break

# print half pyramid


def print_half_pyramid(height):
    for i in range(1, h+1, +1):
        for j in range(h, -1, -1):
            if j > i:
                print(" ", end='')
            else:
                print("#", end='')
        print("")


# call function
print_half_pyramid(h)
