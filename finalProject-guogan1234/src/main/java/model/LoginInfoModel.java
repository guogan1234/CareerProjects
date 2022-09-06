package model;

import data.LoginInfo;

/**
 * An interface class to operate LoginInfo with database
 */
public interface LoginInfoModel {
    /**
     * Add a login info
     * @param info A LoginInfo class contains login info
     * @return The operated row count
     */
    int add(LoginInfo info);

    /**
     * Find login info by username
     * @param username username
     * @return login info with specific username
     */
    LoginInfo findByUserOrderByLoginTime(String username);
}
