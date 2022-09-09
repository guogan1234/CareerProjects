package com.avp.config.dao.user;

import com.avp.config.entity.user.PermSecurityDef;
import org.springframework.data.jpa.repository.JpaRepository;
import org.springframework.data.jpa.repository.JpaSpecificationExecutor;

public interface PermSecurityDefRepository extends JpaRepository<PermSecurityDef, Integer>, JpaSpecificationExecutor<PermSecurityDef> {
    public PermSecurityDef save(PermSecurityDef permSecurityDef);
    public PermSecurityDef findByPermSecurityId(Integer permSecurityId);
    public void deleteByPermSecurityId(Integer permSecurityId);
}
