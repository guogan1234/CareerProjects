package com.avp.config.service.base;

import com.avp.config.entity.base.DevTypeTbl;
import com.avp.config.entity.base.DiTextTbl;

import java.util.List;

public interface DevTypeTblService {
    public List<DevTypeTbl> getList(DevTypeTbl devTypeTbl);

    public DevTypeTbl saveOrUpdate(DevTypeTbl devTypeTbl,String operate) throws Exception;

    public DevTypeTbl detail(String devTypeLabel);

    public void del(String devTypeLabel);
}
