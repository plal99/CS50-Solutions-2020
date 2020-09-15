# importing necessory files
import sys
import csv
import re

# checcking if the input in the terminal is correct or not
if (len(sys.argv) != 3):
    print("Please enter properly")
    sys.exit()
database = sys.argv[1]
sequence = sys.argv[2]

# opening the database
csvFile = csv.DictReader(open(database, "r"))

# predefining the STRs
largeSTR = ["AGATC", "TTTTTTCT", "AATG", "TCTAG", "GATA", "TATC", "GAAA", "TCTG"]
smallSTR = ["AGATC", "AATG", "TATC"]

# setting the STRmain according to the database in the input
if database == "databases/small.csv":
    STRmain = smallSTR
else:
    STRmain = largeSTR

# a dict for storing the number of occurences
STRdict = {}

# opening the sequence as file
sample = open(sequence, "r")

# finding the number of occurences
for line in sample:
    for STR in STRmain:
        try:
            count = max(re.findall('((?:' + STR + ')+)', line))
            STRdict[STR] = int(len(count) / len(STR))
        except:
            STRdict[STR] = 0
        

# finding the name from csvFile
flag = False
for row in csvFile:
    count1 = 0
    for STR in STRmain:
        if int(row.get(STR)) == STRdict.get(STR):
            count1 += 1
            if count1 == len(STRmain):
                print(row['name'])
                flag = True
    
if flag is False: 
    print("No match")
