package model.manager;

import data.LoginInfo;
import model.LoginInfoModel;
import utils.EntityHandler;
import utils.JdbcTemplate;

import java.sql.ResultSet;
import java.sql.SQLException;

/**
 * A class manages database operations about login info
 */
public class LoginInfoManager implements LoginInfoModel {
    private static JdbcTemplate jdbcTemplate = new JdbcTemplate();

    /**
     * Constructor
     */
    public LoginInfoManager(){
//        jdbcTemplate = new JdbcTemplate();
    }

    /**
     * Add a login info
     * @param info A LoginInfo class contains login info
     * @return The operated row count
     */
    @Override
    public int add(LoginInfo info) {
        if(info == null) return -1;
        String sql = "insert into login_info(username, loginTime, dateString) values(?, ?, ?)";
        return jdbcTemplate.update(sql, info.getUsername(), info.getLoginTime(), info.getLogoutTime());
    }

    /**
     * Find login info by username
     * @param username username
     * @return login info with specific username
     */
    @Override
    public LoginInfo findByUserOrderByLoginTime(String username) {
        if(username == null) return null;
        String sql = "select * from login_info where username = ? order by loginTime desc";
        return (LoginInfo) jdbcTemplate.query(sql, new EntityHandler() {
            @Override
            public Object handle(ResultSet resultSet) throws SQLException {
                LoginInfo info = null;
                if(resultSet.next()){
                    info = new LoginInfo();
                    info.setId(resultSet.getInt(1));
                    info.setUsername(resultSet.getString(2));
                    info.setLoginTime(resultSet.getString(3));
                    info.setLogoutTime(resultSet.getString(4));
                }
                return info;
            }
        }, username);
    }
}
