package utils;

import java.sql.Connection;
import java.sql.PreparedStatement;
import java.sql.ResultSet;
import java.sql.SQLException;

/**
 * A class uses jdbc to operate database
 */
public class JdbcTemplate {
    private static Connection connection = null;
    private PreparedStatement statement = null;

    /**
     * Constructor
     */
    public JdbcTemplate(){
        connection = JdbcUtils.getConnection();
    }

    /**
     * Set auto commit operations to database
     * @param autoCommit auto commit flag
     */
    public void setAutoCommit(boolean autoCommit){
        try {
            if(connection != null) connection.setAutoCommit(autoCommit);
        } catch (SQLException e) {
            e.printStackTrace();
        }
    }

    /**
     * Commit operations to database
     */
    public void commit(){
        try {
            if(statement != null) statement.executeBatch();
            if(connection != null) connection.commit();
        } catch (SQLException e) {
            e.printStackTrace();
        }finally {
//            JdbcUtils.close(null, statement, connection);
        }
    }

    /**
     * Add data to database batched
     * @param sql SQL statement
     * @param args arguments
     */
    public void addBatch(String sql, Object... args){
        try {
            if(connection == null) connection = JdbcUtils.getConnection();
            if(statement == null) statement = connection.prepareStatement(sql);
            if(args != null){
                for(int i = 0; i < args.length; i++) {
                    statement.setObject(i + 1, args[i]);
                }
            }
            statement.addBatch();
        } catch (SQLException e) {
            e.printStackTrace();
        }
    }

    /**
     * Update records in database
     * @param sql SQL statement
     * @param args arguments
     * @return The operated row count by this SQL statement
     */
    public int update(String sql, Object... args){
        try {
            if(connection == null) connection = JdbcUtils.getConnection();
            statement = connection.prepareStatement(sql);
            if (args != null) {
                for (int i = 0; i < args.length; i++) {
                    statement.setObject(i + 1, args[i]);
                }
            }
            return statement.executeUpdate();
        }catch (SQLException e){
            e.printStackTrace();
        }finally {
//            JdbcUtils.close(null, statement, connection);
        }
        return -1;
    }

    /**
     * Query records from database
     * @param sql SQL statement
     * @param handler a class handles result set
     * @param args arguments
     * @return a object contains the info of result set
     */
    public Object query(String sql, EntityHandler handler, Object... args){
        ResultSet resultSet = null;
        try{
            if(connection == null) connection = JdbcUtils.getConnection();
            statement = connection.prepareStatement(sql);
            if(args != null){
                for (int i = 0; i < args.length; i++) {
                    statement.setObject(i + 1, args[i]);
                }
            }
            resultSet = statement.executeQuery();
            return handler.handle(resultSet);
        }catch (SQLException e){
            e.printStackTrace();
        }finally {
//            JdbcUtils.close(resultSet, statement, connection);
        }
        return null;
    }
}
