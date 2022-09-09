package com.avp.config.service.user;

import com.avp.config.dao.user.PermTypeDefRepository;
import com.avp.config.entity.user.PermTypeDef;
import org.apache.commons.lang.StringUtils;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.data.jpa.domain.Specification;
import org.springframework.stereotype.Service;

import javax.persistence.criteria.CriteriaBuilder;
import javax.persistence.criteria.CriteriaQuery;
import javax.persistence.criteria.Predicate;
import javax.persistence.criteria.Root;
import java.util.ArrayList;
import java.util.List;

/**
 * Created by zhoujs on 2018/6/22.
 */
@Service
public class PermTypeDefServiceImpl implements PermTypeDefService {

    @Autowired
    private PermTypeDefRepository permTypeDefRepository;

    @Override
    public List<PermTypeDef> getList(PermTypeDef permTypeDef) {
        List<PermTypeDef> permTypeDefList = this.permTypeDefRepository.findAll(new Specification<PermTypeDef>() {
            @Override
            public Predicate toPredicate(Root<PermTypeDef> root, CriteriaQuery<?> query, CriteriaBuilder cb) {
                List<Predicate> list = new ArrayList<Predicate>();

                if (StringUtils.isNotBlank(permTypeDef.getPermTypeName())) {
                    list.add(cb.like(root.get("permTypeName").as(String.class), "%" + permTypeDef.getPermTypeName() + "%"));
                }

                if (permTypeDef.getPermTypeId() != null) {
                    list.add(cb.equal(root.get("permTypeId").as(Integer.class), permTypeDef.getPermTypeId()));
                }
                Predicate[] p = new Predicate[list.size()];
                query.where(cb.and(list.toArray(p)));
                //添加排序的功能
                query.orderBy(cb.asc(root.get("permTypeId").as(Integer.class)));
                return query.getRestriction();
            }
        });
        return permTypeDefList;
    }

    @Override
    public PermTypeDef saveOrUpdate(PermTypeDef permTypeDef, String operate) throws Exception {
        PermTypeDef permTypeDef1 = new PermTypeDef();
        List<PermTypeDef> list = new ArrayList<>();
        if ("update".equals(operate)) {
            //查询详情
            permTypeDef1 = this.permTypeDefRepository.findByPermTypeId(permTypeDef.getPermTypeId());

            if (!permTypeDef.getPermTypeName().equals(permTypeDef1.getPermTypeName())) {
                list = this.permTypeDefRepository.findByPermTypeName(permTypeDef.getPermTypeName());
            }
        } else {
            list = this.permTypeDefRepository.findByPermTypeIdOrPermTypeName(permTypeDef.getPermTypeId(),permTypeDef.getPermTypeName());
        }
        if (list != null && list.size() > 0) {
            throw new Exception("权限类型ID、权限类型名称有重复");
        }
        return this.permTypeDefRepository.save(permTypeDef);
    }

    @Override
    public PermTypeDef detail(Integer permTypeId) {
        return this.permTypeDefRepository.findByPermTypeId(permTypeId);
    }

    @Override
    public void del(Integer permTypeId) {
        this.permTypeDefRepository.delete(permTypeId);
    }
}
