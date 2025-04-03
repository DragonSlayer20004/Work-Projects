"""
=============================================================================
Title
: assignment7.py
Description : This is assignment 7
Author
: Kyle Hinkle
Date: 003/17/2025
Version:
Usage:
Notes:
Python Version: 3.x.x
=============================================================================
"""
import re
import random

def parseFile(filename):
    with open(filename, 'r') as file:
        text = file.read().lower()
        return re.findall("[a-z]+['-]?[a-z]+|[a-z]+", text)

def ghostDictionary(words):
    # Initialize the dictionary with the empty string entry.
    # Brackets indicate that we are adding a list.
    dictionary = {"" : [words[0].lower()]}
    
    # Loop through the words except for the last one to prevent out of bounds error.
    for i in range(len(words) - 1):
        words[i] = words[i].lower()
        if words[i] not in dictionary:
            # Create a new empty list for a new word thats not already in dictionary
            dictionary[words[i]] = []
        # Add the next word to the list of the current word
        dictionary[words[i]].append(words[i + 1])
    return dictionary



def ghostWriter(dictionary, word, number):
    for x in range(number):
        print(word, end=" ")
        if word not in dictionary:
            word = ""
        word = random.choice(dictionary[word.lower()])

#main
filename = input('Enter file name: ')
words = parseFile(filename)
dictionary = ghostDictionary(words)
word = input('Enter starting word: ')
number = int(input('Enter number of words: '))
ghostWriter(dictionary, word, number)

#FILES: Assignments\AssignmentHelpers\Assignment7\alice.txt