package javaClasses.databaseClasses;

// Drag-and-drop question
public class DragAndDropQuestion extends Question {
    private int correctAnswer;
    private String wrongAnswerOne;
    private String wrongAnswerTwo;
    private String wrongAnswerThree;

    public DragAndDropQuestion(int questionID, String description, int correctAnswer, String wrongOne, String wrongTwo, String wrongThree) {
        this.questionID = questionID;
        this.description = description;
        this.correctAnswer = correctAnswer;
        this.wrongAnswerOne = wrongOne;
        this.wrongAnswerTwo = wrongTwo;
        this.wrongAnswerThree = wrongThree;
    }
    // Getters, setters, and other methods
}
