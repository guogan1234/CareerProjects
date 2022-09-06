package utils;

import java.sql.*;
import java.util.ResourceBundle;

/**
 * A util class that operate database by jdbc
 */
public class JdbcUtils {
    private static String url;
    private static String username;
    private static String password;
    private static String driver;

    static{
        loadConfig();
//        try {
//            Class.forName(driver);
//        } catch (ClassNotFoundException e) {
//            e.printStackTrace();
//        }
    }

    /**
     * Load config from properties file
     */
    private static void loadConfig(){
        ResourceBundle bundle = ResourceBundle.getBundle("database");
        if(bundle == null){
            System.out.println("[ResourceBundle: load config failed!]");
        }
        url = bundle.getString("jdbc.url");
        username = bundle.getString("jdbc.username");
        password = bundle.getString("jdbc.password");
        driver = bundle.getString("jdbc.driver");
    }

    /**
     * Get a connection from database
     * @return a connection between database and application
     */
    public static Connection getConnection(){
        Connection connection = null;
        try {
            connection = DriverManager.getConnection(url, username, password);
        } catch (SQLException e) {
            e.printStackTrace();
        }
        return connection;
    }

    /**
     * Close resources that are used between database and application
     * @param resultSet result set
     * @param statement statement
     * @param connection a connection between database and application
     */
    public static void close(ResultSet resultSet, Statement statement, Connection connection){
        try {
            if(resultSet != null) resultSet.close();
            if(statement != null) statement.close();
            if(connection != null) connection.close();
        } catch (SQLException e) {
            e.printStackTrace();
        }
    }
}
