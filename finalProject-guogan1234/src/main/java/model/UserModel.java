package model;

import data.User;

/**
 * An interface class to operate User with database
 */
public interface UserModel {
    /**
     * Add a user
     * @param user A User class contains user info
     * @return The operated row count
     */
    int add(User user);

    /**
     * Find user by name
     * @param name name
     * @return user with specific name
     */
    User findByName(String name);
}
