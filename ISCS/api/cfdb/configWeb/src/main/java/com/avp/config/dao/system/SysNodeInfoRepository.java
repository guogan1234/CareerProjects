package com.avp.config.dao.system;

import com.avp.config.entity.system.SysNodeInfo;
import org.springframework.data.jpa.repository.JpaRepository;
import org.springframework.data.jpa.repository.JpaSpecificationExecutor;

import java.util.List;

public interface SysNodeInfoRepository extends JpaRepository<SysNodeInfo,Integer>,JpaSpecificationExecutor<SysNodeInfo> {
    public SysNodeInfo save(SysNodeInfo sysNodeInfo);

    public SysNodeInfo findByNodeId(Integer nodeId);

    public List<SysNodeInfo> findByNodeName(String nodeName);

    public  List<SysNodeInfo> findByNodeIdOrNodeName(Integer nodeId,String nodeName);

    public void deleteByNodeId(Integer nodeId);
}
