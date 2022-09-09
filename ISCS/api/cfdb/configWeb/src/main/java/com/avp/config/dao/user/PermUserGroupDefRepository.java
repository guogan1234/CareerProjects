package com.avp.config.dao.user;

import com.avp.config.entity.user.PermUserGroupDef;
import org.springframework.data.jpa.repository.JpaRepository;
import org.springframework.data.jpa.repository.JpaSpecificationExecutor;

import java.util.List;

/**
 * Created by zhoujs on 2018/6/26.
 */
public interface PermUserGroupDefRepository extends JpaRepository<PermUserGroupDef, String>, JpaSpecificationExecutor<PermUserGroupDef> {
    public void deleteByRelationId(String relationId);

    public List<PermUserGroupDef> findByGroupIdAndIfLead(Integer groupId, Integer ifLead);

    public PermUserGroupDef findByRelationId(String relationId);
}
