package com.avp.config.service.base;

import com.avp.config.entity.base.RegionTbl;

import java.util.List;

public interface RegionTblService {
    List<RegionTbl> getList(RegionTbl regionTbl);

    RegionTbl saveOrUpdate(RegionTbl regionTbl,String operate)throws Exception;;

    RegionTbl detail(Integer regionId);

    public void del(Integer regionId);
}
