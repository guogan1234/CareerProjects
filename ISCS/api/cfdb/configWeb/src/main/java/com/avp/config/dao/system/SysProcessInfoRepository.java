package com.avp.config.dao.system;

import com.avp.config.entity.system.SysProcessInfo;
import org.springframework.data.jpa.repository.JpaRepository;
import org.springframework.data.jpa.repository.JpaSpecificationExecutor;

/**
 * Created by zhoujs on 2018/5/22.
 */
public interface SysProcessInfoRepository extends JpaRepository<SysProcessInfo,Integer>, JpaSpecificationExecutor<SysProcessInfo> {

    SysProcessInfo save(SysProcessInfo sysProcessInfo);

    SysProcessInfo findByProcId(Integer procId);

    public  void deleteByProcId(Integer procId);

}
