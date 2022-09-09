package com.avp.config.service.user;

import com.avp.config.dao.user.PermSpecGroupRegionRealationRepository;
import com.avp.config.dao.user.vw.PermSpecGroupRegionVwRepository;
import com.avp.config.entity.user.PermSpecGroupRegionRealation;
import com.avp.config.entity.user.vw.PermSpecGroupRegionVw;
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
public class PermSpecGroupRegionRealationServiceImpl implements PermSpecGroupRegionRealationService {

    @Autowired
    private PermSpecGroupRegionRealationRepository permSpecGroupRegionRealationRepository;

    @Autowired
    private PermSpecGroupRegionVwRepository permSpecGroupRegionVwRepository;

    @Override
    public void saveOrUpdate(PermSpecGroupRegionRealation permSpecGroupRegionRealation) throws Exception {
        try {
            String permIds = permSpecGroupRegionRealation.getPermIds();
            String[] permIdArr = new String[]{};
            if (StringUtils.isNotBlank(permIds)) {
                permIdArr = permIds.split(",");
            }

            String regions = permSpecGroupRegionRealation.getRegionIds();
            String[] regionIdArr = new String[]{};
            if (StringUtils.isNotBlank(regions)) {
                regionIdArr = regions.split(",");
            }

            List<PermSpecGroupRegionRealation> list = new ArrayList<>();
            PermSpecGroupRegionRealation permSpecGroupRegionRealation1;

            for (int i = 0; i < permIdArr.length; i++) {
                for (int j = 0; j < regionIdArr.length; j++) {
                    permSpecGroupRegionRealation1 = new PermSpecGroupRegionRealation();
                    permSpecGroupRegionRealation1.setGroupId(permSpecGroupRegionRealation.getGroupId());
                    permSpecGroupRegionRealation1.setPermDefine(permSpecGroupRegionRealation.getPermDefine());
                    permSpecGroupRegionRealation1.setPermId(Integer.valueOf(permIdArr[i]));
                    permSpecGroupRegionRealation1.setRegionId(Integer.valueOf(Integer.valueOf(regionIdArr[j])));
                    list.add(permSpecGroupRegionRealation1);
                }
            }

            this.permSpecGroupRegionRealationRepository.save(list);
        } catch (NumberFormatException e) {
            e.printStackTrace();
            throw new Exception("保存出错！");
        }
    }

    @Override
    public void del(String relationId) {
        this.permSpecGroupRegionRealationRepository.deleteByRelationId(relationId);
    }

    @Override
    public List<PermSpecGroupRegionVw> getList(PermSpecGroupRegionRealation permSpecGroupRegionRealation) {
        List<PermSpecGroupRegionVw> permSpecGroupRegionVwList = this.permSpecGroupRegionVwRepository.findAll(new Specification<PermSpecGroupRegionVw>() {
            @Override
            public Predicate toPredicate(Root<PermSpecGroupRegionVw> root, CriteriaQuery<?> query, CriteriaBuilder cb) {
                List<Predicate> list = new ArrayList<Predicate>();

                if (permSpecGroupRegionRealation.getGroupId() != null) {
                    list.add(cb.equal(root.get("groupId").as(Integer.class), permSpecGroupRegionRealation.getGroupId()));
                }

                Predicate[] p = new Predicate[list.size()];
                query.where(cb.and(list.toArray(p)));
                //添加排序的功能

                query.orderBy(cb.asc(root.get("permId").as(Integer.class)));
                return query.getRestriction();
            }
        });
        return permSpecGroupRegionVwList;
    }
}
