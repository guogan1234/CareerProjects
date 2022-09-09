package com.avp.config.service.user;

import com.avp.config.entity.user.PermDepartmentDef;

import java.util.List;

/**
 * Created by zhoujs on 2018/6/21.
 */
public interface PermDepartmentDefService {
    public List<PermDepartmentDef> getList(PermDepartmentDef permDepartmentDef);

    public PermDepartmentDef saveOrUpdate(PermDepartmentDef permDepartmentDef,String operate) throws Exception;

    public PermDepartmentDef detail(Integer deptId);

    public void del(Integer deptId);
}
