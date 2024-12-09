package javaClasses.databaseClasses;

import java.io.FileInputStream;
import java.sql.Connection;
import java.sql.DriverManager;
import java.sql.SQLException;
import java.util.Properties;
import java.io.InputStream;

public class DatabaseConnector {
    private static final Properties databaseProperties = new Properties();
    public static Connection getConnection() {
        Connection connection = null;
        try{
            Class.forName("com.mysql.cj.jdbc.Driver");

            InputStream input = new FileInputStream("database.properties");
            //InputStream input = new FileInputStream("test.properties"); // Test properties
            databaseProperties.load(input);
            if (input == null) {
                System.out.println("Sorry, unable to find database.properties");
                return null;
            }
            databaseProperties.load(input);

            String url = databaseProperties.getProperty("db.url");
            //jdbc:mysql://localhost:3306/?user=root
            String username = databaseProperties.getProperty("db.username");
            String password = databaseProperties.getProperty("db.password");

            connection = DriverManager.getConnection(url, username, password);
            System.out.println("Connection established successfully.");
        } catch (ClassNotFoundException e) {
            System.out.println("MySQL JDBC Driver not found: " + e.getMessage());
        } catch (SQLException e) {
            System.out.println("Error connecting to the database: " + e.getMessage());
        } catch (Exception e) {
            e.printStackTrace();
        }
        return connection;
    }
}