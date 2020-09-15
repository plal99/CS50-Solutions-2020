# import various header files
import csv
import sys
from cs50 import SQL

# check for commandline arguments
if (len(sys.argv) != 2):
    print("Please enter properly")
    sys.exit()

# create or rewrite students.db each time code runs
open("students.db", "w").close()

# set up a database connection (courtesy of cs50)
db = SQL("sqlite:///students.db")

db.execute("CREATE TABLE students (first TEXT,middle VARCHAR(20),last TEXT,house TEXT,birth NUMERIC);")

with open(sys.argv[1], "r") as file:
    file = csv.DictReader(file)
    for row in file:

        # getting name from csv
        fullName = str(row["name"])
        fullName = fullName.split()
        if len(fullName) == 3:
            first = fullName[0]
            middle = fullName[1]
            last = fullName[2]
        else:
            first = fullName[0]
            middle = None
            last = fullName[1]

        # getting house and birth year from csv
        house = row["house"]
        birth = row["birth"]

        db.execute("INSERT INTO students (first, middle, last, house, birth) VALUES (?, ?, ?, ?, ?)", first, middle, last, house, birth)

