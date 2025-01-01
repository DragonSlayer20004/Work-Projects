package javaClasses.webServer;

import javaClasses.databaseClasses.QuizGrade;

import java.util.ArrayList;
import java.util.List;

public class UserWithGrades {
    private int userID;
    private String userName;
    private List<QuizGrade> grades;

    public UserWithGrades(int userID, String userName) {
        this.userID = userID;
        this.userName = userName;
        this.grades = new ArrayList<>();
    }

    public int getUserID() {
        return userID;
    }

    public void setUserID(int userID) {
        this.userID = userID;
    }

    public String getUserName() {
        return userName;
    }

    public void setUserName(String userName) {
        this.userName = userName;
    }

    public List<QuizGrade> getGrades() {
        return grades;
    }

    public void addGrade(int gradeID, String quizName, int grade) {
        this.grades.add(new QuizGrade(gradeID, quizName, grade));
    }

    @Override
    public String toString() {
        StringBuilder json = new StringBuilder();
        json.append("{")
                .append("\"userID\":").append(userID).append(",")
                .append("\"userName\":\"").append(userName).append("\",")
                .append("\"grades\":[");
        for (QuizGrade grade : grades) {
            json.append("{")
                    .append("\"gradeID\":").append(grade.getGradeID()).append(",")
                    .append("\"quizName\":\"").append(grade.getQuizName()).append("\",")
                    .append("\"grade\":").append(grade.getGrade())
                    .append("},");
        }
        if (!grades.isEmpty()) {
            json.setLength(json.length() - 1); // Remove the trailing comma
        }
        json.append("]}");
        return json.toString();
    }
}
