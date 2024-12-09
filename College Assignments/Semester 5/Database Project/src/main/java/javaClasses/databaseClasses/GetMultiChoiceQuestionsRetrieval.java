package javaClasses.databaseClasses;

import java.sql.Connection;
import java.sql.PreparedStatement;
import java.sql.ResultSet;
import org.json.JSONArray;
import org.json.JSONObject;

public class GetMultiChoiceQuestionsRetrieval {

    public static String GetQuestions(int questionID) {
        // SQL query to retrieve multiple-choice questions and their answers
        String query = "SELECT q.questionID, q.questionTypeID, " +
                "mq.questionDescription AS multiChoiceDesc, mq.multichoiceOptions, mq.correctAnswer AS multiChoiceCorrect, " +
                "a.answerID, a.answerDescription " +
                "FROM Questions q " +
                "LEFT JOIN MultichoiceQuestion mq ON q.questionID = mq.questionId " +
                "LEFT JOIN Answers a ON q.questionID = a.questionID " +
                "WHERE q.questionTypeID = ? ";
 //               "ORDER BY a.answerID";  // Filter only for multiple-choice questions

        // JSON array to store all multiple-choice questions with their answers
        JSONArray quizQuestionsJson = new JSONArray();

        try (Connection connection = DatabaseConnector.getConnection();
             PreparedStatement preparedStatement = connection.prepareStatement(query)) {

            // Set the rockNumber parameter
            preparedStatement.setInt(1, questionID);

            // Execute the query
            ResultSet resultSet = preparedStatement.executeQuery();

            // Process the result set
            while (resultSet.next()) {
                int _questionID = resultSet.getInt("questionID");
                JSONObject questionJson = new JSONObject();

                questionJson.put("questionID", _questionID);
                questionJson.put("type", "multichoice");
                questionJson.put("description", resultSet.getString("multiChoiceDesc"));
                questionJson.put("optionsCount", resultSet.getInt("multichoiceOptions"));
                questionJson.put("correctAnswer", resultSet.getInt("multiChoiceCorrect"));

                // Initialize the answers array
                JSONArray answersArray = new JSONArray();

                // Collect all answers for this question
                do {
                    // Check if the current row corresponds to the same question
                    if (resultSet.getInt("questionID") == _questionID) {
                        JSONObject answerJson = new JSONObject();
                        answerJson.put("answerID", resultSet.getInt("answerID"));
                        answerJson.put("answerDescription", resultSet.getString("answerDescription"));
                        answersArray.put(answerJson);
                    } else {
                        break;  // Exit if we reached the next question
                    }
                } while (resultSet.next()); // Move to the next row

                questionJson.put("answers", answersArray);

                // Add the question JSON to the array
                quizQuestionsJson.put(questionJson);

                // Move the cursor back to the previous row after the inner loop
                //resultSet.previous();
            }

        } catch (Exception e) {
            System.out.println("Error retrieving quiz questions: " + e.getMessage());
            e.printStackTrace();
        }

        // Convert the JSON array to a JSON-formatted string
        return quizQuestionsJson.toString();
    }
}
