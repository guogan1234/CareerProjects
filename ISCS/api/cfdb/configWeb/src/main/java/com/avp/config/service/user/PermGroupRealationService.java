package com.avp.config.service.user;
import com.avp.config.entity.user.PermGroupRealation;
import com.avp.config.entity.user.vw.PermGroupRealationVw;
import java.util.List;
public interface PermGroupRealationService {
    public void saveOrUpdate(PermGroupRealation permGroupRealation);
    public void del(String relationId);
    public List<PermGroupRealationVw> getList(PermGroupRealation permGroupRealation);
}
