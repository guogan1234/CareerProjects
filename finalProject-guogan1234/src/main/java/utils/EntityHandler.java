package utils;

import java.sql.ResultSet;
import java.sql.SQLException;

/**
 * An interface that handles database result Set
 */
public interface EntityHandler {
    Object handle(ResultSet resultSet) throws SQLException;
}
