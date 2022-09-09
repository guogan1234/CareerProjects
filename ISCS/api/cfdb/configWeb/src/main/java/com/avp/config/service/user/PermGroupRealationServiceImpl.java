package com.avp.config.service.user;

import com.avp.config.dao.user.PermFuncDefRepository;
import com.avp.config.dao.user.PermGroupRealationRepository;
import com.avp.config.dao.user.vw.PermGroupRealationVwRepository;
import com.avp.config.entity.user.PermFuncDef;
import com.avp.config.entity.user.PermGroupRealation;
import com.avp.config.entity.user.vw.PermGroupRealationVw;
import com.avp.config.util.BeanTools;
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

@Service
@Transactional(rollbackFor = Exception.class)
public class PermGroupRealationServiceImpl implements PermGroupRealationService {
    @Autowired
    private PermGroupRealationRepository permGroupRealationRepository;
    @Autowired
    private PermGroupRealationVwRepository permGroupRealationVwRepository;
    @Autowired
    private PermFuncDefRepository permFuncDefRepository;

    @Override
    public void saveOrUpdate(PermGroupRealation permGroupRealation) {
        String permIds=permGroupRealation.getPermIds();
        String[] permIdArr=permIds.split(",");
        List<PermGroupRealation> list=new ArrayList<>();
        for(int i=0;i<permIdArr.length;i++){
           PermGroupRealation permGroupRealation2=new PermGroupRealation();
           permGroupRealation2.setPermId(Integer.valueOf(permIdArr[i]));
           permGroupRealation2.setGroupId(permGroupRealation.getGroupId());

           PermFuncDef permFuncDef=this.permFuncDefRepository.findByPermId(Integer.valueOf(permIdArr[i]));
           permGroupRealation2.setPermDefine(permFuncDef.getPermDefine());
           list.add(permGroupRealation2);
        }
        this.permGroupRealationRepository.save(list);
    }

    @Override
    public void del(String relationId) {
         this.permGroupRealationRepository.deleteByRelationId(relationId);
    }

    @Override
    public List<PermGroupRealationVw> getList(PermGroupRealation permGroupRealation) {
        List<PermGroupRealationVw> permGroupRealationVwList=this.permGroupRealationVwRepository.findAll(new Specification<PermGroupRealationVw>() {
            @Override
            public Predicate toPredicate(Root<PermGroupRealationVw> root, CriteriaQuery<?> query, CriteriaBuilder cb) {
                List<Predicate> list = new ArrayList<Predicate>();

                if (permGroupRealation.getGroupId()!=null) {
                    list.add(cb.equal(root.get("groupId").as(Integer.class),permGroupRealation.getGroupId()));
                }
                Predicate[] p = new Predicate[list.size()];
                query.where( cb.and(list.toArray(p)));
                //添加排序的功能
                query.orderBy(cb.asc(root.get("groupId").as(Integer.class)));
                return query.getRestriction();
            }
        });
        return permGroupRealationVwList;
    }
}
