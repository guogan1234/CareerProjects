package com.avp.config.dao.user;

import com.avp.config.entity.user.PermGroupDef;
import org.springframework.data.jpa.repository.JpaRepository;
import org.springframework.data.jpa.repository.JpaSpecificationExecutor;

import java.util.List;

/**
 * Created by zhoujs on 2018/6/22.
 */
public interface PermGroupDefRepository extends JpaRepository<PermGroupDef, Integer>, JpaSpecificationExecutor<PermGroupDef> {

    public PermGroupDef save(PermGroupDef permGroupDef);

    public PermGroupDef findByGroupId(Integer groupId);

    public List<PermGroupDef> findByGroupName(String groupName);

    public List<PermGroupDef> findByGroupIdOrGroupName(Integer groupId, String groupName);
}
