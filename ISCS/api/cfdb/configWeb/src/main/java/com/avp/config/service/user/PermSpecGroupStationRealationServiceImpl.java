package com.avp.config.service.user;

import com.avp.config.dao.user.PermSpecGroupStationRealationRepository;
import com.avp.config.dao.user.vw.PermSpecGroupStationVwRepository;
import com.avp.config.entity.user.PermSpecGroupStationRealation;
import com.avp.config.entity.user.vw.PermSpecGroupStationVw;
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
 * Created by zhoujs on 2018/6/28.
 */
@Service
@Transactional(rollbackFor = Exception.class)
public class PermSpecGroupStationRealationServiceImpl implements PermSpecGroupStationRealationService {

    @Autowired
    private PermSpecGroupStationRealationRepository permSpecGroupStationRealationRepository;
    @Autowired
    private PermSpecGroupStationVwRepository permSpecGroupStationVwRepository;

    @Override
    public void saveOrUpdate(PermSpecGroupStationRealation permSpecGroupStationRealation) throws  Exception {
        try {
            String permIds = permSpecGroupStationRealation.getPermIds();
            String[] permIdArr = new String[]{};
            if (StringUtils.isNotBlank(permIds)) {
                permIdArr = permIds.split(",");
            }

            String stationIds = permSpecGroupStationRealation.getStationIds();
            String[] stationIdArr = new String[]{};
            if (StringUtils.isNotBlank(stationIds)) {
                stationIdArr = stationIds.split(",");
            }

            List<PermSpecGroupStationRealation> list = new ArrayList<>();
            PermSpecGroupStationRealation permSpecGroupStationRealation1 = null;

            for (int i = 0; i < permIdArr.length; i++) {
                for (int j = 0; j < stationIdArr.length; j++) {
                    permSpecGroupStationRealation1 = new PermSpecGroupStationRealation();
                    permSpecGroupStationRealation1.setGroupId(permSpecGroupStationRealation.getGroupId());
                    permSpecGroupStationRealation1.setPermDefine(permSpecGroupStationRealation.getPermDefine());
                    permSpecGroupStationRealation1.setPermId(Integer.valueOf(permIdArr[i]));
                    permSpecGroupStationRealation1.setStationId(Integer.valueOf(Integer.valueOf(stationIdArr[j])));
                    list.add(permSpecGroupStationRealation1);
                }
            }
            this.permSpecGroupStationRealationRepository.save(list);
        } catch (NumberFormatException e) {
            e.printStackTrace();
            throw  new Exception("保存出错!");
        }
    }

    @Override
    public void del(String relationId) {
        this.permSpecGroupStationRealationRepository.deleteByRelationId(relationId);
    }

    @Override
    public List<PermSpecGroupStationVw> getList(PermSpecGroupStationRealation permSpecGroupStationRealation) {
        List<PermSpecGroupStationVw> permSpecGroupStationVwList = this.permSpecGroupStationVwRepository.findAll(new Specification<PermSpecGroupStationVw>() {
            @Override
            public Predicate toPredicate(Root<PermSpecGroupStationVw> root, CriteriaQuery<?> query, CriteriaBuilder cb) {
                List<Predicate> list = new ArrayList<Predicate>();

                if (permSpecGroupStationRealation.getGroupId() != null) {
                    list.add(cb.equal(root.get("groupId").as(Integer.class), permSpecGroupStationRealation.getGroupId()));
                }

                Predicate[] p = new Predicate[list.size()];
                query.where(cb.and(list.toArray(p)));
                //添加排序的功能

                query.orderBy(cb.asc(root.get("permId").as(Integer.class)));
                return query.getRestriction();
            }
        });
        return permSpecGroupStationVwList;
    }
}
