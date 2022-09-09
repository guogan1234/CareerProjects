package com.avp.config.service.impl;

import java.util.List;

import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Service;
import org.springframework.transaction.annotation.Transactional;

import com.avp.config.dao.AioTpTblRepository;
import com.avp.config.entity.AioTpTbl;
import com.avp.config.service.AioTpTblService;

@Service
@Transactional(rollbackFor = Exception.class)
public class AioTpTblServiceImpl implements AioTpTblService {
	
	@Autowired
	private AioTpTblRepository aioTpTblRepository;
	
    @Override
    public void save(AioTpTbl aioTpTbl) {
    	aioTpTblRepository.save(aioTpTbl);
    }

    @Override
    public void update(AioTpTbl aioTpTbl) {
    	aioTpTblRepository.save(aioTpTbl);
    }

    @Override
    public void del(String aioTpLabel) {
    	//此处删除点模板（当点模板下有点实例时不允许删除，逻辑待定）
    	AioTpTbl aTbl = new AioTpTbl();
    	aTbl.setAioTpLabel(aioTpLabel);
    	aioTpTblRepository.delete(aTbl);
    }

    @Override
    public AioTpTbl getDetail(String aioTpLabel) {
        return aioTpTblRepository.findByAioTpLabel(aioTpLabel);
    }

    @Override
    public List<AioTpTbl> getList(String devTpLabel) {
        return aioTpTblRepository.findByDevTpLabel(devTpLabel);
    }

    @Override
    public void copy(AioTpTbl aioTpTbl) {
    	
    }
}
