"""
=============================================================================
Title
: assignment4.py
Description : This is assignment 4
Author
: Kyle Hinkle
Date: 02/010/2025
Version:
Usage:
Notes:
Python Version: 3.x.x
=============================================================================
"""
#Imports
import random
import turtle
import math

#Global Variables
count = 0
cell_size = 70
width, height = 600, 700
cols = 2
rows = height // cell_size
turt = turtle.Turtle()

# Function to calculate Buffon Approximation
def buffonApproximation(needles):
    #Screen setup
    screen = turtle.Screen()
    screen.setup(800, 800)
    screen.tracer(0)  # Disable animation for instant drawing (Wish I knew this earlier)

    # Turtle setup
    turt.speed(0)
    turt.hideturtle()

    draw_grid()
    for _ in range(needles):
        draw_random_lines(100)

    calculated = (needles * 2) / count

    return calculated



# Function to draw the grid
def draw_grid():
    turt.penup()
    for i in range(rows + 1):
        # Horizontal lines
        y = i * cell_size - height // 2
        turt.goto(-width // 2, y)
        turt.pendown()
        turt.goto(width // 2, y)
        turt.penup()

        # Vertical lines
        # First column
        x = 0 * cell_size - width // 2
        turt.goto(x, -height // 2)
        turt.pendown()
        turt.goto(x, height // 2)
        turt.penup()

        #Last column
        x = width // 2
        turt.goto(x, -height // 2)
        turt.pendown()
        turt.goto(x, height // 2)
        turt.penup()

# Function to draw random lines inside the grid
def draw_random_lines(line_length=70):
    global count
    turt.color("red")
    x1 = random.randint(-width // 2, width // 2)
    y1 = random.randint(-height // 2, height // 2)
    
    angle = random.uniform(0, 360)  # Random direction
    x2 = x1 + line_length * math.cos(math.radians(angle))
    y2 = y1 + line_length * math.sin(math.radians(angle))

    # Ensure the line stays within screen bounds 
    x2 = max(-width // 2, min(width // 2, x2))
    y2 = max(-height // 2, min(height // 2, y2))

    turt.penup()
    turt.goto(x1, y1)
    turt.pendown()
    turt.goto(x2, y2)
    turt.penup()

    # Check if the line crosses any horizontal grid line
    for k in range(rows + 1):
        y_grid = k * cell_size - height // 2  # Y position of grid line same as declared above
        if min(y1, y2) <= y_grid <= max(y1, y2):  # If the line crosses the grid line. Min max since either y1 or y2 could be bigger than the other.
            count += 1
            break  # Stop checking after the first intersection otherwise there would be dupes


#Main
needles = 10
calculated = buffonApproximation(needles)

print("Calculated value is: ", calculated, "\nThere are", count, "lines intersecting the box lines.")

# Keep window open to view the lines
turtle.done()