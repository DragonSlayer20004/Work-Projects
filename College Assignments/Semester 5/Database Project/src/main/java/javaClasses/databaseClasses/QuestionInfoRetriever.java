package javaClasses.databaseClasses;

import java.sql.Connection;
import java.sql.PreparedStatement;
import java.sql.ResultSet;

public class QuestionInfoRetriever {

    public static void retrieveQuestionInfo() {
        // Query to get question description, answer options, and whether each is correct or incorrect
        String query = "SELECT mq.questionDescription AS question, " +
                "a.answerDescription AS answer_option, " +
                "CASE WHEN mq.correctAnswer = a.answerID THEN 'Correct' ELSE 'Incorrect' END AS answer_status " +
                "FROM MultichoiceQuestion mq " +
                "JOIN Answers a ON mq.questionId = a.questionID";

        try (Connection connection = DatabaseConnector.getConnection();
             PreparedStatement preparedStatement = connection.prepareStatement(query);
             ResultSet resultSet = preparedStatement.executeQuery()) {

            // Process the result set
            while (resultSet.next()) {
                // Retrieve each column value
                String questionDescription = resultSet.getString("question");
                String answerOption = resultSet.getString("answer_option");
                String answerStatus = resultSet.getString("answer_status");

                // Print or process the information
                System.out.println("Question: " + questionDescription);
                System.out.println("Answer Option: " + answerOption);
                System.out.println("Answer Status: " + answerStatus);
                System.out.println("-----------------------------------");
            }
        } catch (Exception e) {
            e.printStackTrace();
        }
    }

    public static void main(String[] args) {
        // Call the method to retrieve and display question information
        retrieveQuestionInfo();
    }
}