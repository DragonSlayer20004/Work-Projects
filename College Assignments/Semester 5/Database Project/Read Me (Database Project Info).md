Additional Information
Database Configuration
•	Important: Change the password in database.properties.
•	There is currently only one user since that wasn’t the goal of this project.
•	The code is dirty and ugly. I changed styles multiple times and made different ideas throughout. Test classes are left and forgotten but all the important classes and methods all do work with the locally hosted webserver.

Postman Commands:
These are commands that the admin needs to run. They are all POST commands.

Create Question

POST http://127.0.0.1:12345/CreateQuestion

<QuestionRequest>
    <questionID>10</questionID>
    <questionType>multichoice</questionType>
    <questionDescription>What is the hardest rock in the world?</questionDescription>
    <multichoiceOptions>4</multichoiceOptions>
    <correctAnswer>2</correctAnswer>
</QuestionRequest>

<QuestionRequest>
    <questionID>2</questionID>
    <questionType>fillInTheBlank</questionType>
    <questionDescription>_____ is the most common rock in our solar system. </questionDescription>
    <correctAnswerText>Basalt</correctAnswerText>
</QuestionRequest>

<QuestionRequest>
    <questionID>3</questionID>
    <questionType>dragAndDrop</questionType>
    <questionDescription>Match the rock types to their descriptions.</questionDescription>
    <correctAnswer>1</correctAnswer>
    <wrongAnswerOne>2</wrongAnswerOne>
    <wrongAnswerTwo>3</wrongAnswerTwo>
    <wrongAnswerThree>4</wrongAnswerThree>
</QuestionRequest>

Update Question

POST http://127.0.0.1:12345/UpdateQuestion

<QuestionRequest>
    <questionID>2</questionID>
    <questionType>multichoice</questionType>
    <questionDescription>What is my favorite rock type?</questionDescription>
    <multichoiceOptions>4</multichoiceOptions>
    <correctAnswer>8</correctAnswer>
</QuestionRequest>

Create Answer

POST http://127.0.0.1:12345/CreateAnswer

<AnswerRequest>
    <answerDescription>Rock</answerDescription>
    <questionID>10</questionID>
</AnswerRequest>
