package com.avp.config.dao.user;

import com.avp.config.entity.user.PermGroupStationDef;
import org.springframework.data.jpa.repository.JpaRepository;
import org.springframework.data.jpa.repository.JpaSpecificationExecutor;

/**
 * Created by zhoujs on 2018/6/26.
 */
public interface PermGroupStationDefRepository extends JpaRepository<PermGroupStationDef, String>, JpaSpecificationExecutor<PermGroupStationDef> {

    public void deleteByRelationId(String relationId);
}
