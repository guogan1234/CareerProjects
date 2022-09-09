package com.avp.config.dao.base;

import com.avp.config.entity.base.DiTextTbl;
import com.avp.config.entity.base.DoGrpMultiKeys;
import com.avp.config.entity.base.DoGrpTbl;
import org.springframework.data.jpa.repository.JpaRepository;
import org.springframework.data.jpa.repository.JpaSpecificationExecutor;

import java.util.List;

public interface DoGrpTblRepository extends JpaRepository<DoGrpTbl,DoGrpMultiKeys>,JpaSpecificationExecutor<DoGrpTbl> {

     public DoGrpTbl save(DoGrpTbl doGrpTbl);

     public DoGrpTbl findByDoOrderAndDoGrpLabel(int doOrder,String doGrpLabel);

     public void deleteByDoOrderAndDoGrpLabel(int doOrder,String doGrpLabel);
}
