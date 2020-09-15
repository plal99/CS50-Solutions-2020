from cs50 import get_int


# pyramid function
def pyramid(num):
    for i in range(1, num + 1):
        print(" "*(num - i), "#"*i, "  ", "#"*i, sep="")


# input loop
while True:
    num = get_int("Height: ")
    if num in range(1, 9):
        break

# function calling
pyramid(num)
