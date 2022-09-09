package com.avp.config.service.user;
import com.avp.config.entity.user.PermUserDepartmentDef;
import com.avp.config.entity.user.vw.UserDepartmentVw;

import java.util.List;

public interface PermUserDepartmentService {
    public void saveOrUpdate(PermUserDepartmentDef permUserDepartmentDef);
    public void del(String relationId);
    public List<UserDepartmentVw> getList(PermUserDepartmentDef permUserDepartmentDef);
}
