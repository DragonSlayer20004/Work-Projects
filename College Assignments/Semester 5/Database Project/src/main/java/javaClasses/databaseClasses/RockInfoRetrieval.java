package javaClasses.databaseClasses;

import java.sql.Connection;
import java.sql.PreparedStatement;
import java.sql.ResultSet;

public class RockInfoRetrieval {

    public static void GetFullRockInfoById(int customerId) {
        // SQL query to retrieve customer and order details
        String query = "SELECT * FROM Rock";

        try (Connection connection = DatabaseConnector.getConnection();
             PreparedStatement preparedStatement = connection.prepareStatement(query)) {

            // Execute the query
            ResultSet resultSet = preparedStatement.executeQuery();

            // Process the result set
            while (resultSet.next()) {
                // Retrieve and print rock information
                int rockId = resultSet.getInt("RockId");
                String rockName = resultSet.getString("RockName");
                String rockDescription = resultSet.getString("RockDescription");
                String rockColor = resultSet.getString("RockColor");
                String rockImage = resultSet.getString("RockImage");

                // Print rock details
                System.out.println("Rock ID: " + rockId);
                System.out.println("Rock Name: " + rockName);
                System.out.println("Description: " + rockDescription);
                System.out.println("Color: " + rockColor);
                System.out.println("Image: " + rockImage);
                System.out.println("-----------------------------");
            }
        } catch (Exception e) {
            System.out.println("Error retrieving rock information: " + e.getMessage());
            e.printStackTrace();
        }
    }

    public static void main(String[] args) {
        // Example rockId to search for
        int rockId = 0;
        GetFullRockInfoById(rockId);
    }
}