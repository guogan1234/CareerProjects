package com.avp.config.dao.user.vw;

import com.avp.config.entity.user.vw.PermGroupRealationVw;
import org.springframework.data.jpa.repository.JpaRepository;
import org.springframework.data.jpa.repository.JpaSpecificationExecutor;

public interface PermGroupRealationVwRepository extends JpaRepository<PermGroupRealationVw, Integer>, JpaSpecificationExecutor<PermGroupRealationVw> {
}
