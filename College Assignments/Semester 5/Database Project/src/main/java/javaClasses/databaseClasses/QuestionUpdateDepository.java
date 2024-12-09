package javaClasses.databaseClasses;

import java.sql.Connection;
import java.sql.PreparedStatement;
import java.sql.SQLException;

public class QuestionUpdateDepository {

    public static boolean updateMultichoiceQuestion(int questionID, String questionDescription, int multichoiceOptions, int correctAnswer) {
        String updateQuestionQuery = "UPDATE Questions SET questionTypeID = ? WHERE questionID = ?";
        String updateMultichoiceQuery = "UPDATE MultichoiceQuestion SET questionDescription = ?, multichoiceOptions = ?, correctAnswer = ? WHERE questionId = ?";

        try (Connection connection = DatabaseConnector.getConnection();
             PreparedStatement questionStatement = connection.prepareStatement(updateQuestionQuery);
             PreparedStatement multichoiceStatement = connection.prepareStatement(updateMultichoiceQuery)) {

            // Update the Questions table
            questionStatement.setInt(1, 1); // Assuming 1 for multichoice type
            questionStatement.setInt(2, questionID);
            questionStatement.executeUpdate();

            // Update the MultichoiceQuestion table
            multichoiceStatement.setString(1, questionDescription);
            multichoiceStatement.setInt(2, multichoiceOptions);
            multichoiceStatement.setInt(3, correctAnswer);
            multichoiceStatement.setInt(4, questionID);
            multichoiceStatement.executeUpdate();

            return true;

        } catch (SQLException e) {
            System.err.println("Error updating multichoice question: " + e.getMessage());
            return false;
        }
    }

    public static boolean updateFillInTheBlankQuestion(int questionID, String questionDescription, String correctAnswer) {
        String updateQuestionQuery = "UPDATE Questions SET questionTypeID = ? WHERE questionID = ?";
        String updateFillInTheBlankQuery = "UPDATE FillInTheBlankQuestion SET questionDescription = ?, correctAnswer = ? WHERE questionId = ?";

        try (Connection connection = DatabaseConnector.getConnection();
             PreparedStatement questionStatement = connection.prepareStatement(updateQuestionQuery);
             PreparedStatement fillInTheBlankStatement = connection.prepareStatement(updateFillInTheBlankQuery)) {

            // Update the Questions table
            questionStatement.setInt(1, 2); // Assuming 2 for fill-in-the-blank type
            questionStatement.setInt(2, questionID);
            questionStatement.executeUpdate();

            // Update the FillInTheBlankQuestion table
            fillInTheBlankStatement.setString(1, questionDescription);
            fillInTheBlankStatement.setString(2, correctAnswer);
            fillInTheBlankStatement.setInt(3, questionID);
            fillInTheBlankStatement.executeUpdate();

            return true;

        } catch (SQLException e) {
            System.err.println("Error updating fill-in-the-blank question: " + e.getMessage());
            return false;
        }
    }

    public static boolean updateDragAndDropQuestion(int questionID, String questionDescription, int correctAnswer, int wrongAnswerOne, int wrongAnswerTwo, int wrongAnswerThree) {
        String updateQuestionQuery = "UPDATE Questions SET questionTypeID = ? WHERE questionID = ?";
        String updateDragAndDropQuery = "UPDATE DragAndDropQuestion SET questionDescription = ?, correctAnswer = ?, WrongAnswerOne = ?, WrongAnswerTwo = ?, WrongAnswerThree = ? WHERE questionId = ?";

        try (Connection connection = DatabaseConnector.getConnection();
             PreparedStatement questionStatement = connection.prepareStatement(updateQuestionQuery);
             PreparedStatement dragAndDropStatement = connection.prepareStatement(updateDragAndDropQuery)) {

            // Update the Questions table
            questionStatement.setInt(1, 3); // Assuming 3 for drag-and-drop type
            questionStatement.setInt(2, questionID);
            questionStatement.executeUpdate();

            // Update the DragAndDropQuestion table
            dragAndDropStatement.setString(1, questionDescription);
            dragAndDropStatement.setInt(2, correctAnswer);
            dragAndDropStatement.setInt(3, wrongAnswerOne);
            dragAndDropStatement.setInt(4, wrongAnswerTwo);
            dragAndDropStatement.setInt(5, wrongAnswerThree);
            dragAndDropStatement.setInt(6, questionID);
            dragAndDropStatement.executeUpdate();

            return true;

        } catch (SQLException e) {
            System.err.println("Error updating drag-and-drop question: " + e.getMessage());
            return false;
        }
    }
}
