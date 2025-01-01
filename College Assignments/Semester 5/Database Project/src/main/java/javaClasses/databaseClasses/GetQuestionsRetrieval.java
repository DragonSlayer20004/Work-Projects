package javaClasses.databaseClasses;

import java.sql.Connection;
import java.sql.PreparedStatement;
import java.sql.ResultSet;
import org.json.JSONArray;
import org.json.JSONObject;

public class GetQuestionsRetrieval {

    public static String GetQuestions() {
        // SQL query to retrieve quiz questions from the database
        String query = "SELECT q.questionID, q.questionTypeID, " +
                "mq.questionDescription AS multiChoiceDesc, mq.multichoiceOptions, mq.correctAnswer AS multiChoiceCorrect, " +
                "fibq.questionDescription AS fillBlankDesc, fibq.correctAnswer AS fillBlankCorrect, " +
                "ddq.questionDescription AS dragDropDesc, ddq.correctAnswer AS dragDropCorrect, ddq.WrongAnswerOne, ddq.WrongAnswerTwo, ddq.WrongAnswerThree " +
                "FROM Questions q " +
                "LEFT JOIN MultichoiceQuestion mq ON q.questionID = mq.questionId " +
                "LEFT JOIN FillInTheBlankQuestion fibq ON q.questionID = fibq.questionId " +
                "LEFT JOIN DragAndDropQuestion ddq ON q.questionID = ddq.questionId";

        // JSON array to store all questions
        JSONArray quizQuestionsJson = new JSONArray();

        try (Connection connection = DatabaseConnector.getConnection();
             PreparedStatement preparedStatement = connection.prepareStatement(query)) {

            // Execute the query
            ResultSet resultSet = preparedStatement.executeQuery();

            // Process the result set
            while (resultSet.next()) {
                int questionID = resultSet.getInt("questionID");
                int questionTypeID = resultSet.getInt("questionTypeID");
                JSONObject questionJson = new JSONObject();

                questionJson.put("questionID", questionID);
                questionJson.put("questionTypeID", questionTypeID);

                switch (questionTypeID) {
                    case 1: // Multichoice Question
                        questionJson.put("type", "multichoice");
                        questionJson.put("description", resultSet.getString("multiChoiceDesc"));
                        questionJson.put("optionsCount", resultSet.getInt("multichoiceOptions"));
                        questionJson.put("correctAnswer", resultSet.getInt("multiChoiceCorrect"));
                        break;

                    case 2: // Fill-in-the-Blank Question
                        questionJson.put("type", "fillintheblank");
                        questionJson.put("description", resultSet.getString("fillBlankDesc"));
                        questionJson.put("correctAnswer", resultSet.getString("fillBlankCorrect"));
                        break;

                    case 3: // Drag-and-Drop Question
                        questionJson.put("type", "draganddrop");
                        questionJson.put("description", resultSet.getString("dragDropDesc"));
                        questionJson.put("correctAnswer", resultSet.getInt("dragDropCorrect"));
                        questionJson.put("wrongAnswerOne", resultSet.getString("WrongAnswerOne"));
                        questionJson.put("wrongAnswerTwo", resultSet.getString("WrongAnswerTwo"));
                        questionJson.put("wrongAnswerThree", resultSet.getString("WrongAnswerThree"));
                        break;

                    default:
                        System.out.println("Unknown question type: " + questionTypeID);
                        continue; // Skip unknown types
                }

                // Add the JSON object to the array
                quizQuestionsJson.put(questionJson);
            }

        } catch (Exception e) {
            System.out.println("Error retrieving quiz questions: " + e.getMessage());
            e.printStackTrace();
        }

        // Convert the JSON array to a JSON-formatted string
        return quizQuestionsJson.toString();
    }
}
