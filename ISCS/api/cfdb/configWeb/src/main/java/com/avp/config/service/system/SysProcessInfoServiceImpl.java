package com.avp.config.service.system;

import com.avp.config.dao.system.SysProcessInfoRepository;
import com.avp.config.entity.system.SysProcessInfo;
import org.apache.commons.lang.StringUtils;
import org.slf4j.Logger;
import org.slf4j.LoggerFactory;
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

/**
 * Created by zhoujs on 2018/5/22.
 */
@Service
@Transactional(rollbackFor = Exception.class)
public class SysProcessInfoServiceImpl implements SysProcessInfoService {
    private Logger logger = LoggerFactory.getLogger(SysProcessInfoServiceImpl.class);

    @Autowired
    private SysProcessInfoRepository sysProcessInfoRepository;

    @Override
    public List<SysProcessInfo> getList(SysProcessInfo sysProcessInfo) {
        List<SysProcessInfo> sysProcessInfoList = this.sysProcessInfoRepository.findAll(new Specification<SysProcessInfo>() {
            @Override
            public Predicate toPredicate(Root<SysProcessInfo> root, CriteriaQuery<?> criteriaQuery, CriteriaBuilder criteriaBuilder) {
                List<Predicate> list = new ArrayList<Predicate>();

                if (null != sysProcessInfo.getProcId() && sysProcessInfo.getProcId() > 0) {
                    list.add(criteriaBuilder.equal(root.get("procId").as(Integer.class), sysProcessInfo.getProcId()));
                }


                if (StringUtils.isNotBlank(sysProcessInfo.getProcAlias())) {
                    list.add(criteriaBuilder.like(root.get("procAlias").as(String.class), "%" + sysProcessInfo.getProcAlias() + "%"));
                }

                Predicate[] p = new Predicate[list.size()];
                return criteriaBuilder.and(list.toArray(p));
            }
        });
        return sysProcessInfoList;
    }

    @Override
    public SysProcessInfo saveOrUpdate(SysProcessInfo sysProcessInfo,String operate)throws Exception {
        if(!"update".equals(operate)){
            SysProcessInfo sysProcessInfo2=this.sysProcessInfoRepository.findByProcId(sysProcessInfo.getProcId());
            if(sysProcessInfo2!=null){
                throw new Exception("进程id不能重复");
            }
        }
        return this.sysProcessInfoRepository.save(sysProcessInfo);
    }

    @Override
    public SysProcessInfo detail(Integer procId) {
        return this.sysProcessInfoRepository.findByProcId(procId);
    }

    @Override
    public void del(Integer procId) {
        this.sysProcessInfoRepository.deleteByProcId(procId);
    }

}
