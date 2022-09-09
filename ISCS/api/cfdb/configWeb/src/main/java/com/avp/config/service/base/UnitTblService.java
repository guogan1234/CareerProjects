package com.avp.config.service.base;

import com.avp.config.entity.base.UnitTbl;

import java.util.List;

/**
 * Created by zhoujs on 2018/5/17.
 */
public interface UnitTblService {
   public  List<UnitTbl> getList(UnitTbl unitTbl);

   UnitTbl saveOrUpdate(UnitTbl unitTbl,String operate)throws Exception;

   UnitTbl detail(Integer unitId);

   public  void del(Integer unitId);
}
