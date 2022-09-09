package com.avp.config.service.base;

import com.avp.config.entity.base.DiTextTbl;
import com.avp.config.entity.base.DoGrpTbl;

import java.util.List;

public interface DoGrpTblService {
    public List<DoGrpTbl> getList(DoGrpTbl doGrpTbl);

    public DoGrpTbl saveOrUpdate(DoGrpTbl doGrpTbl,String operate) throws Exception;

    public DoGrpTbl saveDoGrpConfig(DoGrpTbl doGrpTbl,String allows)throws  Exception;

    public DoGrpTbl detail(int doOrder,String doGrpLabel);

    public void del(int doOrder,String doGrpLabel);
}
