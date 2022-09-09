package com.avp.config.service.user;

import com.avp.config.dao.user.PermFuncDefRepository;
import com.avp.config.dao.user.PermTypeDefRepository;
import com.avp.config.entity.user.PermFuncDef;
import com.avp.config.entity.user.PermTypeDef;
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
public class PermFuncDefServiceImpl implements PermFuncDefService {
   @Autowired
    private PermFuncDefRepository permFuncDefRepository;
   @Autowired
   private PermTypeDefRepository permTypeDefRepository;


    @Override
    public List<PermFuncDef> getList(PermFuncDef permFuncDef) {
        List<PermFuncDef> permFuncDefList=this.permFuncDefRepository.findAll(new Specification<PermFuncDef>() {
            @Override
            public Predicate toPredicate(Root<PermFuncDef> root, CriteriaQuery<?> query, CriteriaBuilder cb) {
                List<Predicate> list = new ArrayList<Predicate>();

                if (StringUtils.isNotBlank(permFuncDef.getPermName())) {
                    list.add(cb.like(root.get("permName").as(String.class), "%" + permFuncDef.getPermName()+ "%"));
                }

                if (StringUtils.isNotBlank(permFuncDef.getPermTypeName()) && "普通权限".equals(permFuncDef.getPermTypeName())) {
                    list.add(cb.notEqual(root.get("permTypeName").as(String.class),"特殊权限"));
                }
                if (StringUtils.isNotBlank(permFuncDef.getPermTypeName()) && "特殊权限".equals(permFuncDef.getPermTypeName())) {
                    list.add(cb.equal(root.get("permTypeName").as(String.class),permFuncDef.getPermTypeName()));
                }
                if (permFuncDef.getPermId()!=null) {
                    list.add(cb.equal(root.get("permId").as(Integer.class),permFuncDef.getPermId()));
                }
                Predicate[] p = new Predicate[list.size()];
                query.where( cb.and(list.toArray(p)));
                //添加排序的功能
                query.orderBy(cb.asc(root.get("permId").as(Integer.class)));
                return query.getRestriction();
            }
        });
        return permFuncDefList;
    }

    @Override
    public PermFuncDef saveOrUpdate(PermFuncDef permFuncDef, String operate) throws Exception {
        PermFuncDef permFuncDef2=new PermFuncDef();
        List<PermFuncDef> list = new ArrayList<>();
        PermTypeDef permTypeDef=permTypeDefRepository.findByPermTypeId(permFuncDef.getPermTypeId());
        permFuncDef.setPermTypeName(permTypeDef.getPermTypeName());
        if("update".equals(operate)){
            //查询详情
            permFuncDef2=this.permFuncDefRepository.findByPermId(permFuncDef.getPermId());
            if(!permFuncDef.getPermName().equals(permFuncDef2.getPermName())){
                list=this.permFuncDefRepository.findByPermName(permFuncDef.getPermName());
            }
        }else{
            list=this.permFuncDefRepository.findByPermIdOrPermName(permFuncDef.getPermId(),permFuncDef.getPermName());
        }
        if(list!=null && list.size()>0){
            throw new Exception("权限id或权限名不能重复");
        }
        return this.permFuncDefRepository.save(permFuncDef);
    }

    @Override
    public PermFuncDef detail(Integer permId) {
        return this.permFuncDefRepository.findByPermId(permId);
    }

    @Override
    public void del(Integer permId) {
       this.permFuncDefRepository.deleteByPermId(permId);
    }
}
