package com.avp.config.service.user;

import com.avp.config.dao.user.PermGroupDefRepository;
import com.avp.config.entity.user.PermGroupDef;
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
public class PermGroupDefServiceImpl implements PermGroupDefService {

    @Autowired
    private PermGroupDefRepository permGroupDefRepository;

    @Override
    public List<PermGroupDef> getList(PermGroupDef permGroupDef) {
        List<PermGroupDef> permGroupDefList = this.permGroupDefRepository.findAll(new Specification<PermGroupDef>() {
            @Override
            public Predicate toPredicate(Root<PermGroupDef> root, CriteriaQuery<?> query, CriteriaBuilder cb) {
                List<Predicate> list = new ArrayList<Predicate>();

                if (StringUtils.isNotBlank(permGroupDef.getGroupName())) {
                    list.add(cb.like(root.get("groupName").as(String.class), "%" + permGroupDef.getGroupName() + "%"));
                }

                if (permGroupDef.getGroupId() != null) {
                    list.add(cb.equal(root.get("groupId").as(Integer.class), permGroupDef.getGroupId()));
                }
                Predicate[] p = new Predicate[list.size()];
                query.where(cb.and(list.toArray(p)));
                //添加排序的功能
                query.orderBy(cb.asc(root.get("groupId").as(Integer.class)));
                return query.getRestriction();
            }
        });
        return permGroupDefList;
    }

    @Override
    public PermGroupDef saveOrUpdate(PermGroupDef permGroupDef, String operate) throws Exception {
        PermGroupDef permGroupDef1 = new PermGroupDef();
        List<PermGroupDef> list = new ArrayList<>();
        if ("update".equals(operate)) {
            //查询详情
            permGroupDef1 = this.permGroupDefRepository.findByGroupId(permGroupDef.getGroupId());

            if (!permGroupDef.getGroupName().equals(permGroupDef1.getGroupName())) {
                list = this.permGroupDefRepository.findByGroupName(permGroupDef.getGroupName());
            }
        } else {
            list = this.permGroupDefRepository.findByGroupIdOrGroupName(permGroupDef.getGroupId(),permGroupDef.getGroupName());
        }
        if (list != null && list.size() > 0) {
            throw new Exception("用户组ID、用户组名称有重复");
        }
        return this.permGroupDefRepository.save(permGroupDef);
    }

    @Override
    public PermGroupDef detail(Integer groupId) {
        return this.permGroupDefRepository.findByGroupId(groupId);
    }

    @Override
    public void del(Integer groupId) {
        this.permGroupDefRepository.delete(groupId);
    }
}
