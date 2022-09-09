package com.avp.config.service.system;

import com.avp.config.entity.system.SysProcessInfo;

import java.util.List;

/**
 * Created by zhoujs on 2018/5/22.
 */
public interface SysProcessInfoService {

    List<SysProcessInfo> getList(SysProcessInfo sysProcessInfo);

    SysProcessInfo saveOrUpdate(SysProcessInfo sysProcessInfo,String operate)throws  Exception;

    SysProcessInfo detail(Integer procId);

    void del(Integer procId);
}
