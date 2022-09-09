package com.avp.config.service.user;

import com.avp.config.entity.system.DomainTbl;
import com.avp.config.entity.user.PermUserDef;

import java.util.List;

public interface PermUserDefService {
    public List<PermUserDef> getList(PermUserDef permUserDef);

    public PermUserDef saveOrUpdate(PermUserDef permUserDef,String operate) throws Exception;

    public PermUserDef detail(Integer userId);

    public void del(Integer userId);

    public void forbid(Integer userId);

    public void lock(Integer userId);

    public void cancelLock(Integer userId);
}
