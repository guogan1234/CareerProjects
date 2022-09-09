package com.avp.config.entity.user;

import com.fasterxml.jackson.annotation.JsonFormat;
import org.springframework.format.annotation.DateTimeFormat;
import javax.persistence.Entity;
import javax.persistence.Id;
import javax.persistence.Table;
import javax.persistence.Transient;
import java.sql.Date;
import java.sql.Timestamp;

@Entity
@Table(name = "perm_user_def",schema ="conf")
public class PermUserDef {
    @Id
    private Integer userId;
    private String userName;
    private String userAlias;
    private String userPassword;
    private Timestamp userCreateDate;
    private Timestamp userExpireDate;
    private String userDesc;
    private Integer userIfLock;
    private Timestamp  userLockTime;


    public Integer getUserId() {
        return userId;
    }

    public void setUserId(Integer userId) {
        this.userId = userId;
    }

    public String getUserName() {
        return userName;
    }

    public void setUserName(String userName) {
        this.userName = userName;
    }

    public String getUserAlias() {
        return userAlias;
    }

    public void setUserAlias(String userAlias) {
        this.userAlias = userAlias;
    }

    public String getUserPassword() {
        return userPassword;
    }

    public void setUserPassword(String userPassword) {
        this.userPassword = userPassword;
    }

    public String getUserDesc() {
        return userDesc;
    }

    public void setUserDesc(String userDesc) {
        this.userDesc = userDesc;
    }

    public Integer getUserIfLock() {
        return userIfLock;
    }

    public void setUserIfLock(Integer userIfLock) {
        this.userIfLock = userIfLock;
    }

    public Timestamp getUserCreateDate() {
        return userCreateDate;
    }

    public void setUserCreateDate(Timestamp userCreateDate) {
        this.userCreateDate = userCreateDate;
    }

    public Timestamp getUserExpireDate() {
        return userExpireDate;
    }

    public void setUserExpireDate(Timestamp userExpireDate) {
        this.userExpireDate = userExpireDate;
    }

    public Timestamp getUserLockTime() {
        return userLockTime;
    }

    public void setUserLockTime(Timestamp userLockTime) {
        this.userLockTime = userLockTime;
    }


}
