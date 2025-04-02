"""
=============================================================================
Title: assignment9.py
Description: This is assignment 9
Author: Kyle Hinkle
Date: 03/28/2025
Version:
Usage:
Notes:
Python Version: 3.x.x
=============================================================================
"""

# Imports
import turtle

"""Draws a nested triangle using recursion."""
def nestedTriangle(yertle, side):
    if side < 10:
        return

    for _ in range(3):  # Draw an equilateral triangle
        yertle.forward(side)
        yertle.left(120)
    # Should be facing east after loop

    # Move forward before recursive call to avoid overlapping drawings
    yertle.penup()
    yertle.forward(5)
    yertle.pendown()

    nestedTriangle(yertle, (side - 10))

"""Draws a parameterized fractal tree using recursion."""
def parameterizedTree(yertle, trunkLength, rightAngle, leftAngle, rightReduction, leftReduction):
    if trunkLength < 10:
        return

    # Move forward (draw trunk)
    yertle.forward(trunkLength)

    # Left Branch
    yertle.left(leftAngle)
    parameterizedTree(yertle, trunkLength / leftReduction, rightAngle, leftAngle, rightReduction, leftReduction)
    yertle.right(leftAngle)  # Return to trunk position

    # Right Branch
    yertle.right(rightAngle)
    parameterizedTree(yertle, trunkLength / rightReduction, rightAngle, leftAngle, rightReduction, leftReduction)
    yertle.left(rightAngle)  # Return to trunk position

    # Move backward to return to the previous branch point
    yertle.backward(trunkLength)

def branchingTree(yertle, trunkLength, branches) :
    if branches <= 0:
        return
    
    # Set base parameters (change to values if you want it to look like a tree)
    rightAngle = 35 
    leftAngle = 65

    # Move forward (draw trunk)
    yertle.forward(trunkLength)

    # Left Branch
    yertle.left(leftAngle)
    branchingTree(yertle, trunkLength, branches - 1)
    yertle.right(leftAngle)  # Return to trunk position

    # Right Branch
    yertle.right(rightAngle)
    branchingTree(yertle, trunkLength, branches -1)
    yertle.left(rightAngle)  # Return to trunk position

    # Move backward to return to the previous branch point
    yertle.backward(trunkLength)

# Turtle setup
screen = turtle.Screen()
screen.screensize(2300, 1100)
drawerTurtle = turtle.Turtle()
drawerTurtle.speed(0)
screen.tracer(0)  # Disable auto-update for fast rendering

# Position and draw the triangle
drawerTurtle.penup()
drawerTurtle.goto(-75, 400)
drawerTurtle.pendown()
nestedTriangle(drawerTurtle, 150)

# Position the tree (Tree)
drawerTurtle.left(90)
drawerTurtle.penup()
drawerTurtle.goto(-800, -250)  # Adjusted for better placement
drawerTurtle.pendown()

# Create the tree (Tree)
parameterizedTree(drawerTurtle, 100, 35, 35, 1.25, 1.25)

# Position the second tree (Hexagon)
drawerTurtle.penup()
drawerTurtle.goto(0, -100)  # Adjusted for better placement
drawerTurtle.pendown()

# Create the second tree (Hexagon)
parameterizedTree(drawerTurtle, 100, 65, 65, 1.155, 1.155)

# Position the third tree (Butterfly)
drawerTurtle.penup()
drawerTurtle.goto(800, -200)  # Adjusted for better placement
drawerTurtle.pendown()

# Create the third tree (Butterfly)
parameterizedTree(drawerTurtle, 150, 120, 120, 1.25, 1.25)

# Position the fourth tree (Spiral)
drawerTurtle.penup()
drawerTurtle.goto(800, 200)  # Adjusted for better placement
drawerTurtle.pendown()

# Create the fourth tree (Spiral)
branchingTree(drawerTurtle, 20, 10)

# Update screen once drawing is complete
screen.update()

# Keep window open to view the drawing
turtle.done()
