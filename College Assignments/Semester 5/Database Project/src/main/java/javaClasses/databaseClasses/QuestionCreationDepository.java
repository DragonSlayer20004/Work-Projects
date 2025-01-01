package javaClasses.databaseClasses;

import java.sql.Connection;
import java.sql.PreparedStatement;
import java.sql.SQLException;

public class QuestionCreationDepository {

    public static boolean createMultichoiceQuestion(int questionID, String questionDescription, int multichoiceOptions, int correctAnswer) {
        String insertQuestionQuery = "INSERT INTO Questions (questionID, questionTypeID) VALUES (?, ?)";
        String insertMultichoiceQuery = "INSERT INTO MultichoiceQuestion (questionId, questionDescription, multichoiceOptions, correctAnswer) VALUES (?, ?, ?, ?)";

        try (Connection connection = DatabaseConnector.getConnection();
             PreparedStatement questionStatement = connection.prepareStatement(insertQuestionQuery);
             PreparedStatement multichoiceStatement = connection.prepareStatement(insertMultichoiceQuery)) {

            // Insert into Questions table
            questionStatement.setInt(1, questionID);
            questionStatement.setInt(2, 1); // Assuming 1 for multichoice type
            questionStatement.executeUpdate();

            // Insert into MultichoiceQuestion table
            multichoiceStatement.setInt(1, questionID);
            multichoiceStatement.setString(2, questionDescription);
            multichoiceStatement.setInt(3, multichoiceOptions);
            multichoiceStatement.setInt(4, correctAnswer);
            multichoiceStatement.executeUpdate();

            return true;

        } catch (SQLException e) {
            System.err.println("Error creating multichoice question: " + e.getMessage());
            return false;
        }
    }

    public static boolean createFillInTheBlankQuestion(int questionID, String questionDescription, String correctAnswer) {
        String insertQuestionQuery = "INSERT INTO Questions (questionID, questionTypeID) VALUES (?, ?)";
        String insertFillInTheBlankQuery = "INSERT INTO FillInTheBlankQuestion (questionId, questionDescription, correctAnswer) VALUES (?, ?, ?)";

        try (Connection connection = DatabaseConnector.getConnection();
             PreparedStatement questionStatement = connection.prepareStatement(insertQuestionQuery);
             PreparedStatement fillInTheBlankStatement = connection.prepareStatement(insertFillInTheBlankQuery)) {

            // Insert into Questions table
            questionStatement.setInt(1, questionID);
            questionStatement.setInt(2, 2); // Assuming 2 for fill-in-the-blank type
            questionStatement.executeUpdate();

            // Insert into FillInTheBlankQuestion table
            fillInTheBlankStatement.setInt(1, questionID);
            fillInTheBlankStatement.setString(2, questionDescription);
            fillInTheBlankStatement.setString(3, correctAnswer);
            fillInTheBlankStatement.executeUpdate();

            return true;

        } catch (SQLException e) {
            System.err.println("Error creating fill-in-the-blank question: " + e.getMessage());
            return false;
        }
    }

    public static boolean createDragAndDropQuestion(int questionID, String questionDescription, int correctAnswer, int wrongAnswerOne, int wrongAnswerTwo, int wrongAnswerThree) {
        String insertQuestionQuery = "INSERT INTO Questions (questionID, questionTypeID) VALUES (?, ?)";
        String insertDragAndDropQuery = "INSERT INTO DragAndDropQuestion (questionId, questionDescription, correctAnswer, WrongAnswerOne, WrongAnswerTwo, WrongAnswerThree) VALUES (?, ?, ?, ?, ?, ?)";

        try (Connection connection = DatabaseConnector.getConnection();
             PreparedStatement questionStatement = connection.prepareStatement(insertQuestionQuery);
             PreparedStatement dragAndDropStatement = connection.prepareStatement(insertDragAndDropQuery)) {

            // Insert into Questions table
            questionStatement.setInt(1, questionID);
            questionStatement.setInt(2, 3); // Assuming 3 for drag-and-drop type
            questionStatement.executeUpdate();

            // Insert into DragAndDropQuestion table
            dragAndDropStatement.setInt(1, questionID);
            dragAndDropStatement.setString(2, questionDescription);
            dragAndDropStatement.setInt(3, correctAnswer);
            dragAndDropStatement.setInt(4, wrongAnswerOne);
            dragAndDropStatement.setInt(5, wrongAnswerTwo);
            dragAndDropStatement.setInt(6, wrongAnswerThree);
            dragAndDropStatement.executeUpdate();

            return true;

        } catch (SQLException e) {
            System.err.println("Error creating drag-and-drop question: " + e.getMessage());
            return false;
        }
    }
}
