package com.avp.config.service.user;

import com.avp.config.dao.user.PermUserGroupDefRepository;
import com.avp.config.dao.user.vw.UserGroupVwRepository;
import com.avp.config.entity.user.PermUserGroupDef;
import com.avp.config.entity.user.vw.UserGroupVw;
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
 * Created by zhoujs on 2018/6/26.
 */
@Service
@Transactional(rollbackFor = Exception.class)
public class PermUserGroupDefServiceImpl implements PermUserGroupDefService {

    @Autowired
    private PermUserGroupDefRepository permUserGroupDefRepository;

    @Autowired
    private UserGroupVwRepository userGroupVwRepository;

    @Override
    public void saveOrUpdate(PermUserGroupDef permUserGroupDef) {
        String userIds = permUserGroupDef.getUserIds();
        String[] userIdList = userIds.split(",");
        List<PermUserGroupDef> list = new ArrayList<>();
        for (int i = 0; i < userIdList.length; i++) {
            PermUserGroupDef permUserGroupDef1 = new PermUserGroupDef();
            permUserGroupDef1.setUserId(Integer.valueOf(userIdList[i]));
            permUserGroupDef1.setGroupId(permUserGroupDef.getGroupId());
            list.add(permUserGroupDef1);
        }
        this.permUserGroupDefRepository.save(list);
    }

    @Override
    public void del(String relationId) {
        this.permUserGroupDefRepository.deleteByRelationId(relationId);
    }

    @Override
    public List<UserGroupVw> getList(PermUserGroupDef permUserGroupDef) {
        List<UserGroupVw> userGroupVwList = this.userGroupVwRepository.findAll(new Specification<UserGroupVw>() {
            @Override
            public Predicate toPredicate(Root<UserGroupVw> root, CriteriaQuery<?> query, CriteriaBuilder cb) {
                List<Predicate> list = new ArrayList<Predicate>();

                if (permUserGroupDef.getGroupId() != null) {
                    list.add(cb.equal(root.get("groupId").as(Integer.class), permUserGroupDef.getGroupId()));
                }

                Predicate[] p = new Predicate[list.size()];
                query.where(cb.and(list.toArray(p)));
                //添加排序的功能

                query.orderBy(cb.asc(root.get("userId").as(Integer.class)));
                return query.getRestriction();
            }
        });
        return userGroupVwList;
    }

    @Override
    public void changeLead(PermUserGroupDef permUserGroupDef) throws Exception {
        if (permUserGroupDef.getIfLead() == 1) {  //设置组长

            List<PermUserGroupDef> permUserGroupDefList = this.permUserGroupDefRepository.
                    findByGroupIdAndIfLead(permUserGroupDef.getGroupId(), permUserGroupDef.getIfLead());

            if (permUserGroupDefList.size() > 0) {
                throw new Exception("该用户组已有组长");
            }
        }

        PermUserGroupDef permUserGroupDef1 = this.permUserGroupDefRepository.findByRelationId(permUserGroupDef.getRelationId());
        permUserGroupDef1.setIfLead(permUserGroupDef.getIfLead());
        this.permUserGroupDefRepository.save(permUserGroupDef1);

    }
}
