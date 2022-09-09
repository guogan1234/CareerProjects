package com.avp.config.service.system;

import com.avp.config.dao.system.SysNodeInfoRepository;
import com.avp.config.entity.system.SysNodeInfo;
import org.apache.commons.lang.StringUtils;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.data.jpa.domain.Specification;
import org.springframework.stereotype.Service;
import org.springframework.transaction.annotation.Transactional;

import javax.persistence.criteria.CriteriaBuilder;
import javax.persistence.criteria.CriteriaQuery;
import javax.persistence.criteria.Predicate;
import javax.persistence.criteria.Root;
import java.util.ArrayList;
import java.util.List;

@Service
@Transactional(rollbackFor = Exception.class)
public class SysNodeInfoServiceImpl implements SysNodeInfoService {
    @Autowired
    private SysNodeInfoRepository sysNodeInfoRepository;

    @Override
    public List<SysNodeInfo> getList(SysNodeInfo sysNodeInfo) {
        List<SysNodeInfo> sysNodeInfoList=this.sysNodeInfoRepository.findAll(new Specification<SysNodeInfo>() {
            @Override
            public Predicate toPredicate(Root<SysNodeInfo> root, CriteriaQuery<?> query, CriteriaBuilder cb) {
                List<Predicate> list = new ArrayList<Predicate>();

                if (StringUtils.isNotBlank(sysNodeInfo.getNodeName())) {
                    list.add(cb.like(root.get("nodeName").as(String.class), "%" + sysNodeInfo.getNodeName()+ "%"));
                }

                if (sysNodeInfo.getNodeId()!=null) {
                    list.add(cb.equal(root.get("nodeId").as(Integer.class),sysNodeInfo.getNodeId()));
                }
                Predicate[] p = new Predicate[list.size()];
                query.where( cb.and(list.toArray(p)));
                //添加排序的功能
                query.orderBy(cb.asc(root.get("nodeId").as(Integer.class)));
                return query.getRestriction();
            }
        });
        return sysNodeInfoList;
    }

    @Override
    public SysNodeInfo saveOrUpdate(SysNodeInfo sysNodeInfo, String operate) throws Exception {
        //id，label,name 唯一
        List<SysNodeInfo> sysNodeInfoList=new ArrayList<>();
        SysNodeInfo sysNodeInfo2=new SysNodeInfo();
        if("update".equals(operate)){
            //查询详情
            sysNodeInfo2=this.sysNodeInfoRepository.findByNodeId(sysNodeInfo.getNodeId());
            if(!sysNodeInfo.getNodeName().equals(sysNodeInfo2.getNodeName())){
                sysNodeInfoList=this.sysNodeInfoRepository.findByNodeName(sysNodeInfo.getNodeName());
            }
        }else{
            sysNodeInfoList=this.sysNodeInfoRepository.findByNodeIdOrNodeName(sysNodeInfo.getNodeId(),sysNodeInfo.getNodeName());
        }
        if(sysNodeInfoList!=null && sysNodeInfoList.size()>0){
            throw new Exception("节点ID或节点名称有重复");
        }
        return this.sysNodeInfoRepository.save(sysNodeInfo);
    }

    @Override
    public SysNodeInfo detail(int nodeId) {
        return this.sysNodeInfoRepository.findByNodeId(nodeId);
    }

    @Override
    public void del(int nodeId) {
        this.sysNodeInfoRepository.deleteByNodeId(nodeId);
    }
}
