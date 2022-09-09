package com.avp.config.service.impl;

import com.avp.config.dao.*;
import com.avp.config.dao.system.ProSystemTblRepository;
import com.avp.config.entity.*;
import com.avp.config.entity.system.ProSystemTbl;
import com.avp.config.entity.vo.DevCopyVo;
import com.avp.config.service.DevTpTblService;
import com.avp.config.service.PointService;
import com.avp.config.util.UniversalStatus;
import org.apache.commons.lang.StringUtils;
import org.slf4j.Logger;
import org.slf4j.LoggerFactory;
import org.springframework.beans.BeanUtils;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.data.domain.Example;
import org.springframework.data.jpa.domain.Specification;
import org.springframework.stereotype.Service;
import org.springframework.transaction.annotation.Transactional;

import javax.persistence.criteria.CriteriaBuilder;
import javax.persistence.criteria.CriteriaQuery;
import javax.persistence.criteria.Predicate;
import javax.persistence.criteria.Root;
import java.sql.Timestamp;
import java.util.ArrayList;
import java.util.List;

@Service
@Transactional(rollbackFor=Exception.class)
public class DevTpTblServiceImpl implements DevTpTblService {

    private Logger logger= LoggerFactory.getLogger(DevTpTblServiceImpl.class);

    @Autowired
    private DevTpTblRepository devTpTblRepository;
    @Autowired
    private DevTblRepository devTblRepository;
    @Autowired
    private DioTpTblRepository dioTpTblRepository;
    @Autowired
    private AioTpTblRepository aioTpTblRepository;
    @Autowired
    private AccTpTblRepository accTpTblRepository;
    @Autowired
    private ProSystemTblRepository proSystemTblRepository;

    @Autowired
    private PointService pointService;

    @Override
    public DevTpTbl save(DevTpTbl devTpTbl) throws Exception {
        logger.info("新增设备模版");
        DevTpTbl dev=this.devTpTblRepository.findByDevTpLabel(devTpTbl.getDevTpLabel());
        if(dev!=null) {//label重复
            throw new Exception("设备标签重复!");
        }
        ProSystemTbl proSystemTbl=this.proSystemTblRepository.findByProSystemId(devTpTbl.getProSystemId());
        devTpTbl.setDevTpLabel(proSystemTbl.getProSystemLabel()+":"+devTpTbl.getDevTpLabel());
        devTpTbl.setOperateTime(new Timestamp(System.currentTimeMillis()));
        return this.devTpTblRepository.save(devTpTbl);
    }

    @Override
    public DevTpTbl update(DevTpTbl devTpTbl) throws Exception {
        logger.info("修改设备模版");
        devTpTbl.setOperateTime(new Timestamp(System.currentTimeMillis()));
        return this.devTpTblRepository.save(devTpTbl);
    }

    @Override
    public void delete(DevTpTbl devTpTbl) throws Exception {
        logger.info("删除设备模版");
        //1.判断是否存在设备实例，若存在则不可删除
        DevTbl dT = new DevTbl();
        dT.setDevTpLabel(devTpTbl.getDevTpLabel());
        if(devTblRepository.findAll(Example.of(dT)).size()>0) {
            throw new Exception("此模板拥有设备实例");
        }
        //2.无设备实例，删除设备模板下的点模板 ,TODO mix模板暂未处理逻辑
        dioTpTblRepository.deleteByDevTpLabel(devTpTbl.getDevTpLabel());
        aioTpTblRepository.deleteByDevTpLabel(devTpTbl.getDevTpLabel());
        accTpTblRepository.deleteByDevTpLabel(devTpTbl.getDevTpLabel());

        //删除本身
        this.devTpTblRepository.deleteByDevTpLabel(devTpTbl.getDevTpLabel());
    }

    @Override
    public DevTpTbl getDetail(String devTpLabel) {
        logger.info("得到设备模版详情");
        return this.devTpTblRepository.findByDevTpLabel(devTpLabel);
    }

    @Override
    public List<DevTpTbl> getList(DevTpTbl devTpTbl) {
        logger.info("根据专业id得到设备模版列表");
        List<DevTpTbl> devTpTblList=this.devTpTblRepository.findAll(new Specification<DevTpTbl>() {
            @Override
            public Predicate toPredicate(Root<DevTpTbl> root, CriteriaQuery<?> query, CriteriaBuilder cb) {
                List<Predicate> list = new ArrayList<Predicate>();

                if (StringUtils.isNotBlank(devTpTbl.getDevTpName())) {
                    list.add(cb.like(root.get("devTpName").as(String.class), "%" + devTpTbl.getDevTpName()+ "%"));
                }

                if (devTpTbl.getProSystemId()!=null) {
                    list.add(cb.equal(root.get("proSystemId").as(Integer.class),devTpTbl.getProSystemId()));
                }
                Predicate[] p = new Predicate[list.size()];
                return cb.and(list.toArray(p));
            }
        });
        return devTpTblList;
    }

    @Override
    public DevTpTbl copy(DevCopyVo vo) throws Exception {
        logger.info("复制设备");
        String type=vo.getPointType();
        DevTpTbl devTpTbl=this.devTpTblRepository.findByDevTpLabel(vo.getSourceDevTpLabel());
        DevTpTbl devTpTbl2=new DevTpTbl();
        if(devTpTbl!=null){
            BeanUtils.copyProperties(devTpTbl,devTpTbl2);
            devTpTbl2.setDevTpLabel(vo.getDevTpLabel());
            devTpTbl2.setDevTpName(vo.getDevTpName());
            this.save(devTpTbl2);
        }
        //复制下面的点信息
        switch (type){
            case UniversalStatus.POINT_ACC:
                List<AccTpTbl> accTpTblList=this.accTpTblRepository.findByDevTpLabel(vo.getSourceDevTpLabel());
                if(accTpTblList!=null && accTpTblList.size()>0){
                    for(int i=0;i<accTpTblList.size();i++){
                         AccTpTbl accTpTbl=accTpTblList.get(i);
                         AccTpTbl accTpTbl2=new AccTpTbl();
                         BeanUtils.copyProperties(accTpTbl,accTpTbl2);
                         accTpTbl2.setDevTpLabel(devTpTbl2.getDevTpLabel());
                         accTpTbl2.setAccTpLabel(devTpTbl2.getDevTpLabel()+":"+accTpTbl.getAccTpLabel().split("\\:")[2]);
                         this.pointService.save(accTpTbl2,type);
                    }
                }
                break;
            case UniversalStatus.POINT_AIO:
                List<AioTpTbl> aioTpTblList=this.aioTpTblRepository.findByDevTpLabel(vo.getSourceDevTpLabel());
                if(aioTpTblList!=null && aioTpTblList.size()>0){
                    for(int i=0;i<aioTpTblList.size();i++){
                        AioTpTbl aioTpTbl=aioTpTblList.get(i);
                        AioTpTbl aioTpTbl2=new AioTpTbl();
                        BeanUtils.copyProperties(aioTpTbl,aioTpTbl2);
                        aioTpTbl2.setDevTpLabel(devTpTbl2.getDevTpLabel());
                        aioTpTbl2.setAioTpLabel(devTpTbl2.getDevTpLabel()+":"+aioTpTbl.getAioTpLabel().split("\\:")[2]);
                        this.pointService.save(aioTpTbl2,type);
                    }
                }
                break;
            case UniversalStatus.POINT_DIO:
                List<DioTpTbl> dioTpTblList=this.dioTpTblRepository.findByDevTpLabel(vo.getSourceDevTpLabel());
                if(dioTpTblList!=null && dioTpTblList.size()>0){
                    for(int i=0;i<dioTpTblList.size();i++){
                        DioTpTbl dioTpTbl=dioTpTblList.get(i);
                        DioTpTbl dioTpTbl2=new DioTpTbl();
                        BeanUtils.copyProperties(dioTpTbl,dioTpTbl2);
                        dioTpTbl2.setDevTpLabel(devTpTbl2.getDevTpLabel());
                        dioTpTbl2.setDioTpLabel(dioTpTbl2.getDevTpLabel()+":"+dioTpTbl.getDioTpLabel().split("\\:")[2]);
                        this.pointService.save(dioTpTbl2,type);
                    }
                }
                break;
        }
        return devTpTbl2;
    }
}
