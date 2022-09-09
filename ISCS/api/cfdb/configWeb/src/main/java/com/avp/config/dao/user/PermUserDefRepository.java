package com.avp.config.dao.user;

import com.avp.config.entity.system.DomainTbl;
import com.avp.config.entity.user.PermUserDef;
import org.springframework.data.jpa.repository.JpaRepository;
import org.springframework.data.jpa.repository.JpaSpecificationExecutor;

import java.util.List;

public interface PermUserDefRepository extends JpaRepository<PermUserDef,Integer>,JpaSpecificationExecutor<PermUserDef> {
   public PermUserDef save(PermUserDef permUserDef);
   public PermUserDef findByUserId(Integer userId);
   public List<PermUserDef> findByUserName(String userName);
   public List<PermUserDef> findByUserIdOrUserName(Integer userId,String userName);
   public void deleteByUserId(Integer userId);
}
