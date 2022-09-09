package com.avp.config.dao.system;

import org.springframework.data.jpa.repository.JpaRepository;
import com.avp.config.entity.system.DomainTbl;
import org.springframework.data.jpa.repository.JpaSpecificationExecutor;

import java.util.List;


public interface DomainTblRepository extends JpaRepository<DomainTbl, Integer>,JpaSpecificationExecutor<DomainTbl> {

      public DomainTbl save(DomainTbl domainTbl);

      public List<DomainTbl> findByDomainIdOrDomainLabelOrDomainName(int domainId, String domainLabel, String domainName);

      public List<DomainTbl> findByDomainLabel(String domainLabel);

      public List<DomainTbl> findByDomainName(String domainName);

      public DomainTbl findByDomainId(int domainId);

      public void deleteByDomainId(int domainId);
}
