package com.avp.config.dao.base;

import com.avp.config.entity.base.DiTextTbl;
import com.avp.config.entity.base.DiTextTblPK;
import com.avp.config.entity.system.DomainTbl;
import org.springframework.data.jpa.repository.JpaRepository;
import org.springframework.data.jpa.repository.JpaSpecificationExecutor;

import java.util.List;

public interface DiTextTblRepository extends JpaRepository<DiTextTbl,DiTextTblPK>,JpaSpecificationExecutor<DiTextTbl> {
    public List<DiTextTbl> findAllByProSystemId(int proSystemId);

    public DiTextTbl save(DiTextTbl diTextTbl);

    public DiTextTbl findByDiTextLabelAndDiValue(String diTextLabel,int diValue);

    public void deleteByDiTextLabelAndDiValue(String diTextLabel,int diValue);
}
