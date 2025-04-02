"""
=============================================================================
Title
: assignment3.py
Description : This is assignment 3
Author
: Kyle Hinkle
Date: 02/03/2025
Version:
Usage:
Notes:
Python Version: 3.x.x
=============================================================================
"""
#Imports
import math

#Cube
def cube(x):
    return pow(x, 3)

#Area
def area(radius):
    return (math.pi * pow(radius, 2))

#Leftover cupcakes
def leftovers(cupcakes, people):
    return cupcakes % people

#Even or odd
def parity(number):
    if(number % 2 == 1):
        return "Odd"
    else:
        return "Even"

#Grab biggest
def biggest(x, y, z):
    if(x > y & x > z):
        return x
    elif(y > z):
        return y
    else:
        return z

#Return the sum of 1 up to n
def series(n):
    total = 0
    while(n != 0):
        total += n
        n -= 1
    return total

#Identify quadrant location
def quadrant(x, y):
    if(x > 0):
        if(y > 0):
            return 1
        else:
            return 4
    else:
        if(y > 0):
            return 2
        else:
            return 3
        
#Return the inititals of someone's name.
def initials(first, last):
    return (first[0] + ". " + last[0] + ".")

#Return the longest word.
def longer(word1, word2):
    if(len(word1) >= len(word2)):
        return word1
    else:
        return word2

#Return the location of a letter.
def alphabetNumber(letter):
    if(letter > 'a' and letter <= 'z'):
        return (ord(letter) - ord('a') + 1)
    elif(letter > 'A' and letter <= 'Z'):
        return (ord(letter) - ord('A') + 1)
    else:
        return -1

#Returns the file name with an extension attached.
def addExtension(file, extension):
    location = file.find("." + extension)
    if(location == -1):
        return file + "." + extension
    else:
        return file

#Combine two words in the worst way possible.
def interleave(string1, string2):
    result = []
    min_length = min(len(string1), len(string2))
    
    # Combine characters from both strings
    for i in range(min_length):
        result.append(string1[i])
        result.append(string2[i])
    
    # Append remaining characters from the longer string
    result.append(string1[min_length:])
    result.append(string2[min_length:])
    
    return ''.join(result)

#Testing
print("Cubed form of 4:", cube(4))
print("Area of a circle with radius 2.8:", area(2.8))
print("Cupcakes remaining with 5 people and 16 cupcakes:", leftovers(16, 5))
print("Cupcakes remaining with 5 people and 15 cupcakes:", leftovers(15, 5))
print("Cupcakes remaining with 6 people and 3 cupcakes:", leftovers(3, 6))
print("Parity of 2:", parity(2))
print("Parity of 9:", parity(9))
print("Parity of 0:", parity(0))
print("Biggest of 2, 9, 4:", biggest(2, 9, 4))
print("Biggest of 5, 3, 5:", biggest(5, 3, 5))
print("Biggest of 10, 10, 10:", biggest(10, 10, 10))
print("Sum of integers from 5:", series(5))
print("Sum of integers from 100:", series(100))
print("Sum of integers from 241:", series(241))
print("Quadrant location of (1, 5):", quadrant(1, 5))
print("Quadrant location of (-2, 4):", quadrant(-2, 4))
print("Quadrant location of (-4, -6):", quadrant(-4, -6))
print("Quadrant location of (5, -8):", quadrant(5, -8))
print("Initials of the name Tom Hanks:", initials("Tom", "Hanks"))
print("Initials of the name Kevin Smith:", initials("Kevin", "Smith"))
print("Longest word between goat and diversion:", longer("goat", "diversion"))
print("Longest word between goat and goat:", longer("goat", "goat"))
print("Longest word between combat and wombat:", longer("combat", "wombat"))
print("Alphabet number of M:", alphabetNumber('M'))
print("Alphabet number of P:", alphabetNumber('P'))
print("Alphabet number of &:", alphabetNumber('&'))
print("Adding file extension pdf to file name data:", addExtension("data", "pdf"))
print("Adding file extension pdf to file name data.pdf:", addExtension("data.pdf", "pdf"))
print("Adding file extension pdf to file name data.exe:", addExtension("data.exe", "pdf"))
print("Combined words help and flat:", interleave("help", "flat"))
print("Combined words mango and biscuit:", interleave("mango", "biscuit"))
