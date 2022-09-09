package com.avp.config.service.impl;

import java.sql.Timestamp;
import java.util.ArrayList;
import java.util.List;

import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.data.domain.Example;
import org.springframework.stereotype.Service;
import org.springframework.transaction.annotation.Transactional;

import com.avp.config.dao.AccDefTblRepository;
import com.avp.config.dao.AccLimitTblRepository;
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
import com.avp.config.entity.AccDefTblEntity;
import com.avp.config.entity.AccLimitTblEntity;
import com.avp.config.entity.AccTbl;
import com.avp.config.entity.AccTpTbl;
import com.avp.config.entity.AiLimitTblEntity;
import com.avp.config.entity.AiTblEntity;
import com.avp.config.entity.AioDefTblEntity;
import com.avp.config.entity.AioTpTbl;
import com.avp.config.entity.AoTbl;
import com.avp.config.entity.DevTbl;
import com.avp.config.entity.DiTbl;
import com.avp.config.entity.DioDefTblEntity;
import com.avp.config.entity.DioTpTbl;
import com.avp.config.entity.DoTbl;
import com.avp.config.entity.dto.GetSignalInsatnceListDto;
import com.avp.config.entity.po.AccPo;
import com.avp.config.entity.po.AioPo;
import com.avp.config.entity.vo.AccSignalInstanceVo;
import com.avp.config.entity.vo.AioSignalInstanceVo;
import com.avp.config.entity.vo.DevCopyVo;
import com.avp.config.entity.vo.DioSignalInstanceVo;
import com.avp.config.service.SignalInstanceService;
import com.avp.config.util.BeanTools;
import com.avp.config.util.UniversalStatus;

@Service
@Transactional(rollbackFor=Exception.class)
public class SignalInstanceServiceImpl implements SignalInstanceService {

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
	@Autowired
	private AccLimitTblRepository accLimitTblRepository;
	@Autowired
	private AccDefTblRepository accDefTblRepository;
	@Autowired
	private DevTblRepository devTblRepository;
	@Autowired
	private DioTpTblRepository dioTpTblRepository;
	@Autowired
	private AioTpTblRepository aioTpTblRepository;
	@Autowired
	private AccTpTblRepository accTpTblRepository;

	/**
	 * 
	 * @Title: doPointInstance
	 * @Description: 删除信号实例
	 * @author Cookie
	 * @date 2018年3月29日 上午9:53:52
	 * @company 上海艾文普信息技术有限公司
	 * @param signalLabel 信号标签
	 * @throws Exception 
	 */
	@Override
	public void deleteSignalInstance(String signalLabel) throws Exception {
		//删除逻辑，只有特殊点实例才能删除
			diTblRepository.deleteByDiLabel(signalLabel);
			doTblRepository.deleteByDoLabel(signalLabel);
			dioDefTblRepository.deleteByDioDefLabel(signalLabel);
			//aio表
			aiTblRepository.deleteByAiLabel(signalLabel);
			aoTblRepository.deleteByAoLabel(signalLabel);
			aioDefTblRepository.deleteByAioDefLabel(signalLabel);
			aiLimitTblRepository.deleteByAiLimitLabel(signalLabel);
            //ACC表
			accTblRepository.deleteByAccLabel(signalLabel);
			accLimitTblRepository.deleteByAccLimitLabel(signalLabel);
			accDefTblRepository.deleteByAccDefLabel(signalLabel);
	}

	/**
	 * 
	 * @Title: saveSignalInstance
	 * @Description: 增加特殊信号实例
	 * @author Cookie
	 * @date 2018年4月4日 上午9:41:16
	 * @company 上海艾文普信息技术有限公司
	 * @param type
	 * @param o
	 * @return
	 * @throws Exception
	 */
	@Override
	public Object saveSignalInstance(int type, Object o) throws Exception {
		switch (type) {
			case UniversalStatus.SIGNAL_DIO:
				DioSignalInstanceVo dioSignalInstanceVo = (DioSignalInstanceVo)o;
				dioSignalInstanceVo.setDioLabel(dioSignalInstanceVo.getDevLabel()+":"+dioSignalInstanceVo.getDioLabel());
				return this.dioSignalInstance((DioSignalInstanceVo)o);
			case UniversalStatus.SIGNAL_AIO:
				AioSignalInstanceVo aioSignalInstanceVo=(AioSignalInstanceVo)o;
				aioSignalInstanceVo.setAioLabel(aioSignalInstanceVo.getDevLabel()+":"+aioSignalInstanceVo.getAioLabel());
				return this.addAioSignalInstance((AioSignalInstanceVo)o);
			case UniversalStatus.SIGNAL_ACC:
			    AccSignalInstanceVo accSignalInstanceVo=(AccSignalInstanceVo)o;
			    accSignalInstanceVo.setAccLabel(accSignalInstanceVo.getDevLabel()+":"+accSignalInstanceVo.getAccLabel());
				return this.addAccSignalInstance((AccSignalInstanceVo)o);
			case UniversalStatus.SIGNAL_MIX:
				//TODO mix 模板逻辑暂定
				break;
			default:
				throw new Exception("不合法的入参");
		}
		return null;
	}

	/**
	 * 
	 * @Title: updatePointInstance
	 * @Description: 修改特殊信号实例
	 * @author Cookie
	 * @date 2018年3月29日 上午10:31:56
	 * @company 上海艾文普信息技术有限公司
	 * @throws Exception 
	 */
	@Override
	public Object updateSignalInstance(int type, Object o) throws Exception {
		switch (type) {
		case UniversalStatus.SIGNAL_DIO:
			return this.updateDioSignalInstance((DioSignalInstanceVo)o);
		case UniversalStatus.SIGNAL_AIO:
		    return this.updateAioSignalInstance((AioSignalInstanceVo)o);
		case UniversalStatus.SIGNAL_ACC:
			return this.updateAccSignalInstance((AccSignalInstanceVo)o);
		case UniversalStatus.SIGNAL_MIX:
			//TODO mix 模板逻辑暂定

			break;
		default:
			throw new Exception("不合法的入参");
		}
		return null;
	}

	
	/**
	 * 
	 * @Title: getSignalInstance
	 * @Description: 查询单个信号实例详情
	 * @author Cookie
	 * @date 2018年3月29日 上午10:34:24
	 * @company 上海艾文普信息技术有限公司
	 * @param type(1:Dio,2:AIO,3:ACC,4:MIX)
	 * @param signalLabel
	 * @throws Exception 
	 */
	@Override
	public Object getSignalInstance(int type, String signalLabel) throws Exception {
		switch (type) {
		case UniversalStatus.SIGNAL_DIO:
			DioSignalInstanceVo vo = new DioSignalInstanceVo();
			DiTbl diTbl=diTblRepository.findOne(signalLabel);
			//查询DIO点模板
			if(diTbl.getPointTpLabel()!=null) {
				DioTpTbl dTT = dioTpTblRepository.findOne(diTbl.getPointTpLabel());
				if(dTT!=null) {
					vo.setDioTpLabel(dTT.getDioTpLabel());
					vo.setDioTpName(dTT.getDioTpName());
				}
			}
			if(diTbl!=null){
				BeanTools.copy(diTbl,vo);
				vo.setDioLabel(diTbl.getDiLabel());
				vo.setDioName(diTbl.getDiName());
			}
			DoTbl doTbl=doTblRepository.findOne(signalLabel);
			if(doTbl!=null){
				BeanTools.copy(doTbl,vo);
				vo.setCtrlTimeout(doTbl.getExecTimeout());
				vo.setCtrlGrpLabel(doTbl.getDoGrpLabel());
				vo.setCtrlWaitAct(doTbl.getWaitAct());
			}
			DioDefTblEntity dioDefTblEntity = dioDefTblRepository.findOne(signalLabel);
			if(dioDefTblEntity!=null){
				BeanTools.copy(dioDefTblEntity,vo);
				vo.setPolarity2(dioDefTblEntity.getDioPolarity2());
				vo.setPolarity3(dioDefTblEntity.getDioPolarity3());
				vo.setPolarity4(dioDefTblEntity.getDioPolarity4());
			}
			return vo;
		case UniversalStatus.SIGNAL_AIO:
			AioSignalInstanceVo aVo = new AioSignalInstanceVo();
			AiTblEntity aiT = aiTblRepository.findOne(signalLabel);
			//查询AIO点模板
			if(aiT.getPointTpLabel()!=null) {
				AioTpTbl aioTT = aioTpTblRepository.findOne(aiT.getPointTpLabel());
				if(aioTT!=null) {
					aVo.setAioTpLabel(aioTT.getAioTpLabel());
					aVo.setAioTpName(aioTT.getAioTpName());
				}
			}
			if(aiT!=null) {
				BeanTools.copy(aiT, aVo);
				aVo.setAioLabel(aiT.getAiLabel());
				aVo.setAioName(aiT.getAiName());
			}
			AoTbl aoT = aoTblRepository.findOne(signalLabel);
			if(aoT!=null) {
				BeanTools.copy(aoT, aVo);
				aVo.setCtrlWaitAct(aoT.getWaitAct());
				aVo.setCtrlTimeout(aoT.getExecTimeout());
				aVo.setCtrlFormat(aoT.getAoFormat());
				aVo.setCtrlTolerance(aoT.getAoTolerance());
				aVo.setCtrlDataType(aoT.getDataType());
				aVo.setCtrlBottomScale(aoT.getBottomScale());
				aVo.setCtrlTopScale(aoT.getTopScale());
				aVo.setCtrlBottomCode(aoT.getBottomCode());
				aVo.setCtrlTopCode(aoT.getTopCode());
				aVo.setCtrlAoBase(aoT.getAoBase());
				aVo.setCtrlAoCoeff(aoT.getAoCoeff());
			}
			AioDefTblEntity aioDefT = aioDefTblRepository.findOne(signalLabel);
			if(aioDefT!=null) {
				BeanTools.copy(aioDefT, aVo);
			}
			AiLimitTblEntity aiLimitT = aiLimitTblRepository.findOne(signalLabel);
			if(aiLimitT!=null) {
				BeanTools.copy(aiLimitT, aVo);
			}
			return aVo;
		case UniversalStatus.SIGNAL_ACC:
			AccSignalInstanceVo accVo = new AccSignalInstanceVo();
			AccTbl accT = accTblRepository.findOne(signalLabel);
			//查询ACC点模板
			if(accT.getPointTpLabel()!=null) {
				AccTpTbl accTT = accTpTblRepository.findOne(accT.getPointTpLabel());
				if(accTT!=null) {
					accVo.setAccTpLabel(accTT.getAccTpLabel());
					accVo.setAccTpName(accTT.getAccTpName());
				}
			}
			if(accT!=null) {
				BeanTools.copy(accT, accVo);
			}
			AccDefTblEntity accDefT = accDefTblRepository.findOne(signalLabel);
			if(accDefT!=null) {
				BeanTools.copy(accDefT, accVo);
			}
			AccLimitTblEntity accLT = new AccLimitTblEntity();
			if(accLT!=null) {
				BeanTools.copy(accLT, accVo);
			}
			return accVo;
		case UniversalStatus.SIGNAL_MIX:
			//TODO mix 模板逻辑暂定

			return null;
		default:
			throw new Exception("不合法的入参");
		}
	}
	/**
	 * 
	 * @Title: listPointInsatnceByDev
	 * @Description: 查询某个设备实例的所有信号实例
	 * @author Cookie
	 * @date 2018年3月30日 上午9:12:16
	 * @company 上海艾文普信息技术有限公司
	 * @param devLabel
	 */
	@Override
	public List<GetSignalInsatnceListDto> getSignalInstanceList(String devLabel) {
		//获取所有点实例(只需要获取DI,AI,ACC)
		List<GetSignalInsatnceListDto> res = new ArrayList<>();
		DiTbl diT = new DiTbl();
		diT.setDevLabel(devLabel);
		List<DiTbl> diTblList=diTblRepository.findAll(Example.of(diT));
        if(diTblList!=null && diTblList.size()>0){
			for(DiTbl d : diTblList) {
				GetSignalInsatnceListDto dto = new GetSignalInsatnceListDto();
				dto.setSignaId(d.getDiLabel());
				dto.setSignaName(d.getDiName());
				dto.setSignaType(UniversalStatus.SIGNAL_DIO);
				dto.setDevInstanceId(devLabel);
				dto.setIfSpecail(d.getPointTpLabel() == null ? UniversalStatus.ifSpecial : UniversalStatus.noSpecial);
				res.add(dto);
			}
		}
		AiTblEntity aiT = new AiTblEntity();
		aiT.setDevLabel(devLabel);
        List<AiTblEntity> aiTblEntityList=aiTblRepository.findAll(Example.of(aiT));
        if(aiTblEntityList!=null && aiTblEntityList.size()>0){
			for(AiTblEntity d :aiTblEntityList) {
				GetSignalInsatnceListDto dto = new GetSignalInsatnceListDto();
				dto.setSignaId(d.getAiLabel());
				dto.setSignaName(d.getAiName());
				dto.setSignaType(UniversalStatus.SIGNAL_AIO);
				dto.setDevInstanceId(devLabel);
				dto.setIfSpecail(d.getPointTpLabel()==null?UniversalStatus.ifSpecial:UniversalStatus.noSpecial);
				res.add(dto);
			}
		}
		AccTbl accT = new AccTbl();
		accT.setDevLabel(devLabel);
		List<AccTbl> accTblList=accTblRepository.findAll(Example.of(accT));
		if(accTblList!=null && accTblList.size()>0) {
			for (AccTbl d : accTblList) {
				GetSignalInsatnceListDto dto = new GetSignalInsatnceListDto();
				dto.setSignaId(d.getAccLabel());
				dto.setSignaName(d.getAccName());
				dto.setSignaType(UniversalStatus.SIGNAL_ACC);
				dto.setDevInstanceId(devLabel);
				dto.setIfSpecail(d.getPointTpLabel() == null ? UniversalStatus.ifSpecial : UniversalStatus.noSpecial);
				res.add(dto);
			}
		}
		//TODO mix模板逻辑暂定
		return res;
	}
	
	/**
	 * 
	 * @Title: updateExtendsSignalInstance
	 * @Description: 修改继承信号实例
	 * @author Cookie
	 * @date 2018年4月4日 上午9:43:25
	 * @company 上海艾文普信息技术有限公司
	 * @param type
	 * @param o
	 * @return
	 * @throws Exception 
	 */
	@Override
	public Object updateExtendsSignalInstance(int type, Object o) throws Exception {
		switch (type) {
		case UniversalStatus.SIGNAL_DIO:
			return this.updateExtendsDioSignalInstance((DioSignalInstanceVo)o);
		case UniversalStatus.SIGNAL_AIO:
		    return this.updateExtendsAioSignalInstance((AioSignalInstanceVo)o);
		case UniversalStatus.SIGNAL_ACC:
			return this.updateExtendsAccSignalInstance((AccSignalInstanceVo)o);
		case UniversalStatus.SIGNAL_MIX:
			//TODO mix 模板逻辑暂定

			break;
		default:
			throw new Exception("不合法的入参");
		}
		return null;
	}
	

	//------------------------------ 自定义方法 --------------------------------
	private void  saveDi(DioSignalInstanceVo dVo,DevTbl devT){
		DiTbl diT = new DiTbl();
		BeanTools.copy(dVo, diT);
		diT.setDiLabel(dVo.getDioLabel());
		diT.setDevTypeLabel(devT.getDevTypeLabel());
		diT.setLocationId(devT.getStationId());
		diT.setDomainId(devT.getDomainId());
		diT.setProSystemId(devT.getProSystemId());
		diT.setRegionId(devT.getRegionId());
		diT.setCtrlTransGrpLabel(devT.getCtrlTransGrpLabel());
		diT.setDiName(dVo.getDioName());
		diT.setOperateTime(new Timestamp(System.currentTimeMillis()));
		this.diTblRepository.save(diT);
	}

	public void saveDioDef(DioSignalInstanceVo dVo,DevTbl devT){
		DioDefTblEntity dioDefT = new DioDefTblEntity();
		BeanTools.copy(dVo, dioDefT);
		dioDefT.setDioDefLabel(dVo.getDioLabel());
		dioDefT.setLocationId(devT.getStationId());
		dioDefT.setProSystemId(devT.getProSystemId());
		dioDefT.setDomainId(devT.getDomainId());
		dioDefT.setDevTypeLabel(devT.getDevTypeLabel());
		dioDefT.setDioPolarity2(dVo.getPolarity2());
		dioDefT.setDioPolarity3(dVo.getPolarity3());
		dioDefT.setDioPolarity4(dVo.getPolarity4());
		dioDefT.setOperateTime(new Timestamp(System.currentTimeMillis()));
		this.dioDefTblRepository.save(dioDefT);
	}

	private  void saveDo(DioSignalInstanceVo dVo){
		DoTbl doT = new DoTbl();
		BeanTools.copy(dVo, doT);
		doT.setDoLabel(dVo.getDioLabel());
		doT.setExecTimeout(dVo.getCtrlTimeout());
		doT.setDoGrpLabel(dVo.getCtrlGrpLabel());
		doT.setWaitAct(dVo.getCtrlWaitAct());
		doT.setOperateTime(new Timestamp(System.currentTimeMillis()));
		this.doTblRepository.save(doT);
	}

	/**
	 * 
	 * @Title: addDioSignaInstance
	 * @Description: 新增Dio信号实例
	 * @author Cookie
	 * @date 2018年4月2日 上午8:46:57
	 * @company 上海艾文普信息技术有限公司
	 * @throws Exception
	 */
	private DioSignalInstanceVo dioSignalInstance(DioSignalInstanceVo dVo) throws Exception {
        //查询是否已存在该信号实例
        DiTbl diTbl=diTblRepository.findOne(dVo.getDioLabel());
        if(diTbl!=null){
			throw new Exception("已存在此名称信号实例");
		}
		//得到设备实例
		DevTbl devT = devTblRepository.findOne(dVo.getDevLabel());
        this.saveDi(dVo,devT);
        this.saveDioDef(dVo,devT);
		//新增Do表(用过isControl来判断是否生成do表)
		if(dVo.getIsControl()==null||dVo.getIsControl()) {
           this.saveDo(dVo);
		}
        return dVo;
		//mix表逻辑暂定
	}

	private void saveAi(AioSignalInstanceVo aVo,DevTbl devT){
		AiTblEntity aiT = new AiTblEntity();
		BeanTools.copy(aVo, aiT);
		aiT.setAiLabel(aVo.getAioLabel());
		aiT.setDevLabel(devT.getDevLabel());
		aiT.setDevType(devT.getDevTypeLabel());
		aiT.setLocationId(devT.getStationId());
		aiT.setDomainId(devT.getDomainId());
		aiT.setProSystemId(devT.getProSystemId());
		aiT.setRegionId(devT.getRegionId());
		aiT.setAiName(aVo.getAioName());
		aiT.setCtrlTransGrpLabel(devT.getCtrlTransGrpLabel());
		aiT.setOperateTime(new Timestamp(System.currentTimeMillis()));
		this.aiTblRepository.save(aiT);
	}
	private void saveAo(AioSignalInstanceVo aVo){
		AoTbl aoT = new AoTbl();
		BeanTools.copy(aVo, aoT);
		aoT.setAoLabel(aVo.getAioLabel());
		aoT.setWaitAct(aVo.getCtrlWaitAct());
		aoT.setExecTimeout(aVo.getCtrlTimeout());
		aoT.setAoFormat(aVo.getCtrlFormat());
		aoT.setAoTolerance(aVo.getCtrlTolerance());
		aoT.setDataType(aVo.getCtrlDataType());
		aoT.setBottomCode(aVo.getCtrlBottomCode());
		aoT.setTopScale(aVo.getCtrlTopScale());
		aoT.setBottomScale(aVo.getCtrlBottomScale());
		aoT.setTopCode(aVo.getCtrlTopCode());
		aoT.setAoBase(aVo.getCtrlAoBase());
		aoT.setAoCoeff(aVo.getCtrlAoCoeff());
		aoT.setOperateTime(new Timestamp(System.currentTimeMillis()));
		this.aoTblRepository.save(aoT);
	}
	private  void saveAioDef(AioSignalInstanceVo aVo,DevTbl devT){
		AioDefTblEntity aioDefT = new AioDefTblEntity();
		BeanTools.copy(aVo, aioDefT);
		aioDefT.setAioDefLabel(aVo.getAioLabel());
		aioDefT.setLocationId(devT.getStationId());
		aioDefT.setDomainId(devT.getDomainId());
		aioDefT.setProSystemId(devT.getProSystemId());
		aioDefT.setDevTypeLabel(devT.getDevTypeLabel());
		aioDefT.setOperateTime(new Timestamp(System.currentTimeMillis()));
		this.aioDefTblRepository.save(aioDefT);
	}
	private void saveAiLimit(AioSignalInstanceVo aVo,DevTbl devT){
		AiLimitTblEntity aiLimitT = new AiLimitTblEntity();
		BeanTools.copy(aVo, aiLimitT);
		aiLimitT.setAiLimitLabel(aVo.getAioLabel());
		aiLimitT.setDomainId(devT.getDomainId());
		aiLimitT.setLocationId(devT.getStationId());
		aiLimitT.setProSystemId(devT.getProSystemId());
		aiLimitT.setOperateTime(new Timestamp(System.currentTimeMillis()));
		this.aiLimitTblRepository.save(aiLimitT);
	}
	/**
	 * 
	 * @Title: addAioSignaInstance
	 * @Description: 增加Aio信号实例
	 * @author Cookie
	 * @date 2018年4月2日 上午9:49:38
	 * @company 上海艾文普信息技术有限公
	 * @throws Exception
	 */
	private AioSignalInstanceVo addAioSignalInstance(AioSignalInstanceVo aVo) throws Exception {
		DevTbl devT = devTblRepository.findOne(aVo.getDevLabel());
		AiTblEntity aiTblEntity=aiTblRepository.findOne(aVo.getAioLabel());
		if(aiTblEntity!=null) {
			throw new Exception("已存在此名称信号实例");
		}
		this.saveAi(aVo,devT);
		this.saveAioDef(aVo,devT);
		this.saveAiLimit(aVo,devT);
		//新增ao表(通过ifContro字段来判断是否生成ao表)
		if(aVo.getIsControl()==null||aVo.getIsControl()) {
            this.saveAo(aVo);
		}
		return aVo;
	}

	private void saveAcc(AccSignalInstanceVo aVo,DevTbl devT){
		AccTbl accT = new AccTbl();
		BeanTools.copy(aVo, accT);
		accT.setDevTypeLabel(devT.getDevTypeLabel());
		accT.setDomainId(devT.getDomainId());
		accT.setLocationId(devT.getStationId());
		accT.setProSystemId(devT.getProSystemId());
		accT.setRegionId(devT.getRegionId());
		accT.setOperateTime(new Timestamp(System.currentTimeMillis()));
		this.accTblRepository.save(accT);
	}
	private  void saveAccLimit(AccSignalInstanceVo aVo,DevTbl devT){
		AccLimitTblEntity accLT = new AccLimitTblEntity();
		BeanTools.copy(aVo, accLT);
		accLT.setAccLimitLabel(aVo.getAccLabel());
		accLT.setDomainId(devT.getDomainId());
		accLT.setLocationId(devT.getStationId());
		accLT.setProSystemId(devT.getProSystemId());
		accLT.setRegionId(devT.getRegionId());
		accLT.setOperateTime(new Timestamp(System.currentTimeMillis()));
		this.accLimitTblRepository.save(accLT);
	}

	private void saveAccDef(AccSignalInstanceVo aVo,DevTbl devT){
		AccDefTblEntity accDT = new AccDefTblEntity();
		BeanTools.copy(aVo, accDT);
		accDT.setAccDefLabel(aVo.getAccLabel());
		accDT.setDomainId(devT.getDomainId());
		accDT.setLocationId(devT.getStationId());
		accDT.setProSystemId(devT.getProSystemId());
		accDT.setDevTypeLabel(devT.getDevTpLabel());
		accDT.setOperateTime(new Timestamp(System.currentTimeMillis()));
		this.accDefTblRepository.save(accDT);
	}

	/**
	 * 
	 * @Title: addAccSignaInstance
	 * @Description: 增加Acc信号实例
	 * @author Cookie
	 * @date 2018年4月2日 上午10:43:34
	 * @company 上海艾文普信息技术有限公司
	 * @throws Exception
	 */
	private AccSignalInstanceVo addAccSignalInstance(AccSignalInstanceVo aVo) throws Exception {
		//增加特殊信号实例
		DevTbl devT = devTblRepository.findOne(aVo.getDevLabel());
		if(accTblRepository.findOne(aVo.getAccLabel())!=null) {
			throw new Exception("已存在此名称信号实例");
		}
		this.saveAcc(aVo,devT);
		this.saveAccLimit(aVo,devT);
		this.saveAccDef(aVo,devT);
		return aVo;
	}

	/**
	 * 
	 * @Title: updateDioSignaInstance
	 * @Description: 修改Dio模板信号实例
	 * @author Cookie
	 * @date 2018年4月2日 下午5:06:06
	 * @company 上海艾文普信息技术有限公司
	 */
	public DioSignalInstanceVo updateDioSignalInstance(DioSignalInstanceVo dVo ) {
		DevTbl devT = devTblRepository.findOne(dVo.getDevLabel());
		//修改di表
	    this.saveDi(dVo,devT);
		//修改dio_def表
		this.saveDioDef(dVo,devT);
		//修改Do表(若Do表之前未存在，需要新增或不操作，若已存在，则删除或者修改。)
		DoTbl doTbl=this.doTblRepository.findOne(dVo.getDioLabel());
		if(doTbl==null) {
			if(dVo.getIsControl()==null||dVo.getIsControl()) {
				this.saveDo(dVo);
			}
		}else {
			if(dVo.getIsControl()) {
				this.saveDo(dVo);
			}else {
				doTblRepository.deleteByDoLabel(dVo.getDioLabel());
			}
		}
		return dVo;
	}
	/**
	 * 
	 * @Title: updateAioSignaInstance
	 * @Description: 跟新AIO模板信号实例
	 * @author Cookie
	 * @date 2018年4月2日 下午5:35:00
	 * @company 上海艾文普信息技术有限公司
	 */
	public AioSignalInstanceVo updateAioSignalInstance(AioSignalInstanceVo aVo) {
		DevTbl devT = devTblRepository.findOne(aVo.getDevLabel());
		//修改Ai表
		this.saveAi(aVo,devT);
		//修改Aio_def表
		this.saveAioDef(aVo,devT);
		//修改Aio_limit表
		this.saveAiLimit(aVo,devT);
		//修改Ao表(若Ao表之前未存在，需要新增或不操作，若已存在，则删除或者修改。)
		AoTbl aoTbl=this.aoTblRepository.findOne(aVo.getAioLabel());
		if(aoTbl==null) {
			if(aVo.getIsControl()==null||aVo.getIsControl()) {
				this.saveAo(aVo);
			}
		}else {
			if(aVo.getIsControl()) {
				this.saveAo(aVo);
			}else {
				aoTblRepository.deleteByAoLabel(aVo.getAioLabel());
			}
		}
		return aVo;
	}
	
	/**
	 * 
	 * @Title: updateAccSignaInstance
	 * @Description: 修改Acc模板信号实例
	 * @author Cookie
	 * @date 2018年4月2日 下午5:44:20
	 * @company 上海艾文普信息技术有限公司
	 */
	public AccSignalInstanceVo updateAccSignalInstance(AccSignalInstanceVo aVo) {
		DevTbl devT = devTblRepository.findOne(aVo.getDevLabel());
		//修改Acc表
		this.saveAcc(aVo,devT);
		//修改acc_limit表
		this.saveAccLimit(aVo,devT);
		//修改acc_def表
		this.saveAccDef(aVo,devT);
		return aVo;
	}

	/**
	 * 
	 * @Title: updateExtendsAioSignalInstance
	 * @Description: 修改aio模板下继承信号实例
	 * @author Cookie
	 * @date 2018年4月4日 上午10:06:29
	 * @company 上海艾文普信息技术有限公司
	 * @param aVo
	 * @return
	 */
	public AioSignalInstanceVo updateExtendsAioSignalInstance(AioSignalInstanceVo aVo) {
		DevTbl devT = devTblRepository.findOne(aVo.getDevLabel());
		//修改ai表
		this.saveAi(aVo,devT);
		//修改ao表
		this.saveAo(aVo);
		//修改aio_def表
		this.saveAioDef(aVo,devT);
		//修改ai_limit表
		this.saveAiLimit(aVo,devT);
		return aVo;
	}
	
	/**
	 * 
	 * @Title: updateExtendsDioSignalInstance
	 * @Description: 修改dio模板下继承信号实例
	 * @author Cookie
	 * @date 2018年4月4日 上午10:07:26
	 * @company 上海艾文普信息技术有限公司
	 * @param dVo
	 * @return
	 */
	public DioSignalInstanceVo updateExtendsDioSignalInstance(DioSignalInstanceVo dVo) {
		DevTbl devT = devTblRepository.findOne(dVo.getDevLabel());
		//修改di表
		this.saveDi(dVo,devT);
		//修改do
		this.saveDo(dVo);
		//修改dio_def表
		this.saveDioDef(dVo,devT);
		return dVo;
	}
	
	/**
	 * 
	 * @Title: updateExtendsAccSignalInstance
	 * @Description: 修改Acc模板下信号实例
	 * @author Cookie
	 * @date 2018年4月4日 下午2:35:55
	 * @company 上海艾文普信息技术有限公司
	 * @param aVo
	 * @return
	 */
	public AccSignalInstanceVo updateExtendsAccSignalInstance(AccSignalInstanceVo aVo) {
		DevTbl devT = devTblRepository.findOne(aVo.getDevLabel());
		//修改ac表
		this.saveAcc(aVo, devT);
		//修改acc_def表
		this.saveAccDef(aVo,devT);
		//修改acc_limit表
		this.saveAccLimit(aVo,devT);
		return aVo;
	}

	
}
