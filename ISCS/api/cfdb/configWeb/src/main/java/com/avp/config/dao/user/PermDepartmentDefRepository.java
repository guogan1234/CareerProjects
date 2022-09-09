package com.avp.config.dao.user;

import com.avp.config.entity.user.PermDepartmentDef;
import org.springframework.data.jpa.repository.JpaRepository;
import org.springframework.data.jpa.repository.JpaSpecificationExecutor;

import java.util.List;

/**
 * Created by zhoujs on 2018/6/21.
 */
public interface PermDepartmentDefRepository extends JpaRepository<PermDepartmentDef, Integer>, JpaSpecificationExecutor<PermDepartmentDef> {

    public PermDepartmentDef save(PermDepartmentDef permDepartmentDef);

    public PermDepartmentDef findByDeptId(Integer deptId);

    public List<PermDepartmentDef> findByDeptName(String deptName);

    public List<PermDepartmentDef> findByDeptIdOrDeptName(Integer deptId, String deptName);
}
