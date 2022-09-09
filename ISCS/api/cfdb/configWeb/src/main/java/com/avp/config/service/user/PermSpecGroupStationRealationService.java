package com.avp.config.service.user;

import com.avp.config.entity.user.PermSpecGroupStationRealation;
import com.avp.config.entity.user.vw.PermSpecGroupStationVw;

import java.util.List;

/**
 * Created by zhoujs on 2018/6/28.
 */
public interface PermSpecGroupStationRealationService {

    public void saveOrUpdate(PermSpecGroupStationRealation permSpecGroupStationRealation) throws  Exception;

    public void del(String relationId);

    public List<PermSpecGroupStationVw> getList(PermSpecGroupStationRealation permSpecGroupStationRealation);
}
