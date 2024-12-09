package javaClasses.databaseClasses;

// Multi-choice question
public class MultiChoiceQuestion extends Question {
    private int optionsCount;
    private int correctAnswer;

    public MultiChoiceQuestion(int questionID, String description, int optionsCount, int correctAnswer) {
        this.questionID = questionID;
        this.description = description;
        this.optionsCount = optionsCount;
        this.correctAnswer = correctAnswer;
    }
    // Getters, setters, and other methods
}
