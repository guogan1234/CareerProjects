package com.avp.config.entity.user;

import javax.persistence.Entity;
import javax.persistence.Id;
import javax.persistence.Table;
@Entity
@Table(name = "perm_security_def",schema = "conf")
public class PermSecurityDef {
    @Id
    private Integer permSecurityId;
    private Integer permPasswordMinLength;
    private Integer permPasswordMinLetr;
    private Integer permPasswordMinNum;
    private Integer permAccountMaxLock;
    private Integer permAccountLockTimer;
    private Integer permAccountInvalidTimer;

    public Integer getPermSecurityId() {
        return permSecurityId;
    }

    public void setPermSecurityId(Integer permSecurityId) {
        this.permSecurityId = permSecurityId;
    }

    public Integer getPermPasswordMinLength() {
        return permPasswordMinLength;
    }

    public void setPermPasswordMinLength(Integer permPasswordMinLength) {
        this.permPasswordMinLength = permPasswordMinLength;
    }

    public Integer getPermPasswordMinLetr() {
        return permPasswordMinLetr;
    }

    public void setPermPasswordMinLetr(Integer permPasswordMinLetr) {
        this.permPasswordMinLetr = permPasswordMinLetr;
    }

    public Integer getPermPasswordMinNum() {
        return permPasswordMinNum;
    }

    public void setPermPasswordMinNum(Integer permPasswordMinNum) {
        this.permPasswordMinNum = permPasswordMinNum;
    }

    public Integer getPermAccountMaxLock() {
        return permAccountMaxLock;
    }

    public void setPermAccountMaxLock(Integer permAccountMaxLock) {
        this.permAccountMaxLock = permAccountMaxLock;
    }

    public Integer getPermAccountLockTimer() {
        return permAccountLockTimer;
    }

    public void setPermAccountLockTimer(Integer permAccountLockTimer) {
        this.permAccountLockTimer = permAccountLockTimer;
    }

    public Integer getPermAccountInvalidTimer() {
        return permAccountInvalidTimer;
    }

    public void setPermAccountInvalidTimer(Integer permAccountInvalidTimer) {
        this.permAccountInvalidTimer = permAccountInvalidTimer;
    }
}
