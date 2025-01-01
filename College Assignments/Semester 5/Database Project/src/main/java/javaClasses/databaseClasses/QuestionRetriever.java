package javaClasses.databaseClasses;

import java.sql.Connection;
import java.sql.PreparedStatement;
import java.sql.ResultSet;

public class QuestionRetriever {

    public static void GetQuestionById(int questionId) {
        // Query to retrieve the question and its options
        String questionQuery = "SELECT q.questionDescription, " +
                "mq.questionType, " +
                "mqo.answerDescription, " +
                "mqo.answerID, " +
                "mq.correctAnswer " +
                "FROM Questions q " +
                "JOIN MultichoiceQuestion mq ON q.multichoiceQuestion = mq.questionId " +
                "LEFT JOIN Answers mqo ON mq.questionId = mqo.answerID " +
                "WHERE q.questionID = ?";

        try (Connection connection = DatabaseConnector.getConnection();
             PreparedStatement preparedStatement = connection.prepareStatement(questionQuery)) {

            // Set the QuestionID parameter in the query
            preparedStatement.setInt(1, questionId);

            // Execute the query
            ResultSet resultSet = preparedStatement.executeQuery();

            // Process the result set
            if (resultSet.next()) {
                String questionDescription = resultSet.getString("questionDescription");
                String questionType = resultSet.getString("questionType");

                System.out.println("Question: " + questionDescription);
                System.out.println("Type: " + questionType);

                // Handle multiple choice options
                if (questionType.equalsIgnoreCase("MultipleChoice")) {
                    do {
                        String optionDescription = resultSet.getString("answerDescription");
                        int answerID = resultSet.getInt("answerID");
                        boolean isCorrect = answerID == resultSet.getInt("correctAnswer");
                        System.out.println("Option: " + optionDescription + (isCorrect ? " (Correct)" : ""));
                    } while (resultSet.next());
                }
            } else {
                System.out.println("Question not found for ID: " + questionId);
            }
        } catch (Exception e) {
            e.printStackTrace();
        }
    }

    public static void main(String[] args) {
        // Example questionId to search for
        int questionId = 1;
        GetQuestionById(questionId);
    }
}