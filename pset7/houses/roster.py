# import various header files
import sys
from cs50 import SQL

# check for commandline arguments
if (len(sys.argv) != 2):
    print("Please enter properly")
    sys.exit()
    
house = sys.argv[1]

# establishing connection with database
db = SQL("sqlite:///students.db")

# getting details from the database
details = db.execute("SELECT first, middle, last, birth FROM students WHERE house = ? ORDER BY last, first", house)

# printing the data
for row in details:
    if row["middle"] is None:
        print("{} {}, born {}".format(row["first"], row["last"], row["birth"]))
    else:
        print("{} {} {}, born {}".format(row["first"], row["middle"], row["last"], row["birth"]))
