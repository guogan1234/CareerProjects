package com.avp.config.service.user;

import com.avp.config.entity.user.PermTypeDef;

import java.util.List;

/**
 * Created by zhoujs on 2018/6/22.
 */
public interface PermTypeDefService {

    public List<PermTypeDef> getList(PermTypeDef permTypeDef);

    public PermTypeDef saveOrUpdate(PermTypeDef permTypeDef,String operate) throws Exception;

    public PermTypeDef detail(Integer permTypeId);

    public void del(Integer permTypeId);
}
