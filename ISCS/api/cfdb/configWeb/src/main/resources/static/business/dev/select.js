/*
得到下拉框中的值
 */
define(['jquery','jsonStrUtils','stringUtils'],function($,jsonStrUtils,stringUtils){
    /**
     * 得到下拉框值返回的json
     */
    var selectAssignment=function (id,type,selector) {
        $.ajax({
            type:"post",
            url:"/dev/getSelectList",
            data:{"proSystemId":id,"selectType":type},
            async:false,
            success:function (data) {
                if (data.retFlag){//成功
                    //将json转数组
                    var en=jsonStrUtils.jsonStrToObj(data.obj);
                    if (type =="ZY"){
                        var zy = en.proSystemTblList;
                        proSystemSelect(zy,selector);
                    }else if (type=="DT"){
                        var dt=en.devTypeTblList;
                        devTypeSelect(dt,selector);
                    }else if (type=="DTL"){
                        var dtl=en.diTextTblList;
                        diTextSelect(dtl,selector);
                    }else if (type=="AL"){
                        var al=en.alarmLevelTblList;
                       alarmLevelSelect(al,selector);
                    }else if (type=="AT"){
                        var at=en.alarmTypeTblList;
                        alarmTypeSelect(at,selector);
                    }else if (type=="AC"){
                        var at=en.alarmActTblList;
                        alarmActSelect(at,selector);
                    }else if (type=="FM"){
                        var fm=en.formulaTpTblList;
                        formulaSelect(fm, selector);
                    }else if (type=="DG"){
                        var dg=en.doGrpTblList;
                        doGrpSelect(dg,selector);
                    }else if (type=="ST"){
                        var st=en.stationTblList;
                        stationSelect(st);
                    }else if (type =="DM"){
                        var dm=en.domainTblList;
                        domainSelect(dm,selector);
                    }else if (type =="RG"){
                        var rg=en.regionTblList;
                        regionSelect(rg,selector);
                    }else if (type =="DW"){
                        var dw=en.unitTblList;
                        unitSelect(dw, selector);
                    }else if (type =="PT"){
                        var pt=en.permTypeDefList;
                        permTypeSelect(pt,selector);
                    }else if (type=="DEP"){
                        var dep=en.permDepartmentDefList;
                        permDepartmentSelect(dep, selector);
                    }else if (type=="USER"){
                        var user=en.permUserDefList;
                        permUserSelect(user, selector);
                    }else if (type=="GROUP"){
                        var group=en.permGroupDefList;
                        permGroupSelect(group, selector);
                    }else if (type =="CF"){
                        var cf=en.permFuncDefList;
                        permFuncSelect(cf, selector);
                    }else if (type =="SF"){
                        var sf=en.permFuncDefList;
                        permFuncSelect(sf, selector);
                    }
                }
            }
        });
    };
    //普通权限和特殊权限
    var permFuncSelect=function (f,selector) {
        selector.val("").select2();
        if (f.length>0){
            var h=[];
            //h.push("<option value=''>请选择</option>")
            for (var  i =0;i<f.length;i++){
                h.push("<option value='"+f[i].permId+"'>"+f[i].permName+"</option>")
            }
            h.join("");
            selector.html(h);
        }
    };
   //部门
    var permDepartmentSelect=function (dep,selector) {
        selector.val("").select2();
        if (dep.length>0){
            var h=[];
            h.push("<option value=''>请选择部门</option>");
            for (var  i =0;i<dep.length;i++){
                h.push("<option value='"+dep[i].deptId+"'>"+dep[i].deptName+"</option>")
            }
            h.join("");
            selector.html(h);
        }
    };
    //用户
    var permUserSelect=function (user,selector) {
        selector.val("").select2();
        if (user.length>0){
            var h=[];
            //h.push("<option value=''>请选择部门</option>")
            for (var  i =0;i<user.length;i++){
                h.push("<option value='"+user[i].userId+"'>"+user[i].userName+"</option>")
            }
            h.join("");
            selector.html(h);
        }
    };
    //用户组
    var permGroupSelect=function (group,selector) {
        selector.val("").select2();
        if (group.length>0){
            var h=[];
            h.push("<option value=''>请选择</option>");
            for (var  i =0;i<group.length;i++){
                h.push("<option value='"+group[i].groupId+"'>"+group[i].groupName+"</option>")
            }
            h.join("");
            selector.html(h);
        }
    };


    //专业
    var proSystemSelect=function (zy,selector) {
        selector.val("").select2();
        if (zy.length>0){
            var h=[];
            h.push("<option value=''>请选择专业</option>")
            for (var  i =0;i<zy.length;i++){
               h.push("<option value='"+zy[i].proSystemId+"'>"+zy[i].proSystemName+"</option>")
            }
            h.join("");
            selector.html(h);
        }
    };
    //车站
    var stationSelect=function (st) {
        if (st.length>0){
            var h=[];
            for (var  i =0;i<st.length;i++){
                h.push("<option value='"+st[i].stationId+"'>"+st[i].stationName+"</option>")
            }
            h.join("");
            $("#stationSelect").append(h);
            $("#devInstanceFormModal").find("[name='stationId']").html(h);
            $("#stationCopyModal").find("[name='sourceStationId']").html(h);
            $("#stationCopyModal").find("[name='stationId']").html(h);
            $("#batchInstanceFormModal").find("[name='stationId']").html(h);
            $("#alarmInfoFormModal").find("[name='stationId']").html(h);
            $("#permGroupStationModal").find("[name='stationId']").html(h);
            $("#permSpecGroupStationRealationFormModal").find("[name='stationId']").html(h);
        }
    };

    //域名
    var domainSelect=function (dm,selector) {
        selector.val("").select2();
        if(dm.length>0){
            var h=[];
            h.push("<option value=''>请选择</option>")
            for (var i=0;i<dm.length;i++){
                h.push("<option value='"+dm[i].domainId+"'>"+dm[i].domainName+"</option>")
            }
            h.join("");
            selector.html(h);
        }
    };

    //责任区
    var regionSelect=function (rg,selector) {
        selector.val("").select2();
        if (rg.length>0){
            var h=[];
            // h.push("<option value=''>请选择</option>")
            for (var i=0;i<rg.length;i++){
                h.push("<option value='"+rg[i].regionId+"'>"+rg[i].regionName+"</option>")
            }
            h.join("");
            selector.html(h);
        }
    };

    //设备类型
    var devTypeSelect=function (dt,selector) {
        selector.val("").select2();
        if(dt.length>0){
            var h=[];
            h.push("<option value=''>请选择设备</option>")
            for (var i=0;i<dt.length;i++){
                h.push("<option value='"+dt[i].devTypeLabel+"'>"+dt[i].devTypeName+"</option>")
            }
            h.join("");
            selector.html(h);
        }
    };
    //数字量文本
    var diTextSelect=function (dt,selector) {
        if(dt.length>0){
            var h=[];
            h.push("<option value=''>请选择</option>")
            for (var i=0;i<dt.length;i++){
                h.push("<option value='"+dt[i].diTextLabel+"'>"+dt[i].diValue+"</option>")
            }
            h.join("");
            selector.html(h);
        }
    };
    //报警等级
    var alarmLevelSelect=function (dt,selector) {
        if(dt.length>0){
            var h=[];
            h.push("<option value=''>请选择</option>")
            for (var i=0;i<dt.length;i++){
                h.push("<option value='"+dt[i].levelId+"'>"+dt[i].levelName+"</option>")
            }
            h.join("");
            selector.html(h);
        }
    };
    //报警类型
    var alarmTypeSelect=function (dt,selector) {
        if(dt.length>0){
            var h=[];
            h.push("<option value=''>请选择</option>")
            for (var i=0;i<dt.length;i++){
                h.push("<option value='"+dt[i].typeId+"'>"+dt[i].typeName+"</option>")
            }
            h.join("");
            selector.html(h);
        }
    };
    //报警动作
    var alarmActSelect=function (dt,selector) {
        if(dt.length>0){
            var h=[];
            h.push("<option value=''>请选择</option>")
            for (var i=0;i<dt.length;i++){
                h.push("<option value='"+dt[i].actId+"'>"+dt[i].actName+"</option>")
            }
            h.join("");
            selector.html(h);
        }
    };
    //公式模板标签
    var formulaSelect=function (dt,selector) {
        if(dt.length>0){
            var h=[];
            h.push("<option value=''>请选择</option>")
            for (var i=0;i<dt.length;i++){
                h.push("<option value='"+dt[i].formulaTpLabel+"'>"+dt[i].formulaTpName+"</option>")
            }
            h.join("");
            selector.html(h);
        }
    };
    //单位
    var unitSelect=function (dw,selector) {
        if(dw.length>0){
            var h=[];
            h.push("<option value=''>请选择</option>")
            for (var i=0;i<dw.length;i++){
                h.push("<option value='"+dw[i].unitId+"'>"+dw[i].unitName+"</option>")
            }
            h.join("");
            selector.html(h);
        }
    };
    //权限类型
    var permTypeSelect=function (pt,selector) {
        if(pt.length>0){
            var h=[];
            h.push("<option value=''>请选择</option>")
            for (var i=0;i<pt.length;i++){
                h.push("<option value='"+pt[i].permTypeId+"'>"+pt[i].permTypeName+"</option>")
            }
            h.join("");
            selector.html(h);
        }
    };


    //控制动作组标签
    var doGrpSelect=function (dt,selector) {
        if(dt.length>0){
            var h=[];
            h.push("<option value=''>请选择</option>")
            for (var i=0;i<dt.length;i++){
                h.push("<option value='"+dt[i].doGrpLabel+"_"+dt[i].doOrder+"'>"+dt[i].doValue+"</option>")
            }
            h.join("");
            selector.html(h);
        }
    };
    return {
        "selectAssignment":selectAssignment
    }
});