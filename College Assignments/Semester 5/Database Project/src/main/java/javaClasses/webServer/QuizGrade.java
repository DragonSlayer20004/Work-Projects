package javaClasses.databaseClasses;

public class QuizGrade {
    private int gradeID;
    private String quizName;
    private int grade;

    public QuizGrade(int gradeID, String quizName, int grade) {
        this.gradeID = gradeID;
        this.quizName = quizName;
        this.grade = grade;
    }

    public int getGradeID() {
        return gradeID;
    }

    public void setGradeID(int gradeID) {
        this.gradeID = gradeID;
    }

    public String getQuizName() {
        return quizName;
    }

    public void setQuizName(String quizName) {
        this.quizName = quizName;
    }

    public int getGrade() {
        return grade;
    }

    public void setGrade(int grade) {
        this.grade = grade;
    }
}
