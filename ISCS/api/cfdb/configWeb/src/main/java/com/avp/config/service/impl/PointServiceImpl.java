package com.avp.config.service.impl;

import java.sql.Timestamp;
import java.util.ArrayList;
import java.util.List;

import com.avp.config.entity.vo.DevCopyVo;
import com.avp.config.service.SignalInstanceService;
import org.springframework.beans.BeanUtils;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.data.domain.Example;
import org.springframework.stereotype.Service;
import org.springframework.transaction.annotation.Transactional;

import com.avp.config.dao.AccTblRepository;
import com.avp.config.dao.AccTpTblRepository;
import com.avp.config.dao.AiLimitTblRepository;
import com.avp.config.dao.AiTblRepository;
import com.avp.config.dao.AioDefTblRepository;
import com.avp.config.dao.AioTpTblRepository;
import com.avp.config.dao.AoTblRepository;
import com.avp.config.dao.DevTblRepository;
import com.avp.config.dao.DiTblRepository;
import com.avp.config.dao.DioDefTblRepository;
import com.avp.config.dao.DioTpTblRepository;
import com.avp.config.dao.DoTblRepository;
import com.avp.config.entity.AccTbl;
import com.avp.config.entity.AccTpTbl;
import com.avp.config.entity.AiTblEntity;
import com.avp.config.entity.AioTpTbl;
import com.avp.config.entity.DevTbl;
import com.avp.config.entity.DiTbl;
import com.avp.config.entity.DioTpTbl;
import com.avp.config.entity.PointEntity;
import com.avp.config.entity.vo.AccSignalInstanceVo;
import com.avp.config.entity.vo.AioSignalInstanceVo;
import com.avp.config.entity.vo.DioSignalInstanceVo;
import com.avp.config.service.PointService;
import com.avp.config.util.BeanTools;
import com.avp.config.util.UniversalStatus;

@Service
@Transactional(rollbackFor = Exception.class)
public class PointServiceImpl implements PointService {
    @Autowired
    private DioTpTblRepository dioTpTblRepository;

    @Autowired
    private AioTpTblRepository aioTpTblRepository;

    @Autowired
    private AccTpTblRepository accTpTblRepository;
    
    @Autowired
    private DevTblRepository devTblRepository;
    
	@Autowired
	private SignalInstanceService signalInstanceService;

	@Autowired
	private DiTblRepository diTblRepository;
	@Autowired
	private DoTblRepository doTblRepository;
	@Autowired
	private DioDefTblRepository dioDefTblRepository;
	@Autowired
	private AiTblRepository aiTblRepository;
	@Autowired
	private AoTblRepository aoTblRepository;
	@Autowired
	private AioDefTblRepository aioDefTblRepository;
	@Autowired
	private AiLimitTblRepository aiLimitTblRepository;
	@Autowired
	private AccTblRepository accTblRepository;
	
    @Override
    public Object save(Object o, String type) throws Exception {
        switch (type){
            case UniversalStatus.POINT_ACC:
                AccTpTbl accTpTbl = (AccTpTbl)o;
                accTpTbl.setOperateTime(new Timestamp(System.currentTimeMillis()));
                //Cookie 此处需要在对应的设备实例下生成对应的继承信号实例
                List<DevTbl> devTs =this.devTblRepository.findByDevTpLabel(accTpTbl.getDevTpLabel());
                for(DevTbl t : devTs) {
                	AccSignalInstanceVo accSignalInstanceVo=new AccSignalInstanceVo();
                	BeanTools.copy(accTpTbl, accSignalInstanceVo);
        			accSignalInstanceVo.setAccName(accTpTbl.getAccTpName());
        			accSignalInstanceVo.setAccLabel(t.getDevLabel()+":"+accTpTbl.getAccTpLabel().split("\\:")[2]);
        			accSignalInstanceVo.setPointTpLabel(accTpTbl.getAccTpLabel());
        			accSignalInstanceVo.setDevLabel(t.getDevLabel());
        			this.signalInstanceService.updateExtendsSignalInstance(UniversalStatus.SIGNAL_ACC,accSignalInstanceVo);
                }
                return this.accTpTblRepository.save(accTpTbl);
            case  UniversalStatus.POINT_AIO:
                AioTpTbl aioTpTbl=(AioTpTbl)o;
                aioTpTbl.setOperateTime(new Timestamp(System.currentTimeMillis()));
                //Cookie 此处需要在对应的设备实例下生成对应的继承信号实例
                DevTbl devT2 = new DevTbl();
                devT2.setDevTpLabel(aioTpTbl.getDevTpLabel());
                List<DevTbl> devTs2 = devTblRepository.findAll(Example.of(devT2));
                for(DevTbl t : devTs2) {
        			AioSignalInstanceVo aioSignalInstanceVo=new AioSignalInstanceVo();
                	BeanTools.copy(aioTpTbl, aioSignalInstanceVo);
        			aioSignalInstanceVo.setAioName(aioTpTbl.getAioTpName());
        			aioSignalInstanceVo.setAioLabel(t.getDevLabel()+":"+aioTpTbl.getAioTpLabel().split("\\:")[2]);
        			aioSignalInstanceVo.setPointTpLabel(aioTpTbl.getAioTpLabel());
        			aioSignalInstanceVo.setDevLabel(t.getDevLabel());
                    this.signalInstanceService.updateExtendsSignalInstance(UniversalStatus.SIGNAL_AIO,aioSignalInstanceVo);
                }
                return this.aioTpTblRepository.save(aioTpTbl);
            case  UniversalStatus.POINT_DIO:
                DioTpTbl dioTpTbl=(DioTpTbl)o;
                dioTpTbl.setOperateTime(new Timestamp(System.currentTimeMillis()));
                //Cookie 此处需要在对应的设备实例下生成对应的继承信号实例
                DevTbl devT3 = new DevTbl();
                devT3.setDevTpLabel(dioTpTbl.getDevTpLabel());
                List<DevTbl> devTs3 = devTblRepository.findAll(Example.of(devT3));
                for(DevTbl t : devTs3) {
                	DioSignalInstanceVo dioSignalInstanceVo=new DioSignalInstanceVo();
                	BeanTools.copy(dioTpTbl, dioSignalInstanceVo);
        			dioSignalInstanceVo.setDioLabel(t.getDevLabel()+":"+dioTpTbl.getDioTpLabel().split("\\:")[2]);
        			dioSignalInstanceVo.setDioName(dioTpTbl.getDioTpName());
        			dioSignalInstanceVo.setPointTpLabel(dioTpTbl.getDioTpLabel());
        			dioSignalInstanceVo.setDevLabel(t.getDevLabel());
        			this.signalInstanceService.updateExtendsSignalInstance(UniversalStatus.SIGNAL_DIO,dioSignalInstanceVo);
                }
                return this.dioTpTblRepository.save(dioTpTbl);
        }
        return null;
    }

    @Override
    public Object update(Object o, String type) throws Exception {
        switch (type){
            case UniversalStatus.POINT_ACC:
                AccTpTbl accTpTbl = (AccTpTbl)o;
                accTpTbl.setOperateTime(new Timestamp(System.currentTimeMillis()));
                //查询已有的点模板
                AccTpTbl accTpTbl2=this.accTpTblRepository.findByAccTpLabel(accTpTbl.getAccTpLabel());
                if(!accTpTbl.getAccTpName().equals(accTpTbl2.getAccTpName())){
                    //更新点对应的信号实例
                    this.accTblRepository.updateAccNameByLabel(accTpTbl.getAccTpName(),accTpTbl.getAccTpLabel());
                }
                return  this.accTpTblRepository.save(accTpTbl);
            case  UniversalStatus.POINT_AIO:
                AioTpTbl aioTpTbl=(AioTpTbl)o;
                aioTpTbl.setOperateTime(new Timestamp(System.currentTimeMillis()));
                //查询已有的点模板
                AioTpTbl aioTpTbl2=this.aioTpTblRepository.findByAioTpLabel(aioTpTbl.getAioTpLabel());
                if(!aioTpTbl.getAioTpName().equals(aioTpTbl2.getAioTpName())){
                    this.aiTblRepository.updateAiNameByLabel(aioTpTbl.getAioTpName(),aioTpTbl.getAioTpLabel());
                }
                return this.aioTpTblRepository.save(aioTpTbl);
            case  UniversalStatus.POINT_DIO:
                DioTpTbl dioTpTbl=(DioTpTbl)o;
                dioTpTbl.setOperateTime(new Timestamp(System.currentTimeMillis()));
                //查询已有的点模板
                DioTpTbl dioTpTbl2=this.dioTpTblRepository.findByDioTpLabel(dioTpTbl.getDioTpLabel());
                if(!dioTpTbl.getDioTpName().equals(dioTpTbl2.getDioTpName())){
                    this.diTblRepository.updateDiNameByLabel(dioTpTbl.getDioTpName(),dioTpTbl.getDioTpLabel());
                }
                return this.dioTpTblRepository.save(dioTpTbl);
        }
        return null;
    }

    @Override
    public List getList(String devId) {
        List<AccTpTbl> accTpTblList=this.accTpTblRepository.findByDevTpLabel(devId);

        List<AioTpTbl> aioTpTblList=this.aioTpTblRepository.findByDevTpLabel(devId);

        List<DioTpTbl> dioTpTblList=this.dioTpTblRepository.findByDevTpLabel(devId);

        List<PointEntity> pointList=new ArrayList<PointEntity>();
        if(accTpTblList!=null && accTpTblList.size()>0){
            for(int i=0;i<accTpTblList.size();i++){
                PointEntity pointEntity=new PointEntity();
                AccTpTbl accTpTbl=accTpTblList.get(i);
                pointEntity.setDevId(accTpTbl.getDevTpLabel());
                pointEntity.setPointId(accTpTbl.getAccTpLabel());
                pointEntity.setPointName(accTpTbl.getAccTpName());
                pointEntity.setType(UniversalStatus.POINT_ACC);
                pointEntity.setDevTpLabel(accTpTbl.getDevTpLabel());
                pointList.add(pointEntity);
            }
        }
        if(aioTpTblList!=null && aioTpTblList.size()>0){
            for(int i=0;i<aioTpTblList.size();i++){
                PointEntity pointEntity=new PointEntity();
                AioTpTbl aioTpTbl=aioTpTblList.get(i);
                pointEntity.setDevId(aioTpTbl.getDevTpLabel());
                pointEntity.setPointName(aioTpTbl.getAioTpName());
                pointEntity.setPointId(aioTpTbl.getAioTpLabel());
                pointEntity.setType(UniversalStatus.POINT_AIO);
                pointEntity.setDevTpLabel(aioTpTbl.getDevTpLabel());
                pointList.add(pointEntity);
            }
        }
        if(dioTpTblList!=null && dioTpTblList.size()>0){
            for(int i=0;i<dioTpTblList.size();i++){
                PointEntity pointEntity=new PointEntity();
                DioTpTbl dioTpTbl=dioTpTblList.get(i);
                pointEntity.setPointId(dioTpTbl.getDioTpLabel());
                pointEntity.setPointName(dioTpTbl.getDioTpName());
                pointEntity.setDevId(dioTpTbl.getDevTpLabel());
                pointEntity.setType(UniversalStatus.POINT_DIO);
                pointEntity.setDevTpLabel(dioTpTbl.getDevTpLabel());
                pointList.add(pointEntity);
            }
        }
        return pointList;
    }

    @Override
    public Object getDetail(String id, String type) {
        switch (type){
            case UniversalStatus.POINT_ACC:
                return this.accTpTblRepository.findByAccTpLabel(id);
            case  UniversalStatus.POINT_AIO:
                return this.aioTpTblRepository.findByAioTpLabel(id);
            case  UniversalStatus.POINT_DIO:
                return this.dioTpTblRepository.findByDioTpLabel(id);
        }
        return null;
    }

    @Override
    public void del(String id) throws Exception {
        this.aioTpTblRepository.deleteByAioTpLabel(id);
        this.dioTpTblRepository.deleteByDioTpLabel(id);
        this.accTpTblRepository.deleteByAccTpLabel(id);
        //删除点模板下所有继承实例
        //根据点模板名字查询到所有他的继承信号实例
        AiTblEntity aiT = new AiTblEntity();
        aiT.setPointTpLabel(id);
        List<AiTblEntity> aiTs = aiTblRepository.findAll(Example.of(aiT));
        for(AiTblEntity ai:aiTs) {
        	signalInstanceService.deleteSignalInstance(ai.getAiLabel());
        }
        DiTbl diT = new DiTbl();
        diT.setPointTpLabel(id);
        List<DiTbl> diTs = diTblRepository.findAll(Example.of(diT));
        for(DiTbl di : diTs) {
        	signalInstanceService.deleteSignalInstance(di.getDiLabel());
        }
        AccTbl accT = new AccTbl();
        accT.setPointTpLabel(id);
        List<AccTbl> accTs = accTblRepository.findAll(Example.of(accT));
        for(AccTbl acc:accTs) {
        	signalInstanceService.deleteSignalInstance(acc.getAccLabel());
        }
    }

    @Override
    public Object copy(Object o) throws Exception {
        DevCopyVo vo = (DevCopyVo)o;
        String type=vo.getPointType();
        switch (type){
            case UniversalStatus.POINT_ACC:
                 AccTpTbl accTpTbl=this.accTpTblRepository.findByAccTpLabel(vo.getSourcePointTpLabel());
                 if(accTpTbl!=null){
                      AccTpTbl accTpTbl2=new AccTpTbl();
                      BeanUtils.copyProperties(accTpTbl,accTpTbl2);
                      accTpTbl2.setAccTpLabel(accTpTbl.getDevTpLabel()+":"+vo.getPointTpLabel());
                      vo.setPointTpLabel(accTpTbl2.getAccTpLabel());
                      vo.setDevTpLabel(accTpTbl2.getDevTpLabel());
                      accTpTbl2.setAccTpName(vo.getPointTpName());
                      this.save(accTpTbl2,type);
                 }
                 return vo;
            case  UniversalStatus.POINT_AIO:
                AioTpTbl aioTpTbl=this.aioTpTblRepository.findByAioTpLabel(vo.getSourcePointTpLabel());
                if(aioTpTbl!=null){
                    AioTpTbl aioTpTbl2=new AioTpTbl();
                    BeanUtils.copyProperties(aioTpTbl,aioTpTbl2);
                    aioTpTbl2.setAioTpLabel(aioTpTbl.getDevTpLabel()+":"+vo.getPointTpLabel());
                    vo.setPointTpLabel(aioTpTbl2.getAioTpLabel());
                    vo.setDevTpLabel(aioTpTbl2.getDevTpLabel());
                    aioTpTbl2.setAioTpName(vo.getPointTpName());
                    this.save(aioTpTbl2,type);
                }
                return vo;
            case  UniversalStatus.POINT_DIO:
                DioTpTbl dioTpTbl=this.dioTpTblRepository.findByDioTpLabel(vo.getSourcePointTpLabel());
                if(dioTpTbl!=null){
                    DioTpTbl dioTpTbl2=new DioTpTbl();
                    BeanUtils.copyProperties(dioTpTbl,dioTpTbl2);
                    dioTpTbl2.setDioTpLabel(dioTpTbl.getDevTpLabel()+":"+vo.getPointTpLabel());
                    vo.setPointTpLabel(dioTpTbl2.getDioTpLabel());
                    vo.setDevTpLabel(dioTpTbl2.getDevTpLabel());
                    dioTpTbl2.setDioTpName(vo.getPointTpName());
                    this.save(dioTpTbl2,type);
                }
                return vo;
        }
        return null;
    }

    //-------------------------------自定义方法 -----------------------------------
    /**
     * 
     * @Title: accTpToVo
     * @Description: 将acc模板转成
     * @author Cookie
     * @date 2018年4月8日 下午5:34:12
     * @company 上海艾文普信息技术有限公司
     * @param accT
     * @param accVo
     * @return
     */
    public AccSignalInstanceVo accTpToVo(AccTpTbl accT,AccSignalInstanceVo accVo) {
    	
    	return accVo;
    }
}
