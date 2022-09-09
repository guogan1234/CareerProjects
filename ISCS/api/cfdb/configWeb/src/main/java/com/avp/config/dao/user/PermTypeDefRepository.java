package com.avp.config.dao.user;

import com.avp.config.entity.user.PermTypeDef;
import org.springframework.data.jpa.repository.JpaRepository;
import org.springframework.data.jpa.repository.JpaSpecificationExecutor;

import java.util.List;

/**
 * Created by zhoujs on 2018/6/22.
 */
public interface PermTypeDefRepository extends JpaRepository<PermTypeDef, Integer>, JpaSpecificationExecutor<PermTypeDef> {

        public PermTypeDef save(PermTypeDef permTypeDef);

        public PermTypeDef findByPermTypeId(Integer permTypeId);

        public List<PermTypeDef> findByPermTypeName(String permTypeName);

        public List<PermTypeDef> findByPermTypeIdOrPermTypeName(Integer permTypeId,String permTypeName);
}
