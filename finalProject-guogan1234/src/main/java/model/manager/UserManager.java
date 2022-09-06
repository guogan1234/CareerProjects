package model.manager;

import data.User;
import model.UserModel;
import utils.EntityHandler;
import utils.JdbcTemplate;

import java.sql.ResultSet;
import java.sql.SQLException;

/**
 * A class manages database operations about user
 */
public class UserManager implements UserModel {
    private static JdbcTemplate jdbcTemplate = new JdbcTemplate();

    /**
     * Constructor
     */
    public UserManager(){
//        jdbcTemplate = new JdbcTemplate();
    }

    /**
     * Add a user
     * @param user A User class contains user info
     * @return The operated row count
     */
    @Override
    public int add(User user) {
        if(user == null) return 0;
        String sql = "insert into user(name, password, salt) values(?, ?, ?)";
        return jdbcTemplate.update(sql, user.getName(), user.getSaltedPassword(), user.getSalt());
    }

    /**
     * Find user by name
     * @param name name
     * @return user with specific name
     */
    @Override
    public User findByName(String name) {
        String sql = "select * from user where name=?";
        return (User) jdbcTemplate.query(sql, new EntityHandler() {
            @Override
            public Object handle(ResultSet resultSet) throws SQLException {
                User user = null;
                if(resultSet.next()){
                    user = new User();
                    user.setId(resultSet.getInt(1));
                    user.setName(resultSet.getString(2));
                    user.setSaltedPassword(resultSet.getString(3));
                    user.setSalt(resultSet.getString(4));
                }
                return user;
            }
        }, name);
    }
}
