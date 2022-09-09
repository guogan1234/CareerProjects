package com.avp.config.service.user;

import com.avp.config.entity.user.PermFuncDef;
import com.avp.config.entity.user.PermUserDef;

import java.util.List;

public interface PermFuncDefService {
    public List<PermFuncDef> getList(PermFuncDef permFuncDef);

    public PermFuncDef saveOrUpdate(PermFuncDef permFuncDef,String operate) throws Exception;

    public PermFuncDef detail(Integer permId);

    public void del(Integer permId);
}
