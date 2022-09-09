package com.avp.config.service.user;

import com.avp.config.dao.user.PermUserDepartmentRepository;
import com.avp.config.dao.user.vw.UserDepartmentVwRepository;
import com.avp.config.entity.user.PermUserDepartmentDef;
import com.avp.config.entity.user.vw.UserDepartmentVw;
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
public class PermUserDepartmentServiceImpl implements PermUserDepartmentService {

    @Autowired
    private PermUserDepartmentRepository permUserDepartmentRepository;
    @Autowired
    private UserDepartmentVwRepository userDepartmentVwRepository;

    @Override
    public void saveOrUpdate(PermUserDepartmentDef permUserDepartmentDef) {
        String userIds=permUserDepartmentDef.getUserIds();
        String[]  userIdList=userIds.split(",");
        List<PermUserDepartmentDef> list=new ArrayList<>();
        for (int i=0;i<userIdList.length;i++){
            PermUserDepartmentDef permUserDepartmentDef2=new PermUserDepartmentDef();
            permUserDepartmentDef2.setDeptId(permUserDepartmentDef.getDeptId());
            permUserDepartmentDef2.setUserId(Integer.valueOf(userIdList[i]));
            list.add(permUserDepartmentDef2);
        }
        this.permUserDepartmentRepository.save(list);
    }

    @Override
    public void del(String relationId) {
        this.permUserDepartmentRepository.deleteByRelationId(relationId);
    }

    @Override
    public List<UserDepartmentVw> getList(PermUserDepartmentDef permUserDepartmentDef) {
        List<UserDepartmentVw> userDepartmentVwList=this.userDepartmentVwRepository.findAll(new Specification<UserDepartmentVw>() {
            @Override
            public Predicate toPredicate(Root<UserDepartmentVw> root, CriteriaQuery<?> query, CriteriaBuilder cb) {
                List<Predicate> list = new ArrayList<Predicate>();

                if (permUserDepartmentDef.getDeptId()!=null) {
                    list.add(cb.equal(root.get("deptId").as(Integer.class),permUserDepartmentDef.getDeptId()));
                }
                Predicate[] p = new Predicate[list.size()];
                query.where( cb.and(list.toArray(p)));
                //添加排序的功能
                query.orderBy(cb.asc(root.get("deptId").as(Integer.class)));
                return query.getRestriction();
            }
        });
        return userDepartmentVwList;
    }
}
