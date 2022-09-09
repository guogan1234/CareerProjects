package com.avp.config.service.impl;

import java.sql.Timestamp;
import java.util.ArrayList;
import java.util.List;

import com.avp.config.dao.*;
import com.avp.config.dao.system.DomainTblRepository;
import com.avp.config.dao.system.ProSystemTblRepository;
import com.avp.config.dao.system.StationTblRepository;
import com.avp.config.entity.*;
import com.avp.config.entity.vo.AccSignalInstanceVo;
import com.avp.config.entity.vo.AioSignalInstanceVo;
import com.avp.config.entity.vo.DevCopyVo;
import com.avp.config.entity.vo.DioSignalInstanceVo;
import com.avp.config.service.SignalInstanceService;
import com.avp.config.util.UniversalStatus;
import org.apache.commons.lang.StringUtils;
import org.springframework.beans.BeanUtils;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.data.domain.Example;
import org.springframework.stereotype.Service;
import org.springframework.transaction.annotation.Transactional;

import com.avp.config.entity.dto.AddDevDto;
import com.avp.config.service.DevTblService;
import com.avp.config.util.BeanTools;

@Service
@Transactional(rollbackFor = Exception.class)
public class DevTblServiceImpl implements DevTblService{

	@Autowired
	private DevTblRepository devTblRepository;
	@Autowired
	private AiTblRepository aiTblRepository;
	@Autowired
	private AoTblRepository aoTblRepository;
	@Autowired
	private DiTblRepository diTblRepository;
	@Autowired
	private DoTblRepository doTblRepository;
	@Autowired
	private AccTblRepository accTblRepository;
	@Autowired
	private DioDefTblRepository dioDefTblRepository;
	@Autowired
	private AioDefTblRepository aioDefTblRepository;
	@Autowired
	private AiLimitTblRepository aiLimitTblRepository;
	@Autowired
	private AccLimitTblRepository accLimitTblRepository;
	@Autowired
	private AccDefTblRepository accDefTblRepository;
	@Autowired
	private DomainTblRepository domainTblRepository;
	@Autowired
	private StationTblRepository stationTblRepository;
	@Autowired
	private ProSystemTblRepository proSystemTblRepository;
	@Autowired
	private DevTpTblRepository devTpTblRepository;
	@Autowired
	private AioTpTblRepository aioTpTblRepository;
	@Autowired
	private DioTpTblRepository dioTpTblRepository;
	@Autowired
	private AccTpTblRepository accTpTblRepository;
	@Autowired
	private SignalInstanceService signalInstanceService;


	/**
	 * 
	 * @Title: addDev
	 * @Description: 新增设备实例
	 * @author Cookie
	 * @date 2018年3月28日 上午10:01:31
	 * @company 上海艾文普信息技术有限公司
	 * @param param
	 * @throws Exception 
	 */
	@Override
	public AddDevDto addDev(DevTbl param) throws Exception {
		//此处需要重新拼接devLaBel名：domain_label.station_label.pro_system_label.传入label
		String stationLabel = stationTblRepository.findOne(param.getStationId()).getStationLabel();
		String newName = domainTblRepository.findOne(param.getDomainId()).getDomainLabel()+":"+stationLabel+":"
				+ proSystemTblRepository.findOne(param.getProSystemId()).getProSystemLabel()+":"+param.getDevLabel();
		param.setDevLabel(newName);
		DevTpTbl devTpTbl=this.devTpTblRepository.findByDevTpLabel(param.getDevTpLabel());
		param.setDevTypeLabel(devTpTbl.getDevTypeLabel());
		param.setOperateTime(new Timestamp(System.currentTimeMillis()));
		//查询是否已存在该名称设备
		if(devTblRepository.findOne(param.getDevLabel())!=null) {
			throw new Exception("已存在此名称设备");
		}
		param = devTblRepository.save(param);
		AddDevDto aDto = new AddDevDto();
		BeanTools.copy(param, aDto);
		aDto.setStationLabel(stationLabel);
		//此处需要生成信号实例,查询该实例模板关联的点模板
		//生成所有aio模板下的信号实例
		AioTpTbl aioT = new AioTpTbl();
		aioT.setDevTpLabel(param.getDevTpLabel());
		List<AioTpTbl> aioTs = this.aioTpTblRepository.findAll(Example.of(aioT));
		for(AioTpTbl t:aioTs) {
			AioSignalInstanceVo aioSignalInstanceVo=new AioSignalInstanceVo();
			aioSignalInstanceVo.setAioName(t.getAioTpName());
			aioSignalInstanceVo.setAioLabel(param.getDevLabel()+":"+t.getAioTpLabel().split("\\:")[2]);
			aioSignalInstanceVo.setPointTpLabel(t.getAioTpLabel());
			aioSignalInstanceVo.setDevLabel(param.getDevLabel());
			this.signalInstanceService.updateExtendsSignalInstance(UniversalStatus.SIGNAL_AIO,aioSignalInstanceVo);
		}
		//生成所有dio模板下的信号实例
		DioTpTbl dioT = new DioTpTbl();
		dioT.setDevTpLabel(param.getDevTpLabel());
		List<DioTpTbl> dioTs = dioTpTblRepository.findAll(Example.of(dioT));
		for(DioTpTbl t :dioTs) {
			DioSignalInstanceVo dioSignalInstanceVo=new DioSignalInstanceVo();
			dioSignalInstanceVo.setDioLabel(param.getDevLabel()+":"+t.getDioTpLabel().split("\\:")[2]);
			dioSignalInstanceVo.setDioName(t.getDioTpName());
			dioSignalInstanceVo.setPointTpLabel(t.getDioTpLabel());
			dioSignalInstanceVo.setDevLabel(param.getDevLabel());
			this.signalInstanceService.updateExtendsSignalInstance(UniversalStatus.SIGNAL_DIO,dioSignalInstanceVo);
		}
		//生成所有ACC模板下的信号实例
		AccTpTbl accTT = new AccTpTbl();
		accTT.setDevTpLabel(param.getDevTpLabel());
		List<AccTpTbl> accTs = accTpTblRepository.findAll(Example.of(accTT));
		for(AccTpTbl t:accTs) {
			AccSignalInstanceVo accSignalInstanceVo=new AccSignalInstanceVo();
			accSignalInstanceVo.setAccName(t.getAccTpName());
			accSignalInstanceVo.setAccLabel(param.getDevLabel()+":"+t.getAccTpLabel().split("\\:")[2]);
			accSignalInstanceVo.setPointTpLabel(t.getAccTpLabel());
			accSignalInstanceVo.setDevLabel(param.getDevLabel());
			this.signalInstanceService.updateExtendsSignalInstance(UniversalStatus.SIGNAL_ACC,accSignalInstanceVo);
		}
		return aDto;
	}

	/**
	 * 
	 * @Title: delDev
	 * @Description: 删除设备实例
	 * @author Cookie
	 * @date 2018年3月28日 下午2:20:33
	 * @company 上海艾文普信息技术有限公司
	 * @param devLabel
	 */
	@Override
	public void delDev(String devLabel) {
		//1.删除dev_tbl表中设备实例信息
		devTblRepository.delete(devLabel);
		//2.删除点实例（3-28 拟定删除共五张表数据 3-28 4:20 更改为修改十张表）
		//此处Di,Ai 表中存在devLabel ，Do,Ao 等无devLabel字段，它主键与Di,Ai相同
		DiTbl dT = new DiTbl();
		dT.setDevLabel(devLabel);
		List<DiTbl> dTs = diTblRepository.findAll(Example.of(dT));
		List<DoTbl> doTs = new ArrayList<>();
		List<DioDefTblEntity> a1 = new ArrayList<>();
		for(DiTbl d : dTs) {
			DoTbl dTo = new DoTbl();
			DioDefTblEntity diT = new DioDefTblEntity();
			diT.setDioDefLabel(d.getDevLabel());
			dTo.setDoLabel(d.getDiLabel());
			doTs.add(dTo);
			a1.add(diT);
		}
		//删除Do表中数据
		doTblRepository.deleteInBatch(doTs);
		//删除di表中相关数据
		diTblRepository.deleteInBatch(dTs);
		//删除dio_def表中数据
		dioDefTblRepository.deleteInBatch(a1);

		AiTblEntity aT = new AiTblEntity();
		aT.setDevLabel(devLabel);
		List<AiTblEntity> aTs = aiTblRepository.findAll(Example.of(aT));
		List<AoTbl> aoTs = new ArrayList<>();
		List<AioDefTblEntity> a2 = new ArrayList<>();
		List<AiLimitTblEntity> a3 = new ArrayList<>();
		for(AiTblEntity a : aTs) {
			AoTbl aoT = new AoTbl();
			AioDefTblEntity aiT = new AioDefTblEntity();
			AiLimitTblEntity ailT = new AiLimitTblEntity();
			aoT.setAoLabel(a.getAiLabel());
			aiT.setAioDefLabel(a.getDevLabel());
			ailT.setAiLimitLabel(a.getDevLabel());
			a2.add(aiT);
			a3.add(ailT);
			aoTs.add(aoT);
		}
		//删除Ao表中的数据
		aoTblRepository.deleteInBatch(aoTs);
		//删除ai表中的相关数据
		aiTblRepository.deleteInBatch(aTs);
		//删除aio_def表中数据
		aioDefTblRepository.deleteInBatch(a2);
		//删除aio_limit表中数据
		aiLimitTblRepository.deleteInBatch(a3);

		AccTbl acT = new AccTbl();
		acT.setDevLabel(devLabel);
		List<AccTbl> ats = accTblRepository.findAll(Example.of(acT));
		List<AccDefTblEntity> a4 = new ArrayList<>();
		List<AccLimitTblEntity> a5 = new ArrayList<>();
		for(AccTbl a :ats) {
			AccDefTblEntity acT1 = new AccDefTblEntity();
			AccLimitTblEntity alT = new AccLimitTblEntity();
			acT1.setAccDefLabel(a.getDevLabel());
			alT.setAccLimitLabel(a.getDevLabel());
			a4.add(acT1);
			a5.add(alT);
		}
		//删除acctbl表中数据
		accTblRepository.deleteByDevLabel(devLabel);
		//删除acc_def表中数据
		accDefTblRepository.deleteInBatch(a4);
		//删除acc_limit表中数据
		accLimitTblRepository.deleteInBatch(a5);
	}

	/**
	 * 
	 * @Title: updateDevTbl
	 * @Description: 跟新设备实例
	 * @author Cookie
	 * @date 2018年3月28日 下午3:29:40
	 * @company 上海艾文普信息技术有限公司
	 * @return
	 */
	@Override
	public DevTbl updateDevTbl(DevTbl param) {
		return devTblRepository.saveAndFlush(param);
	}

	/**
	 * 
	 * @Title: queryDevTbl
	 * @Description: 查询单个设备实例
	 * @author Cookie
	 * @date 2018年3月28日 下午3:34:36
	 * @company 上海艾文普信息技术有限公司
	 * @param devLabel
	 * @return
	 */
	@Override
	public DevTbl queryDevTbl(String devLabel) {
		return devTblRepository.findOne(devLabel);
	}

	/**
	 * 
	 * @Title: getDevList
	 * @Description: 查询某个某设备模板下的（车站）设备实例集合
	 * @author Cookie
	 * @date 2018年3月29日 下午2:32:47
	 * @company 上海艾文普信息技术有限公司
	 * @param devTpLabel
	 * @param stationId
	 * @return
	 * @throws Exception 
	 */
	@Override
	public List<DevTbl> getDevList(String devTpLabel, Integer stationId) throws Exception {
		if(StringUtils.isBlank(devTpLabel)) {
			throw new Exception("设备类型不能为空");
		}
		DevTbl dT = new DevTbl();
		dT.setDevTpLabel(devTpLabel);
		dT.setStationId(stationId);
		dT.setOperateTime(null);
		return devTblRepository.findAll(Example.of(dT));
	}

	/**
	 * 
	 * @Title: copyDevInstance
	 * @Description: 复制设备实例
	 * @author Cookie
	 * @date 2018年4月10日 上午11:36:18
	 * @company 上海艾文普信息技术有限公司
	 * @param vo
	 * @return
	 * @throws Exception
	 */
	@Override
	public Object copyDevInstance(DevCopyVo vo) throws Exception {
		//1.复制设备实例本身(以及继承信号实例)
		DevTbl devT = devTblRepository.findOne(vo.getSourceInstanceLabel());

		DevTbl devTbl=new DevTbl();
		BeanUtils.copyProperties(devT,devTbl);
		devTbl.setDevLabel(vo.getDevInstanceLabel());
		devTbl.setDevName(vo.getDevInstanceName());
		this.addDev(devTbl);
		//2.复制设备实例的特殊信号实例
		this.copySpecialSignalInstance(devTbl.getDevLabel(),vo.getSourceInstanceLabel());
		return devTbl;
	}

	/**
	 * 
	 * @Title: copyStation
	 * @Description: 复制车站
	 * @author Cookie
	 * @date 2018年4月11日 下午1:49:18
	 * @company 上海艾文普信息技术有限公司
	 * @param vo
	 * @return
	 * @throws Exception
	 */
	@Override
	public Object copyStation(DevCopyVo vo) throws Exception {
		//查询被复制车站的设备实例信息(此处需要加上域ID)
		DevTbl devT = new DevTbl();
		devT.setStationId(vo.getSourceStationId());
		devT.setDomainId(vo.getSourceDomainId());
		List<DevTbl> devTs = devTblRepository.findAll(Example.of(devT));
		for(DevTbl d : devTs) {
			DevTbl devTbl=new DevTbl();
			BeanUtils.copyProperties(d,devTbl);
			//复制设备实例重构devLabel
			//1.复制设备实例本身(以及他的继承信号实例)
			//d.setStationId(vo.getLocationId());
            devTbl.setDomainId(vo.getDomainId());
            devTbl.setStationId(vo.getStationId());
            devTbl.setDevLabel(devTbl.getDevLabel().split("\\:")[3]);
			this.addDev(devTbl);
			//2.复制设备实例的特殊信号实例
			this.copySpecialSignalInstance(devTbl.getDevLabel(),d.getDevLabel());
		}
		return vo;
	}

	/**
	 * 
	 * @Title: copyDevInstanceList
	 * @Description: 批量生成设备实例
	 * @author Cookie
	 * @date 2018年4月11日 下午1:50:15
	 * @company 上海艾文普信息技术有限公司
	 * @param vo
	 * @return
	 * @throws Exception
	 */
	@Override
	public Object copyDevInstanceList(DevCopyVo vo) throws Exception {
		//根据自增类型确定devlabel
		if(vo.getIncreasedType().equals(UniversalStatus.numberAdd)) {
			for(int i = 0;i<vo.getCount();i++) {
				DevTbl devT = new DevTbl();
				BeanTools.copy(vo, devT);
				devT.setDevLabel(vo.getDevInstanceLabel());
				devT.setDevName(vo.getDevInstanceName()+(Integer.valueOf(vo.getStarInput())+vo.getStep()*i));
				devT.setDevLabel(devT.getDevLabel()+(Integer.valueOf(vo.getStarInput())+vo.getStep()*i));
				this.addDev(devT);
			}
		}else if(vo.getIncreasedType().equals(UniversalStatus.letterAdd)){
			char letterInput = vo.getStarInput().charAt(0);
			String addLabel = null;
			for(int i = 0;i<vo.getCount();i++) {
				//判断是大写字符还是小写字母
				if(Character.isUpperCase(letterInput)) {
					//大写字母
					addLabel = this.numberToLetter(true, (Integer.valueOf(letterInput)+vo.getStep()*i)-64);
				}else if(Character.isLowerCase(letterInput)) {
					//小写字母
					addLabel = this.numberToLetter(true, (Integer.valueOf(letterInput)+vo.getStep()*i)-96);
				}
				DevTbl devT = new DevTbl();
				BeanTools.copy(vo, devT);
                devT.setDevLabel(vo.getDevInstanceLabel());
                devT.setDevName(vo.getDevInstanceName()+addLabel);
				devT.setDevLabel(devT.getDevLabel()+addLabel);
				this.addDev(devT);
			}
		}
		return null;
	}

	//-------------------------自定义方法-----------------------
	/**
	 * 
	 * @Title: numberToLetter
	 * @Description: 将数字转换为指定递增格式字母
	 * @author Cookie
	 * @date 2018年4月12日 下午2:13:21
	 * @company 上海艾文普信息技术有限公司
	 * @param isBig
	 * @param number
	 * @return
	 */
	public String numberToLetter(Boolean isBig,int number) {
		String result = "";
		if(isBig) {
			List<Integer> numList = new ArrayList<>();
			while(number>0) {
				numList.add(number%26==0?26:number%26);
				if(number%26==0) {
					number = number -1;
				}
				number=number/26;
			}
			for(Integer i : numList) {
				result = (char)(i+64)+result;
			}
		}else {
			List<Integer> numList = new ArrayList<>();
			while(number>0) {
				numList.add(number%26==0?26:number%26);
				if(number%26==0) {
					number = number -1;
				}
				number=number/26;
			}
			for(Integer i : numList) {
				result = (char)(i+96)+result;
			}
		}
		return result;
	}

	/**
	 * 
	 * @Title: copySpecialSignalInstance
	 * @Description: 复制设备实例下所有特殊信号实例
	 * @author Cookie
	 * @date 2018年4月12日 下午3:28:55
	 * @company 上海艾文普信息技术有限公司
	 * @param devLabel
	 * @param sourceInstanceLabel
	 */
	public void copySpecialSignalInstance(String devLabel,String sourceInstanceLabel) {
		//复制Aio模板下的所有信号实例
		AiTblEntity aiT = new AiTblEntity();
		aiT.setDevLabel(sourceInstanceLabel);
		List<AiTblEntity> aiTs = aiTblRepository.findAll(Example.of(aiT));
		for(AiTblEntity a : aiTs) {
			if(a.getPointTpLabel()!=null) { continue; }
			//插入aio_def ai_limit ao ai
			String newLabel = devLabel+":"+a.getAiLabel().split("\\:")[a.getAiLabel().split("\\:").length-1];
			AioDefTblEntity aioDefT = aioDefTblRepository.findOne(a.getAiLabel());
			if(aioDefT!=null) {
				AioDefTblEntity aioDefTblEntity=new AioDefTblEntity();
				BeanUtils.copyProperties(aioDefT,aioDefTblEntity);
				aioDefTblEntity.setAioDefLabel(newLabel);
				aioDefTblEntity.setOperateTime(new Timestamp(System.currentTimeMillis()));
				aioDefTblRepository.save(aioDefTblEntity);
			}
			AoTbl aoT = aoTblRepository.findOne(a.getAiLabel());
			if(aoT!=null) {
				AoTbl aoTbl=new AoTbl();
				BeanUtils.copyProperties(aoT,aoTbl);
				aoTbl.setAoLabel(newLabel);
				aoTbl.setOperateTime(new Timestamp(System.currentTimeMillis()));
				aoTblRepository.save(aoTbl);
			}
			AiLimitTblEntity aiLT = aiLimitTblRepository.findOne(a.getAiLabel());
			if(aiLT!=null) {
				AiLimitTblEntity aiLimitTblEntity=new AiLimitTblEntity();
				BeanUtils.copyProperties(aiLT,aiLimitTblEntity);
				aiLimitTblEntity.setAiLimitLabel(newLabel);
				aiLimitTblEntity.setOperateTime(new Timestamp(System.currentTimeMillis()));
				aiLimitTblRepository.save(aiLimitTblEntity);
			}
			AiTblEntity aiTblEntity=new AiTblEntity();
			BeanUtils.copyProperties(a,aiTblEntity);
			aiTblEntity.setAiLabel(newLabel);
			aiTblEntity.setDevLabel(devLabel);
			aiTblEntity.setOperateTime(new Timestamp(System.currentTimeMillis()));
			aiTblRepository.save(aiTblEntity);
		}
		//复制dio模板下的所有信号实例
		DiTbl diT = new DiTbl();
		diT.setDevLabel(sourceInstanceLabel);
		List<DiTbl> diTs = diTblRepository.findAll(Example.of(diT));
		for(DiTbl d : diTs) {
			if(d.getPointTpLabel()!=null) { continue; }
			String newLabel = devLabel+":"+d.getDiLabel().split("\\:")[d.getDiLabel().split("\\:").length-1];
			//复制 do di dio_def
			DoTbl doT = doTblRepository.findOne(d.getDiLabel());
			if(doT!=null) {
				DoTbl doTbl=new DoTbl();
				BeanUtils.copyProperties(doT,doTbl);
				doTbl.setDoLabel(newLabel);
				doTbl.setOperateTime(new Timestamp(System.currentTimeMillis()));
				doTblRepository.save(doTbl);
			}
			DioDefTblEntity dioDefT = dioDefTblRepository.findOne(d.getDiLabel());
			if(dioDefT!=null) {
				DioDefTblEntity dioDefTblEntity=new DioDefTblEntity();
				BeanUtils.copyProperties(dioDefT,dioDefTblEntity);
				dioDefTblEntity.setDioDefLabel(newLabel);
				dioDefTblEntity.setOperateTime(new Timestamp(System.currentTimeMillis()));
				dioDefTblRepository.save(dioDefTblEntity);
			}
			DiTbl diTbl=new DiTbl();
			BeanUtils.copyProperties(d,diTbl);
			diTbl.setDiLabel(newLabel);
			diTbl.setDevLabel(devLabel);
			diTbl.setOperateTime(new Timestamp(System.currentTimeMillis()));
			diTblRepository.save(diTbl);
		}
		//复制Acc模板下的所有信号实例
		AccTbl accT = new AccTbl();
		accT.setDevLabel(sourceInstanceLabel);
		List<AccTbl> accTs = accTblRepository.findAll(Example.of(accT));
		for(AccTbl a : accTs) {
			//复制acc acc_def acc_limit
			if(a.getPointTpLabel()!=null) { continue; }
			String newLabel = devLabel+":"+a.getAccLabel().split("\\:")[a.getAccLabel().split("\\:").length-1];
			AccDefTblEntity accDT = accDefTblRepository.findOne(a.getAccLabel());
			if(accDT!=null) {
				AccDefTblEntity accDefTblEntity=new AccDefTblEntity();
				BeanUtils.copyProperties(accDT,accDefTblEntity);
				accDefTblEntity.setAccDefLabel(newLabel);
				accDefTblEntity.setOperateTime(new Timestamp(System.currentTimeMillis()));
				accDefTblRepository.save(accDefTblEntity);
			}
			AccLimitTblEntity accLT = accLimitTblRepository.findOne(a.getAccLabel());
			if(accLT!=null) {
				AccLimitTblEntity accLimitTblEntity=new AccLimitTblEntity();
				BeanUtils.copyProperties(accLT,accLimitTblEntity);
				accLimitTblEntity.setAccLimitLabel(newLabel);
				accLimitTblEntity.setOperateTime(new Timestamp(System.currentTimeMillis()));
				accLimitTblRepository.save(accLimitTblEntity);
			}

			AccTbl accTbl=new AccTbl();
			BeanUtils.copyProperties(a,accTbl);
			accTbl.setAccLabel(newLabel);
			accTbl.setDevLabel(devLabel);
			accTbl.setOperateTime(new Timestamp(System.currentTimeMillis()));
			accTblRepository.save(accTbl);
		}
	}

}
