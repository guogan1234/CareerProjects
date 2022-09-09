package com.avp.config.service.user;

import com.avp.config.entity.user.PermGroupDef;

import java.util.List;

/**
 * Created by zhoujs on 2018/6/22.
 */
public interface PermGroupDefService {
    public List<PermGroupDef> getList(PermGroupDef permGroupDef);

    public PermGroupDef saveOrUpdate(PermGroupDef permGroupDef,String operate) throws Exception;

    public PermGroupDef detail(Integer groupId);

    public void del(Integer groupId);
}
