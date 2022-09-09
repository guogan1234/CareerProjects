package com.avp.config.service.user;

import com.avp.config.dao.user.PermDepartmentDefRepository;
import com.avp.config.entity.user.PermDepartmentDef;
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

/**
 * Created by zhoujs on 2018/6/21.
 */
@Service
@Transactional(rollbackFor = Exception.class)
public class PermDepartmentDefServiceImpl implements PermDepartmentDefService {

    @Autowired
    private PermDepartmentDefRepository permDepartmentDefRepository;

    @Override
    public List<PermDepartmentDef> getList(PermDepartmentDef permDepartmentDef) {
        List<PermDepartmentDef> permDepartmentDefList = this.permDepartmentDefRepository.findAll(new Specification<PermDepartmentDef>() {
            @Override
            public Predicate toPredicate(Root<PermDepartmentDef> root, CriteriaQuery<?> query, CriteriaBuilder cb) {
                List<Predicate> list = new ArrayList<Predicate>();

                if (StringUtils.isNotBlank(permDepartmentDef.getDeptName())) {
                    list.add(cb.like(root.get("deptName").as(String.class), "%" + permDepartmentDef.getDeptName() + "%"));
                }

                if (permDepartmentDef.getDeptId() != null) {
                    list.add(cb.equal(root.get("deptId").as(Integer.class), permDepartmentDef.getDeptId()));
                }
                Predicate[] p = new Predicate[list.size()];
                query.where(cb.and(list.toArray(p)));
                //添加排序的功能
                query.orderBy(cb.asc(root.get("deptId").as(Integer.class)));
                return query.getRestriction();
            }
        });
        return permDepartmentDefList;
    }

    @Override
    public PermDepartmentDef saveOrUpdate(PermDepartmentDef permDepartmentDef, String operate) throws Exception {
        PermDepartmentDef permDepartmentDef1 = new PermDepartmentDef();
        List<PermDepartmentDef> list = new ArrayList<>();
        if ("update".equals(operate)) {
            //查询详情
            permDepartmentDef1 = this.permDepartmentDefRepository.findByDeptId(permDepartmentDef.getDeptId());
            if (!permDepartmentDef.getDeptName().equals(permDepartmentDef1.getDeptName())) {
                list = this.permDepartmentDefRepository.findByDeptName(permDepartmentDef.getDeptName());
            }
        } else {
            list = this.permDepartmentDefRepository.findByDeptIdOrDeptName(permDepartmentDef.getDeptId(), permDepartmentDef.getDeptName());
        }
        if (list != null && list.size() > 0) {
            throw new Exception("部门ID、部门名称有重复");
        }
        return this.permDepartmentDefRepository.save(permDepartmentDef);
    }

    @Override
    public PermDepartmentDef detail(Integer deptId) {
        return this.permDepartmentDefRepository.findByDeptId(deptId);
    }

    @Override
    public void del(Integer deptId) {
        this.permDepartmentDefRepository.delete(deptId);
    }
}
