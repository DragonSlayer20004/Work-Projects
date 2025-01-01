package javaClasses.databaseClasses;

import javaClasses.webServer.AnswerRequest;
import java.sql.Connection;
import java.sql.PreparedStatement;
import java.sql.SQLException;

public class AnswerCreationDepository {

    // Method to insert a single answer
    public static boolean insertAnswer(AnswerRequest answer) {
        String insertAnswerQuery = "INSERT INTO Answers (questionID, answerDescription) VALUES (?, ?)";

        try (Connection connection = DatabaseConnector.getConnection();
             PreparedStatement answerStatement = connection.prepareStatement(insertAnswerQuery)) {

            // Set the parameters for questionID and answerDescription
            answerStatement.setInt(1, answer.getQuestionID());
            answerStatement.setString(2, answer.getAnswerDescription());

            // Execute the insert statement
            answerStatement.executeUpdate();
            return true;

        } catch (SQLException e) {
            System.err.println("Error inserting answer: " + e.getMessage());
            return false;
        }
    }
}
