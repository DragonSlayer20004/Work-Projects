"""
=============================================================================
Title
: assignment2.py
Description : This is an example script.
Author
: Kyle Hinkle
Date: 1/31/2025
Version
Usage
Notes
Python Version: 3.x.x
=============================================================================
"""
#Imports
import random
import turtle

#Draw Square
def drawSquare(drawerTurtle):
    for x in range(4):
        drawerTurtle.forward(100)
        drawerTurtle.left(90)

#Move Left
def moveLeft(drawerTurtle):
    drawerTurtle.left(90)

#Move Right
def moveRight(drawerTurtle):
    drawerTurtle.right(90)

#Move forward
def moveForward(drawerTurtle):
    drawerTurtle.forward(100)

#Draw Circle
def drawCircle(drawerTurtle):
    drawerTurtle.circle(50)

#Draw Line
def drawLine(drawerTurtle, distance):
    drawerTurtle.forward(distance)
    drawerTurtle.right(180)
    drawerTurtle.forward(distance)
    drawerTurtle.right(180)

#Draw Rays
def drawRays(drawerTurtle, rayCount, distance):
    for x in range(rayCount):
        drawLine(drawerTurtle, distance)
        #Divide 360 by raycount to get circle like affect
        drawerTurtle.right(360/rayCount)

#Randomly do stuff. Chaos degree affects how much stuff happens
def chaosRays(drawerTurtle, chaosDegree):
    for x in range(chaosDegree):
        match random.randrange(1, 8):
            case 1:
                moveLeft(drawerTurtle)
                print("move turtle left")
            case 2:
                moveRight(drawerTurtle)
                print("move turtle right")
            case 3:
                moveForward(drawerTurtle)
                print("move turtle forward")
            case 4:
                drawSquare(drawerTurtle)
                print("move turtle in a square")
            case 5:
                drawCircle(drawerTurtle)
                print("move turtle in a circle")
            case 6:
                drawerTurtle.hideturtle()
                drawerTurtle.goto(random.randrange(5, 30), random.randrange(5, 30))
                drawerTurtle.showturtle()
                print("teleported turtle somewhere random")
            case _:
                turtle.speed(10)
                drawRays(drawerTurtle, random.randrange(5, 30), random.randrange(50, 100))
                print("move turtle to draw a ray")
                turtle.speed(6)

#Main
drawerTurtle = turtle.Turtle()
chaosRays(drawerTurtle, 100)


#Main
#Move based on text
#Uncomment below to and comment above main to move based on text.
# print("hello turtle")
# drawerTurtle = turtle.Turtle()
# while True:
#     print("make your move")
#     moveTurtle = input()
#     match moveTurtle:
#         case "left":
#             moveLeft(drawerTurtle)
#             print("move turtle left")
#         case "right":
#             moveRight(drawerTurtle)
#             print("move turtle right")
#         case "forward":
#             moveForward(drawerTurtle)
#             print("move turtle forward")
#         case "square":
#             drawSquare(drawerTurtle)
#             print("move turtle in a square")
#         case "circle":
#             drawCircle(drawerTurtle)
#             print("move turtle in a circle")
#         case "ray":
#             drawRays(drawerTurtle, random.randrange(5, 30), random.randrange(50, 100))
#             print("move turtle to draw a ray")
#         case "quit":
#             print("goodbye")
#             break
#         case _:
#             print("try again\nyou said " + moveTurtle + " which is a invalid move")