package com.avp.config.service.user;

import com.avp.config.entity.user.PermSpecGroupRegionRealation;
import com.avp.config.entity.user.vw.PermSpecGroupRegionVw;

import java.util.List;

/**
 * Created by zhoujs on 2018/6/28.
 */
public interface PermSpecGroupRegionRealationService {

    public void saveOrUpdate(PermSpecGroupRegionRealation permSpecGroupRegionRealation) throws Exception;

    public void del(String relationId);

    public List<PermSpecGroupRegionVw> getList(PermSpecGroupRegionRealation permSpecGroupRegionRealation);
}
