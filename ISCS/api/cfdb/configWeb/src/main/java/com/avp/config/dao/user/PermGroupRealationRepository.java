package com.avp.config.dao.user;

import com.avp.config.entity.user.PermGroupRealation;
import org.springframework.data.jpa.repository.JpaRepository;
import org.springframework.data.jpa.repository.JpaSpecificationExecutor;
import java.util.List;

public interface PermGroupRealationRepository extends JpaRepository<PermGroupRealation,String>,JpaSpecificationExecutor<PermGroupRealation> {
    public void deleteByRelationId(String relationId);
}
