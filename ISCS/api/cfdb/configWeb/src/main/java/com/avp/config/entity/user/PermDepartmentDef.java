package com.avp.config.entity.user;

import javax.persistence.Entity;
import javax.persistence.Id;
import javax.persistence.Table;

@Entity
@Table(name = "perm_department_def",schema = "conf")
public class PermDepartmentDef {
    @Id
    private  Integer deptId;
    private String deptName;
    private Integer delongDeptId;
    private String deptDesc;

    public Integer getDeptId() {
        return deptId;
    }

    public void setDeptId(Integer deptId) {
        this.deptId = deptId;
    }

    public String getDeptName() {
        return deptName;
    }

    public void setDeptName(String deptName) {
        this.deptName = deptName;
    }

    public Integer getDelongDeptId() {
        return delongDeptId;
    }

    public void setDelongDeptId(Integer delongDeptId) {
        this.delongDeptId = delongDeptId;
    }

    public String getDeptDesc() {
        return deptDesc;
    }

    public void setDeptDesc(String deptDesc) {
        this.deptDesc = deptDesc;
    }
}
