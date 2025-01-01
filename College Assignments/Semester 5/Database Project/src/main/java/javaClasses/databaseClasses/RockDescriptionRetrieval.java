package javaClasses.databaseClasses;

import java.sql.Connection;
import java.sql.PreparedStatement;
import java.sql.ResultSet;

public class RockDescriptionRetrieval {
    public static String GetRockInfoById(int rockNumber) {
        // SQL query to retrieve customer and order details
        String query = "SELECT RockDescription FROM Rock WHERE RockID = ?";

        try (Connection connection = DatabaseConnector.getConnection();
             PreparedStatement preparedStatement = connection.prepareStatement(query)) {

            // Set the rockNumber parameter
            preparedStatement.setInt(1, rockNumber);

            // Execute the query
            ResultSet resultSet = preparedStatement.executeQuery();

            // Process the result set
            while (resultSet.next()) {
                // Retrieve and print rock information
                String rockDescription = resultSet.getString("RockDescription");
                return rockDescription;
            }
        } catch (Exception e) {
            System.out.println("Error retrieving rock information: " + e.getMessage());
            e.printStackTrace();
        }
        return "Error retrieving rock information: " + rockNumber;
    }
}
