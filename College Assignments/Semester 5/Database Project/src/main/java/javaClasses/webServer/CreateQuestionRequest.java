package javaClasses.webServer;

public class CreateQuestionRequest {
    private int questionID;
    private String questionType;
    private String questionDescription;
    private int multichoiceOptions; // For multiple-choice questions
    private int correctAnswer; // For multiple-choice and drag-and-drop questions
    private String correctAnswerText; // For fill-in-the-blank questions
    private int wrongAnswerOne; // For drag-and-drop questions
    private int wrongAnswerTwo; // For drag-and-drop questions
    private int wrongAnswerThree; // For drag-and-drop questions

    // Getters and Setters
    public int getQuestionID() {
        return questionID;
    }

    public void setQuestionID(int questionID) {
        this.questionID = questionID;
    }

    public String getQuestionType() {
        return questionType;
    }

    public void setQuestionType(String questionType) {
        this.questionType = questionType;
    }

    public String getQuestionDescription() {
        return questionDescription;
    }

    public void setQuestionDescription(String questionDescription) {
        this.questionDescription = questionDescription;
    }

    public int getMultichoiceOptions() {
        return multichoiceOptions;
    }

    public void setMultichoiceOptions(int multichoiceOptions) {
        this.multichoiceOptions = multichoiceOptions;
    }

    public int getCorrectAnswer() {
        return correctAnswer;
    }

    public void setCorrectAnswer(int correctAnswer) {
        this.correctAnswer = correctAnswer;
    }

    public String getCorrectAnswerText() {
        return correctAnswerText;
    }

    public void setCorrectAnswerText(String correctAnswerText) {
        this.correctAnswerText = correctAnswerText;
    }

    public int getWrongAnswerOne() {
        return wrongAnswerOne;
    }

    public void setWrongAnswerOne(int wrongAnswerOne) {
        this.wrongAnswerOne = wrongAnswerOne;
    }

    public int getWrongAnswerTwo() {
        return wrongAnswerTwo;
    }

    public void setWrongAnswerTwo(int wrongAnswerTwo) {
        this.wrongAnswerTwo = wrongAnswerTwo;
    }

    public int getWrongAnswerThree() {
        return wrongAnswerThree;
    }

    public void setWrongAnswerThree(int wrongAnswerThree) {
        this.wrongAnswerThree = wrongAnswerThree;
    }

    @Override
    public String toString() {
        return "QuestionRequest{" +
                "questionID=" + questionID +
                ", questionType='" + questionType + '\'' +
                ", questionDescription='" + questionDescription + '\'' +
                ", multichoiceOptions=" + multichoiceOptions +
                ", correctAnswer=" + correctAnswer +
                ", correctAnswerText='" + correctAnswerText + '\'' +
                ", wrongAnswerOne=" + wrongAnswerOne +
                ", wrongAnswerTwo=" + wrongAnswerTwo +
                ", wrongAnswerThree=" + wrongAnswerThree +
                '}';
    }
}
