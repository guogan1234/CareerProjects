package com.avp.config.dao.system;

import com.avp.config.entity.system.ProSystemTbl;
import org.springframework.data.jpa.repository.JpaRepository;
import org.springframework.data.jpa.repository.JpaSpecificationExecutor;
import java.util.List;

public interface ProSystemTblRepository extends JpaRepository<ProSystemTbl,Integer>,JpaSpecificationExecutor<ProSystemTbl> {
    public ProSystemTbl save(ProSystemTbl proSystemTbl);

    public List<ProSystemTbl> findByProSystemIdOrProSystemLabelOrProSystemName(int proSystemId, String proSystemLabel, String proSystemName);

    public List<ProSystemTbl> findByProSystemLabel(String ProSystemLabel);

    public List<ProSystemTbl> findByProSystemName(String ProSystemName);

    public ProSystemTbl findByProSystemId(int ProSystemId);

    public void deleteByProSystemId(int ProSystemId);
}
