# for regex
import re

# inputing
print("Text: ", end="")
text = str(input())

# letter count loop
letterCount = 0
for char in text:
    if char.isalpha():
        letterCount += 1

# word count
word = text.split(" ")
wordCount = len(word)

# sentence count
sentence = re.split('\.|!|\?', text)
sentenceCount = len(sentence) - 1

l = letterCount * 100 / wordCount
s = sentenceCount * 100 / wordCount

# index calculation
index = round(0.0588 * l - 0.296 * s - 15.8)

# loop to determine the calculation
if index > 16:
    print("Grade 16+")
elif index < 1:
    print("Before Grade 1")
else:
    print("Grade", index)

