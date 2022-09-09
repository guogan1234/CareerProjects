package com.avp.config.service.system;

import com.avp.config.entity.system.SysNodeInfo;

import java.util.List;

public interface SysNodeInfoService {
    public List<SysNodeInfo> getList(SysNodeInfo sysNodeInfo);

    public SysNodeInfo saveOrUpdate(SysNodeInfo sysNodeInfo,String operate) throws Exception;

    public SysNodeInfo detail(int nodeId);

    public void del(int nodeId);
}
