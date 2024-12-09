package javaClasses.databaseClasses;

import javaClasses.webServer.UserWithGrades;

import java.sql.*;

public class UserAndGradesRetrieval {

    public static UserWithGrades getUserWithGrades(int userID) {
        String query = "SELECT u.userID, u.userName, q.gradeID, q.quizName, q.grade " +
                "FROM Users u " +
                "LEFT JOIN QuizGrades q ON u.userID = q.userID " +
                "WHERE u.userID = ?";

        UserWithGrades userWithGrades = null;

        try (Connection connection = DatabaseConnector.getConnection();
             PreparedStatement statement = connection.prepareStatement(query)) {

            statement.setInt(1, userID);
            ResultSet resultSet = statement.executeQuery();

            // Aggregate data for the user
            while (resultSet.next()) {
                if (userWithGrades == null) {
                    userWithGrades = new UserWithGrades(
                            resultSet.getInt("userID"),
                            resultSet.getString("userName")
                    );
                }
                int gradeID = resultSet.getInt("gradeID");
                String quizName = resultSet.getString("quizName");
                int grade = resultSet.getInt("grade");

                // Add grade details only if they exist
                if (quizName != null && gradeID != 0) {
                    userWithGrades.addGrade(gradeID, quizName, grade);
                }
            }
        } catch (SQLException e) {
            System.err.println("Error retrieving user and grades: " + e.getMessage());
            e.printStackTrace();
        }
        return userWithGrades;
    }
}
