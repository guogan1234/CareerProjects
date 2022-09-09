package com.avp.config.service.user;

import com.avp.config.dao.user.PermUserDefRepository;
import com.avp.config.entity.user.PermUserDef;
import org.apache.commons.codec.digest.DigestUtils;
import org.apache.commons.lang.StringUtils;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.data.jpa.domain.Specification;
import org.springframework.stereotype.Service;
import org.springframework.transaction.annotation.Transactional;
import javax.persistence.criteria.CriteriaBuilder;
import javax.persistence.criteria.CriteriaQuery;
import javax.persistence.criteria.Predicate;
import javax.persistence.criteria.Root;
import java.security.MessageDigest;
import java.sql.Timestamp;
import java.util.ArrayList;
import java.util.Calendar;
import java.util.Date;
import java.util.List;

@Service
@Transactional(rollbackFor = Exception.class)
public class PermUserDefServiceImpl implements PermUserDefService {
    @Autowired
    private PermUserDefRepository permUserDefRepository;

    @Override
    public List<PermUserDef> getList(PermUserDef permUserDef) {
        List<PermUserDef> permUserDefList=this.permUserDefRepository.findAll(new Specification<PermUserDef>() {
            @Override
            public Predicate toPredicate(Root<PermUserDef> root, CriteriaQuery<?> query, CriteriaBuilder cb) {
                List<Predicate> list = new ArrayList<Predicate>();

                if (StringUtils.isNotBlank(permUserDef.getUserName())) {
                    list.add(cb.like(root.get("userName").as(String.class), "%" + permUserDef.getUserName()+ "%"));
                }

                if (permUserDef.getUserId()!=null) {
                    list.add(cb.equal(root.get("userId").as(Integer.class),permUserDef.getUserId()));
                }
                Predicate[] p = new Predicate[list.size()];
                query.where( cb.and(list.toArray(p)));
                //添加排序的功能
                query.orderBy(cb.asc(root.get("userId").as(Integer.class)));
                return query.getRestriction();
            }
        });
        return permUserDefList;
    }

    @Override
    public PermUserDef saveOrUpdate(PermUserDef permUserDef, String operate) throws Exception {
        PermUserDef permUserDef2=new PermUserDef();
        List<PermUserDef> list = new ArrayList<>();
        if("update".equals(operate)){
            //查询详情
            permUserDef2=this.permUserDefRepository.findByUserId(permUserDef.getUserId());
            if(permUserDef.getUserName().equals(permUserDef2.getUserName())){

            }else if(!permUserDef.getUserName().equals(permUserDef2.getUserName())){
                list=this.permUserDefRepository.findByUserName(permUserDef.getUserName());
            }
        }else{
            permUserDef.setUserPassword( DigestUtils.md5Hex(permUserDef.getUserPassword()));
            list=this.permUserDefRepository.findByUserIdOrUserName(permUserDef.getUserId(),permUserDef.getUserName());
        }
        if(list!=null && list.size()>0){
            throw new Exception("用户id或用户名不能重复");
        }

        permUserDef.setUserCreateDate(new Timestamp(System.currentTimeMillis()));
        Calendar c = Calendar.getInstance();
        //c.set
        Date d = c.getTime();
        c.add(Calendar.YEAR, 3);
        permUserDef.setUserExpireDate(new Timestamp(c.getTimeInMillis()));
        return this.permUserDefRepository.save(permUserDef);
    }

    @Override
    public PermUserDef detail(Integer userId) {
        return this.permUserDefRepository.findByUserId(userId);
    }

    @Override
    public void del(Integer userId) {
        this.permUserDefRepository.deleteByUserId(userId);
    }

    @Override
    public void forbid(Integer userId) {
        PermUserDef permUserDef=this.permUserDefRepository.findByUserId(userId);
        permUserDef.setUserExpireDate(new Timestamp(System.currentTimeMillis()));
        this.permUserDefRepository.save(permUserDef);
    }

    @Override
    public void lock(Integer userId) {
        PermUserDef permUserDef=this.permUserDefRepository.findByUserId(userId);
        permUserDef.setUserIfLock(1);
        permUserDef.setUserLockTime(new Timestamp(System.currentTimeMillis()));
        this.permUserDefRepository.save(permUserDef);
    }

    @Override
    public void cancelLock(Integer userId) {
        PermUserDef permUserDef=this.permUserDefRepository.findByUserId(userId);
        permUserDef.setUserIfLock(0);
        //permUserDef.setUserLockTime(new Timestamp(0));
        this.permUserDefRepository.save(permUserDef);
    }
}
