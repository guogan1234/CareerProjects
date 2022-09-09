package com.avp.config.service.system;

import com.avp.config.entity.system.ProSystemTbl;
import java.util.List;

public interface ProSystemTblService {
    public List<ProSystemTbl> getList(ProSystemTbl proSystemTbl);

    public ProSystemTbl saveOrUpdate(ProSystemTbl proSystemTbl,String operate) throws Exception;

    public ProSystemTbl detail(int proSystemId);

    public void del(int proSystemId);
}
