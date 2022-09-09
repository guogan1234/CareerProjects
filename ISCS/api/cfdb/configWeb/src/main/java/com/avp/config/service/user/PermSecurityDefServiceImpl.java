package com.avp.config.service.user;

import com.avp.config.dao.user.PermSecurityDefRepository;
import com.avp.config.entity.user.PermSecurityDef;
import com.avp.config.entity.user.PermUserDef;
import org.apache.commons.lang.StringUtils;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.data.jpa.domain.Specification;
import org.springframework.stereotype.Service;
import org.springframework.transaction.annotation.Transactional;

import javax.persistence.criteria.CriteriaBuilder;
import javax.persistence.criteria.CriteriaQuery;
import javax.persistence.criteria.Predicate;
import javax.persistence.criteria.Root;
import java.sql.Timestamp;
import java.util.ArrayList;
import java.util.List;


@Service
@Transactional(rollbackFor = Exception.class)
public class PermSecurityDefServiceImpl implements  PermSecurityDefService {
    @Autowired
    private PermSecurityDefRepository permSecurityDefRepository;

    @Override
    public List<PermSecurityDef> getList(PermSecurityDef permSecurityDef) {
        List<PermSecurityDef> permSecurityDefList=this.permSecurityDefRepository.findAll(new Specification<PermSecurityDef>() {
            @Override
            public Predicate toPredicate(Root<PermSecurityDef> root, CriteriaQuery<?> query, CriteriaBuilder cb) {
                List<Predicate> list = new ArrayList<Predicate>();
                if (permSecurityDef.getPermSecurityId()!=null) {
                    list.add(cb.equal(root.get("permSecurityId").as(Integer.class),permSecurityDef.getPermSecurityId()));
                }
                Predicate[] p = new Predicate[list.size()];
                query.where( cb.and(list.toArray(p)));
                //添加排序的功能
                query.orderBy(cb.asc(root.get("permSecurityId").as(Integer.class)));
                return query.getRestriction();
            }
        });
        return permSecurityDefList;
    }

    @Override
    public PermSecurityDef saveOrUpdate(PermSecurityDef permSecurityDef, String operate) throws Exception {
        if("update".equals(operate)){
        }else{
            PermSecurityDef  permSecurityDef2=this.permSecurityDefRepository.findByPermSecurityId(permSecurityDef.getPermSecurityId());
            if(permSecurityDef2!=null){
                throw new Exception("策略id不能重复");
            }
        }
        return this.permSecurityDefRepository.save(permSecurityDef);
    }

    @Override
    public PermSecurityDef detail(Integer permSecurityId) {
        return this.permSecurityDefRepository.findByPermSecurityId(permSecurityId);
    }

    @Override
    public void del(Integer permSecurityId) {
        this.permSecurityDefRepository.deleteByPermSecurityId(permSecurityId);
    }
}
