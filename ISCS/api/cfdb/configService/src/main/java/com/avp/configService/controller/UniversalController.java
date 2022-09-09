package com.avp.configService.controller;

import com.avp.configService.entity.vo.UniversalVo;
import com.avp.configService.entity.vo.ZyEntity;
import com.avp.configService.service.UniversalService;
import com.avp.configService.util.*;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.data.domain.PageRequest;
import org.springframework.data.domain.Pageable;
import org.springframework.http.ResponseEntity;
import org.springframework.util.StringUtils;
import org.springframework.web.bind.annotation.RequestMapping;
import org.springframework.web.bind.annotation.RequestMethod;
import org.springframework.web.bind.annotation.RestController;

import java.util.ArrayList;
import java.util.Iterator;
import java.util.List;
import java.util.Map;

@RestController
@RequestMapping("/conf")
public class UniversalController {

    @Autowired
    private UniversalService universalService;

    @RequestMapping(value = "/execSql")
    public ResponseEntity<RestBody<Integer>> execSql(String sql) {
        // RestBody restBody=new RestBody(ResponseCode.OK);
        ResponseBuilder<Integer> builer = ResponseBuilder.createBuilder();

        if (StringUtils.isEmpty(sql)) {
//            restBody.setMessage("sql不能为空");
//            restBody.setCode(ResponseCode.FAILED.value());
            builer.setErrorCode(ResponseCode.BAD_REQUEST, "sql不能为空");
            return builer.getResponseEntity();
        }
        try {
            builer.setResultEntity(this.universalService.execSql(sql));
        } catch (Exception e) {
            builer.setErrorCode(ResponseCode.BAD_REQUEST, e.getStackTrace().toString());
        }
        return builer.getResponseEntity();
    }

    @RequestMapping(value = "/getList", method = RequestMethod.POST)
    public ResponseEntity<RestBody<List<Map<String, Object>>>> getList(String sql) {
        ResponseBuilder<List<Map<String, Object>>> builer = ResponseBuilder.createBuilder();
        if (StringUtils.isEmpty(sql)) {
            builer.setErrorCode(ResponseCode.BAD_REQUEST, "sql不能为空");
            return builer.getResponseEntity();
        }
        builer.setResultEntity(this.universalService.getList(sql));
        return builer.getResponseEntity();
    }

    @RequestMapping(value = "/getPageList")
    public ResponseEntity<RestBody<UniversalVo>> getPageList(String sql, Integer pageNumber, Integer pageSize) {

        ResponseBuilder<UniversalVo> builer = ResponseBuilder.createBuilder();
        if (StringUtils.isEmpty(sql)) {
            builer.setErrorCode(ResponseCode.BAD_REQUEST, "sql不能为空");
            return builer.getResponseEntity();
        }
        UniversalVo universalVo = new UniversalVo();
        org.springframework.data.domain.Page page = null;
        List<Map<String, Object>> list = new ArrayList<>();
        if (!StringUtils.isEmpty(pageNumber) && !StringUtils.isEmpty(pageSize)) {
            Pageable pageable = new PageRequest(pageNumber, pageSize);
            page = this.universalService.getPage(sql, pageable);
            list = page.getContent();
        } else {
            list = this.universalService.getList(sql);
        }

        if (list != null && list.size() > 0) {
            //得到列名
            Map<String, Object> map = list.get(0);
            List<String> columns = ReflectUtils.getColumns(map);
            universalVo.setColumnList(columns);
            universalVo.setColumnSize(columns.size());

            List<List<String>> dataList = new ArrayList<>();
            for (int i = 0; i < list.size(); i++) {
                Map<String, Object> map2 = list.get(i);
                Iterator it = map2.keySet().iterator();
                List<String> dlist = new ArrayList<>();
                while (it.hasNext()) {
                    Object key = it.next();
                    dlist.add(String.valueOf(map2.get(key)));
                }
                dataList.add(dlist);
            }
            universalVo.setDataList(dataList);
            universalVo.setDataCurrentSize(dataList.size());
        }
        if (!StringUtils.isEmpty(pageNumber) && !StringUtils.isEmpty(pageSize)) {
            Page pageUtil = new Page();
            pageUtil.setNumber(page.getNumber());
            pageUtil.setSize(page.getSize());
            pageUtil.setTotalPages(page.getTotalPages());

            pageUtil.setHasNext(page.hasNext() == true ? 1 : 0);
            pageUtil.setTotalElements(page.getTotalElements());
            pageUtil.setHasPrevious(page.hasPrevious() == true ? 1 : 0);
            universalVo.setPage(pageUtil);
        }
        builer.setResultEntity(universalVo);
        return builer.getResponseEntity();
    }


    @RequestMapping(value = "/getPageList2", method = RequestMethod.POST)
    public ResponseEntity<RestBody<ZyEntity>> getPageList2(String tableName, int domainId, int currentPage, int size) {
        Pageable pageable = new PageRequest(currentPage, size);
        ResponseBuilder<ZyEntity> builer = ResponseBuilder.createBuilder();
        builer.setResultEntity(this.universalService.getPage(tableName, domainId, currentPage, size));
        return builer.getResponseEntity();
    }
}
