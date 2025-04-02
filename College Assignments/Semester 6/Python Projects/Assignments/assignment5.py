"""
=============================================================================
Title
: assignment5.py
Description : This is assignment 5
Author
: Kyle Hinkle
Date: 02/17/2025
Version:
Usage:
Notes:
Python Version: 3.x.x
=============================================================================
"""

def fibonacci(n):
    list = [1, 1]
    for x in range(n - 2):
        list.append(list[x] + list[x + 1])
    return list

def removeDuplicates(values):
    list = []
    for x in range(len(values)):
        if values[x] not in list:
            list.append(values[x])
    return list

def mergeSorted(list1, list2):
    newList = []
    listOneLocation = 0
    listTwoLocation = 0
    lowest = 0
    lowestList = True
    while(listOneLocation < len(list1) or listTwoLocation < len(list2)):
        if(listOneLocation < len(list1)):
            lowest = list1[listOneLocation]
            lowestList = True
        if(listTwoLocation < len(list2)):
            if(list2[listTwoLocation] < lowest):
                lowest = list2[listTwoLocation]
                lowestList = False
        if(lowestList == True):
            newList.append(lowest)
            listOneLocation += 1
        elif(lowestList == False):
            newList.append(lowest)
            listTwoLocation += 1
    return newList

def longestWord(filename):
    longestWord = ""
    longestWordCount = 0

    with open(filename, "r") as file: # With simplifies file management so there is no need for a try: finally nor a need to close at the end
        for line in file:
            words = line.split()  # Splitting line into word list
            for word in words:
                if len(word) > longestWordCount:
                    longestWord = word
                    longestWordCount = len(word)

    return longestWord

def occurrences(filename):
    wordDictionary = {}

    with open(filename, "r") as file: # With simplifies file management so there is no need for a try: finally nor a need to close at the end
        for line in file:
            words = line.split()  # Splitting line into word list
            for word in words:
                wordDictionary[word] = wordDictionary.get(word, 0) + 1 # Defaults to 0 if there is nothing there

    file.close()
    return wordDictionary

def misspellings(textFile, wordFile):
    wordDictionary = []
    invalidWordDictionary = []

    #grab all the correct words.
    with open(wordFile, "r") as file: # With simplifies file management so there is no need for a try: finally nor a need to close at the end
        for line in file:
            words = line.split()  # Splitting line into word list
            for word in words:
                wordDictionary.append(word.lower())

    #checks if words in textFile exist in wordDictionary
    with open(textFile, "r") as file: # With simplifies file management so there is no need for a try: finally nor a need to close at the end
        for line in file:
            words = line.split()  # Splitting line into word list
            for word in words:
                if word.lower() not in wordDictionary:
                    invalidWordDictionary.append(word.lower())

    return invalidWordDictionary

#main
print(fibonacci(10))
print()

values = [17, 9, 10, 4, 2, 10, 16, 20, 11, 9]
print(removeDuplicates(values))
print()

list1 = [7, 15, 23, 24, 25, 27, 37, 44, 53, 60]
list2 = [6, 11, 15, 22, 32, 34]
print(mergeSorted(list1, list2))
print()

print(longestWord('Assignments\AssignmentHelpers\Assignment6\gettysburg.txt'))
print()

print(occurrences('Assignments\AssignmentHelpers\Assignment6\gettysburg.txt'))
print()

print(misspellings('Assignments\AssignmentHelpers\Assignment6\gettysburg.txt', 'Assignments\AssignmentHelpers\Assignment6\words.txt'))
