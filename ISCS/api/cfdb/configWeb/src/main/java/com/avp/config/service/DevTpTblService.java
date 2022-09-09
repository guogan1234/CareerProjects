package com.avp.config.service;

import com.avp.config.entity.DevTpTbl;
import com.avp.config.entity.vo.DevCopyVo;

import java.util.List;

public interface DevTpTblService {

    public DevTpTbl save(DevTpTbl devTpTbl) throws Exception;

    public DevTpTbl update(DevTpTbl devTpTbl) throws Exception;

    public void delete(DevTpTbl devTpTbl) throws Exception;

    public DevTpTbl  getDetail(String devTpLabel);

    public List<DevTpTbl> getList(DevTpTbl devTpTbl);

    public DevTpTbl copy(DevCopyVo vo) throws Exception;

}
