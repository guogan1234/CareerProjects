package com.avp.config.dao.user.vw;

import com.avp.config.entity.user.vw.UserDepartmentVw;
import org.springframework.data.jpa.repository.JpaRepository;
import org.springframework.data.jpa.repository.JpaSpecificationExecutor;

public interface UserDepartmentVwRepository extends JpaRepository<UserDepartmentVw, Integer>, JpaSpecificationExecutor<UserDepartmentVw> {

}
