package com.avp.config.service.user;

import com.avp.config.entity.user.PermSecurityDef;
import com.avp.config.entity.user.PermUserDef;

import java.util.List;

public interface PermSecurityDefService {

    public List<PermSecurityDef> getList(PermSecurityDef permSecurityDef);

    public PermSecurityDef saveOrUpdate(PermSecurityDef permSecurityDef,String operate) throws Exception;

    public PermSecurityDef detail(Integer permSecurityId);

    public void del(Integer permSecurityId);
}
