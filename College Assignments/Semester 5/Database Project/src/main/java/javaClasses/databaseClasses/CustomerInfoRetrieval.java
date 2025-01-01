package javaClasses.databaseClasses;

import java.sql.Connection;
import java.sql.PreparedStatement;
import java.sql.ResultSet;

public class CustomerInfoRetrieval {

    public static void getCustomerInfoById(int customerId) {
        // SQL query to retrieve customer and order details
        String query = "SELECT c.CustomerID, c.FirstName, c.LastName, c.BillingAddress, " +
                "o.TicketPrice, o.Quantity, be.EventName, be.EventDate, be.EventLength " +
                "FROM Customer c " +
                "JOIN Orders o ON c.CustomerID = o.CustomerID " +
                "JOIN BusinessEvents be ON o.EventID = be.EventID " +
                "WHERE c.CustomerID = ?";

        try (Connection connection = DatabaseConnector.getConnection();
             PreparedStatement preparedStatement = connection.prepareStatement(query)) {

            // Set the CustomerID parameter in the query
            preparedStatement.setInt(1, customerId);

            // Execute the query
            ResultSet resultSet = preparedStatement.executeQuery();

            // Process the result set
            while (resultSet.next()) {
                int id = resultSet.getInt("CustomerID");
                String firstName = resultSet.getString("FirstName");
                String lastName = resultSet.getString("LastName");
                String billingAddress = resultSet.getString("BillingAddress");
                double ticketPrice = resultSet.getDouble("TicketPrice");
                int quantity = resultSet.getInt("Quantity");
                String eventName = resultSet.getString("EventName");
                String eventDate = resultSet.getString("EventDate");
                int eventLength = resultSet.getInt("EventLength");

                // Print customer and event information
                System.out.println("Customer ID: " + id);
                System.out.println("Name: " + firstName + " " + lastName);
                System.out.println("Billing Address: " + billingAddress);
                System.out.println("Event: " + eventName);
                System.out.println("Event Date: " + eventDate);
                System.out.println("Event Length: " + eventLength + " minutes");
                System.out.println("Ticket Price: $" + ticketPrice);
                System.out.println("Quantity: " + quantity);
                System.out.println("Total Price: $" + (ticketPrice * quantity));
            }
        } catch (Exception e) {
            e.printStackTrace();
        }
    }

    public static void main(String[] args) {
        // Example CustomerID to search for
        int customerId = 1;
        getCustomerInfoById(customerId);
    }
}