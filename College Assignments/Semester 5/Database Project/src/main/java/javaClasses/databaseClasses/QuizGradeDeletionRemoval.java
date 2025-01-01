package javaClasses.databaseClasses;

import java.sql.Connection;
import java.sql.DriverManager;
import java.sql.PreparedStatement;
import java.sql.SQLException;

public class QuizGradeDeletionRemoval {

    public static boolean deleteQuizGradeById(int gradeID) {
        String deleteQuery = "DELETE FROM QuizGrades WHERE gradeID = ?";

        try (Connection connection = DatabaseConnector.getConnection();
             PreparedStatement statement = connection.prepareStatement(deleteQuery)) {

            statement.setInt(1, gradeID);
            int rowsAffected = statement.executeUpdate();

            return rowsAffected > 0; // Return true if at least one row was deleted
        } catch (SQLException e) {
            e.printStackTrace();
            return false;
        }
    }
}
