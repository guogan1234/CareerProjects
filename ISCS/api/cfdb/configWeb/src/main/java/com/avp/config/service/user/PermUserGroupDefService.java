package com.avp.config.service.user;

import com.avp.config.entity.user.PermUserGroupDef;
import com.avp.config.entity.user.vw.UserGroupVw;

import java.util.List;

/**
 * Created by zhoujs on 2018/6/26.
 */
public interface PermUserGroupDefService {

    public void saveOrUpdate(PermUserGroupDef permUserGroupDef);

    public void del(String relationId);

    public List<UserGroupVw> getList(PermUserGroupDef permUserGroupDef);

    public void changeLead(PermUserGroupDef permUserGroupDef) throws Exception;
}
