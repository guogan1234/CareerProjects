package data;

/**
 * A class that contains login info
 */
public class LoginInfo {
    private int id;
    private String username;
    private String loginTime;
    private String logoutTime;

    public LoginInfo() {
    }

    public LoginInfo(int id, String username, String loginTime, String logoutTime) {
        this.id = id;
        this.username = username;
        this.loginTime = loginTime;
        this.logoutTime = logoutTime;
    }

    public int getId() {
        return id;
    }

    public void setId(int id) {
        this.id = id;
    }

    public String getUsername() {
        return username;
    }

    public void setUsername(String username) {
        this.username = username;
    }

    public String getLoginTime() {
        return loginTime;
    }

    public void setLoginTime(String loginTime) {
        this.loginTime = loginTime;
    }

    public String getLogoutTime() {
        return logoutTime;
    }

    public void setLogoutTime(String logoutTime) {
        this.logoutTime = logoutTime;
    }
}
