from cs50 import get_int

# inputing card number 
num = get_int("Number: ")
strNum = str(num)
numLen = len(strNum)

# calculating if the card number is valid or not
fsum = 0
for i in range(numLen-2, -1, -2):
    tempSum = int(strNum[i]) * 2
    if tempSum > 9:
        tempSum = tempSum % 10 + (tempSum // 10) % 10
    fsum += tempSum

for i in range(numLen-1, -1, -2):
    fsum += int(strNum[i])

# tag set to true if card is valid
valid = False
if fsum > 9 and fsum % 10 == 0:
    valid = True

# condition for AMERICAN EXPRESS
if numLen == 15 and strNum[:2] in ["34", "37"]:
    print("AMEX")
# condition for MASTERCARD
elif numLen == 16 and strNum[:2] in ["51", "51", "53", "54", "55"] and valid:
    print("MASTERCARD")
# condition for VISA
elif (numLen == 16 or numLen == 13) and strNum[0] == "4" and valid:
    print("VISA")
# condition for INVALID
else:
    print("INVALID")

