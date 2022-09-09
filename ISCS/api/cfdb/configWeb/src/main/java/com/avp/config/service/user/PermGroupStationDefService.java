package com.avp.config.service.user;

import com.avp.config.entity.user.PermGroupStationDef;
import com.avp.config.entity.user.vw.GroupStationVw;

import java.util.List;

/**
 * Created by zhoujs on 2018/6/26.
 */
public interface PermGroupStationDefService {

    public void saveOrUpdate(PermGroupStationDef permGroupStationDef);

    public void del(String relationId);

    public List<GroupStationVw> getList(PermGroupStationDef permGroupStationDef);
}
