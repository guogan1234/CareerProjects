package com.avp.config.dao.user;

import com.avp.config.entity.user.PermFuncDef;
import com.avp.config.entity.user.PermUserDef;
import org.springframework.data.jpa.repository.JpaRepository;
import org.springframework.data.jpa.repository.JpaSpecificationExecutor;

import java.util.List;

public interface PermFuncDefRepository extends JpaRepository<PermFuncDef,Integer>,JpaSpecificationExecutor<PermFuncDef> {
    public PermFuncDef save(PermFuncDef permFuncDef);
    public PermFuncDef findByPermId(Integer permId);
    public List<PermFuncDef> findByPermName(String permName);
    public List<PermFuncDef> findByPermIdOrPermName(Integer permId,String permName);
    public void deleteByPermId(Integer permId);
}
