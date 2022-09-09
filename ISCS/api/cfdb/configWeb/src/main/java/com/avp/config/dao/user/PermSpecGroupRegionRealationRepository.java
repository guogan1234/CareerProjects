package com.avp.config.dao.user;

import com.avp.config.entity.user.PermSpecGroupRegionRealation;
import org.springframework.data.jpa.repository.JpaRepository;
import org.springframework.data.jpa.repository.JpaSpecificationExecutor;

/**
 * Created by zhoujs on 2018/6/28.
 */
public interface PermSpecGroupRegionRealationRepository extends JpaRepository<PermSpecGroupRegionRealation, String>, JpaSpecificationExecutor<PermSpecGroupRegionRealation> {

    public void deleteByRelationId(String relationId);
}
