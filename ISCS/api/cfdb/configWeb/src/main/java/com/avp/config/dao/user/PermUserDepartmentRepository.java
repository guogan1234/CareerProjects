package com.avp.config.dao.user;

import com.avp.config.entity.user.PermUserDepartmentDef;
import org.springframework.data.jpa.repository.JpaRepository;
import org.springframework.data.jpa.repository.JpaSpecificationExecutor;

import java.util.List;

public interface PermUserDepartmentRepository extends JpaRepository<PermUserDepartmentDef,String>,JpaSpecificationExecutor<PermUserDepartmentDef> {
    public void deleteByRelationId(String relationId);
}
