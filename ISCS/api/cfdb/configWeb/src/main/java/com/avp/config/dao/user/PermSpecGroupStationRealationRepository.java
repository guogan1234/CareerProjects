package com.avp.config.dao.user;

import com.avp.config.entity.user.PermSpecGroupStationRealation;
import org.springframework.data.jpa.repository.JpaRepository;
import org.springframework.data.jpa.repository.JpaSpecificationExecutor;

/**
 * Created by zhoujs on 2018/6/28.
 */
public interface PermSpecGroupStationRealationRepository extends JpaRepository<PermSpecGroupStationRealation, String>, JpaSpecificationExecutor<PermSpecGroupStationRealation> {

    public void deleteByRelationId(String relationId);
}
