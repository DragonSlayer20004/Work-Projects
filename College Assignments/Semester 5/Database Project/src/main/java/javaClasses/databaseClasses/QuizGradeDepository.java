package javaClasses.databaseClasses;

import javaClasses.webServer.QuizGradeData;

import java.sql.Connection;
import java.sql.PreparedStatement;
import java.sql.SQLException;

public class QuizGradeDepository {

    private static final String INSERT_QUERY =
            "INSERT INTO QuizGrades (userID, quizName, grade) VALUES (?, ?, ?)";

    public static boolean saveQuizGrade(QuizGradeData quizGradeData) {
        try (Connection connection = DatabaseConnector.getConnection();
             PreparedStatement statement = connection.prepareStatement(INSERT_QUERY)) {

            // Set parameters for the prepared statement
            statement.setInt(1, quizGradeData.getUserID());
            statement.setString(2, quizGradeData.getQuizName());
            statement.setInt(3, quizGradeData.getGrade());

            // Execute the update
            int rowsInserted = statement.executeUpdate();
            return rowsInserted > 0;

        } catch (SQLException e) {
            e.printStackTrace();
            return false;
        }
    }
}
