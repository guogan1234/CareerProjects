package com.avp.config.service.user;

import com.avp.config.dao.user.PermGroupStationDefRepository;
import com.avp.config.dao.user.vw.GroupStationVwRepository;
import com.avp.config.entity.user.PermGroupStationDef;
import com.avp.config.entity.user.vw.GroupStationVw;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.data.jpa.domain.Specification;
import org.springframework.stereotype.Controller;
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
@Controller
@Transactional(rollbackFor = Exception.class)
public class PermGroupStationDefServiceImpl implements PermGroupStationDefService {

    @Autowired
    private PermGroupStationDefRepository permGroupStationDefRepository;

    @Autowired
    private GroupStationVwRepository groupStationVwRepository;

    @Override
    public void saveOrUpdate(PermGroupStationDef permGroupStationDef) {
        String stationIds = permGroupStationDef.getStationIds();
        String[] stationIdList = stationIds.split(",");
        List<PermGroupStationDef> list = new ArrayList<>();
        for (int i = 0; i < stationIdList.length; i++) {
            PermGroupStationDef permGroupStationDef1 = new PermGroupStationDef();
            permGroupStationDef1.setGroupId(permGroupStationDef.getGroupId());
            permGroupStationDef1.setStationId(Integer.valueOf(stationIdList[i]));
            list.add(permGroupStationDef1);
        }
        this.permGroupStationDefRepository.save(list);
    }

    @Override
    public void del(String relationId) {
        this.permGroupStationDefRepository.deleteByRelationId(relationId);
    }

    @Override
    public List<GroupStationVw> getList(PermGroupStationDef permGroupStationDef) {
        List<GroupStationVw> groupStationVwList = this.groupStationVwRepository.findAll(new Specification<GroupStationVw>() {
            @Override
            public Predicate toPredicate(Root<GroupStationVw> root, CriteriaQuery<?> query, CriteriaBuilder cb) {
                List<Predicate> list = new ArrayList<Predicate>();

                if (permGroupStationDef.getGroupId() != null) {
                    list.add(cb.equal(root.get("groupId").as(Integer.class), permGroupStationDef.getGroupId()));
                }

                Predicate[] p = new Predicate[list.size()];
                query.where(cb.and(list.toArray(p)));
                //添加排序的功能

                query.orderBy(cb.asc(root.get("groupId").as(Integer.class)));
                return query.getRestriction();
            }
        });
        return groupStationVwList;
    }
}
