package data;

/**
 * A class contains user password info
 */
public class User {
    private int id;
    private String name;
    private String saltedPassword;
    private String salt;

    public int getId() {
        return id;
    }

    public void setId(int id) {
        this.id = id;
    }

    public String getName() {
        return name;
    }

    public void setName(String name) {
        this.name = name;
    }

    public String getSaltedPassword() {
        return saltedPassword;
    }

    public void setSaltedPassword(String saltedPassword) {
        this.saltedPassword = saltedPassword;
    }

    public String getSalt() {
        return salt;
    }

    public void setSalt(String salt) {
        this.salt = salt;
    }
}
