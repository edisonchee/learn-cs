import sys
from cs50 import get_string

# Make sure script is ran with appropriate number of args

if len(sys.argv) != 2:
    sys.stdout.write("Please provide key.\n")
    sys.exit(1)

# prep to work with input

input_key = sys.argv[1]

# strings are immutable in python, so work with lists

cipher_key = list()

# Complain if any non-alphabetical letter

if not input_key.isalpha():
    sys.stdout.write("Please input only letters A-Z, a-z.\n")
    sys.exit(1)

# Make cipher key

for c in input_key:
    if ord(c) > 64 and ord(c) < 91:
        cipher_key.append(ord(c) - 65)
    else:
        cipher_key.append(ord(c) - 97)

# set up counter

counter = 0

# get user input for plain text

plain_text = get_string("plaintext: ")
print("ciphertext: ", end="")

# print cipher text

for c in plain_text:
    char = ord(c)
    if counter == len(cipher_key):
        counter = 0
    if char > 64 and char < 91:
        distance = 90 - char
        key_num = cipher_key[counter] % 26
        if key_num < distance or key_num == distance:
            print(chr(char + key_num), end="")
        else:
            print(chr(64 + key_num - distance), end="")
        counter += 1
    elif char > 96 and char < 123:
        distance = 122 - char
        key_num = cipher_key[counter] % 26
        if key_num < distance or key_num == distance:
            print(chr(char + key_num), end="")
        else:
            print(chr(96 + key_num - distance), end="")
        counter += 1
    else:
        print(f"{c}", end="")

print("")
