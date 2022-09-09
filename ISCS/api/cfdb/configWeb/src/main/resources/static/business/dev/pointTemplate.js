define(['devSelect','ajax','form','modalHelp','popUp','wizardHelp'],function (devSelect,ajax,form,modalHelp,popUp,wizardHelp) {
     var operateModal=function () {
         $('#pointDioModal').on('show.bs.modal', function (event) {
             $.fn.modal.Constructor.prototype.enforceFocus = function () { };
             var b = $(event.relatedTarget);
             var modal = $(this);
             var operate=b.data("operate");
             var types=[];
             types.push("DTL");
             types.push("AL");
             types.push("FM");
             types.push("DG");
             modalHelp.modalInit($("#proSystem").val(),types,$("#pointDioModal"),"pointDioWizard");
             if(operate=="add"){//新增点
                 var devId= b.data('id');
                 if (!addValidate()){
                     return false;
                 }
                 modal.find("[name='devTpLabel']").val(devId);
                 $("#savePointDioModal").show();
                 $("#updatePointDioModal").addClass("hide");
                 $("#pointDioModalLabel").html("新增点模版")
                 wizardHelp.wizardHide("pointDioWizard",1);
             }else if(operate=="update"){//修改点
                 var pointId=b.data('id');
                 var type=b.parent().parent().find("[name='hidType']").val();
                 getDetail(pointId,type);
                 modal.find("[name='dioTpLabel']").prop("readOnly",true);
                 $("#savePointDioModal").hide();
                 $("#updatePointDioModal").removeClass("hide");
                 $("#pointDioModalLabel").html("修改点模版")
                 var isControl=modal.find("[name='isControl']").val();
                 if(isControl=="true"){
                     modalHelp.clearModal($("#pointDio2"));
                     wizardHelp.wizardDisplay("pointDioWizard",1);
                 }else{
                     wizardHelp.wizardHide("pointDioWizard",1);
                 }
             }else if (operate=="detail"){//详情
                 var pointId=b.data('id');
                 var type=b.parent().parent().find("[name='hidType']").val();
                 getDetail(pointId,type);
                 modalHelp.forbidModal($("#pointDioModal"));
                 $("#pointDioModalLabel").html("查看点模版")
                 $("#savePointDioModal").hide();
                 $("#updatePointDioModal").removeClass("hide");
                 $("#updatePointDioModal").addClass("hide");
             }
         });
         $('#pointAioModal').on('show.bs.modal', function (event) {
             $.fn.modal.Constructor.prototype.enforceFocus = function () { };
             var b = $(event.relatedTarget);
             var modal = $(this);
             var operate=b.data("operate");
             var types=[];
             types.push("AAD");
             types.push("FM");
             types.push("AL");
             types.push("DW");
             modalHelp.modalInit($("#proSystem").val(),types,$("#pointAioModal"),"pointAioWizard");
             if(operate=="add"){//新增点
                 var devId= b.data('id');
                 if (!addValidate()){
                     return false;
                 }
                 modal.find("[name='devTpLabel']").val(devId);
                 $("#savePointAioModal").show();
                 $("#updatePointAioModal").addClass("hide");
                 $("#pointAioModalLabel").html("新增点模版")
                 wizardHelp.wizardHide("pointAioWizard",2);
                 wizardHelp.wizardHide("pointAioWizard",1);
             }else if(operate=="update"){//修改点
                 var pointId=b.data('id');
                 var type=b.parent().parent().find("[name='hidType']").val();
                 getDetail(pointId,type);
                 modal.find("[name='aioTpLabel']").prop("readOnly",true);
                 $("#savePointAioModal").hide();
                 $("#updatePointAioModal").removeClass("hide");
                 $("#pointAioModalLabel").html("修改点模版");
                 var isControl=modal.find("[name='isControl']").val();
                 if(isControl=="true"){
                     wizardHelp.wizardDisplay("pointAioWizard",2);
                 }else{
                     wizardHelp.wizardHide("pointAioWizard",2);
                 }

                 var isLimit=modal.find("[name='isLimit']").val();
                 if(isLimit=="true" || isLimit==true){
                     wizardHelp.wizardDisplay("pointAioWizard",1);
                     //得到限值组数
                     var limitNum=modal.find("[name='limitNum']").val();
                     for(var i=Number(limitNum)+1;i<=3;i++){
                         modal.find("input[name='limitUp"+i+"']").each(function(){
                             $(this).hide();
                         });
                         modal.find("input[name='limitLow"+i+"']").each(function(){
                             $(this).hide();
                         });
                     }
                 }else{
                     wizardHelp.wizardHide("pointAioWizard",1);
                 }
             }else if (operate=="detail"){//详情
                 var pointId=b.data('id');
                 var type=b.parent().parent().find("[name='hidType']").val();
                 getDetail(pointId,type);
                 modalHelp.forbidModal($("#pointAioModal"));
                 $("#pointAioModalLabel").html("查看点模版")
                 $("#savePointAioModal").hide();
                 $("#updatePointAioModal").removeClass("hide");
                 $("#updatePointAioModal").addClass("hide");
             }
         });

         $('#pointAccModal').on('show.bs.modal', function (event) {
             $.fn.modal.Constructor.prototype.enforceFocus = function () { };
             var b = $(event.relatedTarget);
             var modal = $(this);
             var operate=b.data("operate");
             var types=[];
             types.push("AAD");
             types.push("DW");
             types.push("AL");
             modalHelp.modalInit($("#proSystem").val(),types,$("#pointAccModal"),"pointAccWizard");
             if(operate=="add"){//新增点
                 var devId= b.data('id');
                 if (!addValidate()){
                     return false;
                 }
                 modal.find("[name='devTpLabel']").val(devId);
                 $("#savePointAccModal").show();
                 $("#updatePointAccModal").addClass("hide");
                 $("#pointAccModalLabel").html("新增点模版");
                 wizardHelp.wizardHide("pointAccWizard",1);
             }else if(operate=="update"){//修改点
                 var pointId=b.data('id');
                 var type=b.parent().parent().find("[name='hidType']").val();
                 getDetail(pointId,type);
                 modal.find("[name='accTpLabel']").prop("readOnly",true);
                 $("#savePointAccModal").hide();
                 $("#updatePointAccModal").removeClass("hide");
                 $("#pointAccModalLabel").html("修改点模版");
                 var isLimit=modal.find("[name='isLimit']").val();
                 if(isLimit=="true" || isLimit==true){
                     wizardHelp.wizardDisplay("pointAccWizard",1);
                     //得到限值组数
                     var limitNum=modal.find("[name='limitNum']").val();
                     if(stringUtils.isNotBlank(limitNum)){
                         for(var i=Number(limitNum)+1;i<=3;i++){
                             modal.find("input[name='limitUp"+i+"']").each(function(){
                                 $(this).hide();
                             });
                             modal.find("input[name='limitLow"+i+"']").each(function(){
                                 $(this).hide();
                             });
                         }
                     }
                 }else{
                     wizardHelp.wizardHide("pointAccWizard",1);
                 }
             }else if (operate=="detail"){//详情
                 var pointId=b.data('id');
                 var type=b.parent().parent().find("[name='hidType']").val();
                 getDetail(pointId,type);
                 modalHelp.forbidModal($("#pointAccModal"));
                 $("#pointAccModalLabel").html("查看点模版")
                 $("#savePointAccModal").hide();
                 $("#updatePointAccModal").removeClass("hide");
                 $("#updatePointAccModal").addClass("hide");
             }
         });
         $('#pointCopyModal').on('show.bs.modal', function (event) {
             $.fn.modal.Constructor.prototype.enforceFocus = function () { };
             var b = $(event.relatedTarget);
             var modal = $(this);
             var operate=b.data("operate");
             var types=[];
             modalHelp.modalInit("",types,$("#pointCopyModal"));
             if (operate=="copy"){//详情
                 var pointId=b.data('id');
                 var pointType=b.data("type");
                 modal.find("[name='pointType']").val(pointType);
                 modal.find("[name='sourcePointTpLabel']").val(pointId);
             }
         });
     };
    var addValidate=function () {
        //得到点模版类型
        var pointSelect= $("#pointSelect").val();
        if (stringUtils.isBlank(pointSelect)){
            popUp.showPopUp("devTbl","fail","新增点模版","请选择一种点模版类型");
            return false;
        }
        return true;
    };
     var pointDigTemplateHtml=function (id,name,devId,type,needHeader) {
         var h=[];
         if (needHeader) {
             h.push('<tr data-subid="point_' + devId + '_header" style="background-color: #b7d8e457;">');
             h.push('<th></th><th></th><th>点模版标签</th><th>点模版名称</th><th>点类型</th><th>普通操作</th> </tr>');
         }
         h.push('<tr  data-subid="point_'+devId+'" data-id="'+id+'" style="background-color: #b7d8e457;">');
         h.push('<td></td><td><input type="hidden" name="hidType" value="'+type+'" /></td>');
         h.push('<td>'+id+'</td>');
         if (type=="DIO") {
             h.push('<td><a href="#" data-id="' + id + '" data-operate="detail" data-target="#pointDioModal" data-toggle="modal" name="pointName">' + name + '</a></td>');
             h.push('<td>离散量</td>');
         }else if (type=="AIO") {
             h.push('<td><a href="#" data-id="' + id + '" data-operate="detail" data-target="#pointAioModal" data-toggle="modal" name="pointName">' + name + '</a></td>');
             h.push('<td>模拟量</td>');
         }else if(type=="ACC"){
             h.push('<td><a href="#" data-id="' + id + '" data-operate="detail" data-target="#pointAccModal" data-toggle="modal" name="pointName">' + name + '</a></td>');
             h.push('<td>累积量</td>');
         }
         h.push('<td>');
         if (type=="DIO"){
             h.push('<a href="#" data-id="'+id+'" data-toggle="modal" data-target="#pointDioModal" data-operate="update">修改</a>');
         }else if (type=="AIO"){
             h.push('<a href="#" data-id="'+id+'" data-toggle="modal" data-target="#pointAioModal" data-operate="update">修改</a>');
         }else if(type=="ACC"){
             h.push('<a href="#" data-id="'+id+'" data-toggle="modal" data-target="#pointAccModal" data-operate="update">修改</a>');
         }
         h.push('<a href="#" data-id="'+id+'" data-devid="'+devId+'" name="pointDel"> 删除</a><a href="#" name="pointCopy" data-id="'+id+'" data-type="'+type+'" data-toggle="modal" data-target="#pointCopyModal" data-operate="copy"> 复制</a></td></tr>');
         return h.join("");
     };

    var searchPoint=function (id) {
        var  data={"devId":id};
        ajax.ajaxInit("/point/pointList",data,true);
        ajax.post(searchCal,"","devTbl");
    };
    var searchCal=function (en) {
        if (en.length>0){
            var html=[];
            for (var i=0;i<en.length;i++){
                if (i ==0){
                    html.push(pointDigTemplateHtml(en[i].pointId,en[i].pointName,en[i].devTpLabel,en[i].type,true));
                }else {
                    html.push(pointDigTemplateHtml(en[i].pointId, en[i].pointName, en[i].devTpLabel,en[i].type,false));
                }
            }
            $("#devTbl tbody").find("tr[data-id='"+en[0].devTpLabel+"']").after(html.join(""));
        }
    };

    var saveDio=function (type) {
        var data=form.formInit($("#pointDioFormModal"));
        ajax.ajaxInit("/point/saveDioPoint",data,true);
        ajax.post(saveDioCal,"","devTbl");
    };
    var saveDioCal=function (d) {
        var html=pointDigTemplateHtml(d.dioTpLabel,d.dioTpName,d.devTpLabel,"DIO",false);
        $("#devTbl").find("tr[data-subid='point_"+d.devTpLabel+"_header']").after(html);
        $("#pointDioModal").modal('hide');
    };

    var saveAio=function (type) {
        var data=form.formInit($("#pointAioFormModal"));
        ajax.ajaxInit("/point/saveAioPoint",data,true);
        ajax.post(saveAioCal,"","devTbl");
    };
    var saveAioCal=function (d) {
        var html=pointDigTemplateHtml(d.aioTpLabel,d.aioTpName,d.devTpLabel,"AIO",false);
        $("#devTbl").find("tr[data-subid='point_"+d.devTpLabel+"_header']").after(html);
        $("#pointAioModal").modal('hide');
    };

    var saveAcc=function (type) {
        var data=form.formInit($("#pointAccFormModal"));
        ajax.ajaxInit("/point/saveAccPoint",data,true);
        ajax.post(saveAccCal,"","devTbl");
    };
    var saveAccCal=function (d) {
        var html=pointDigTemplateHtml(d.accTpLabel,d.accTpName,d.devTpLabel,"ACC",false);
        $("#devTbl").find("tr[data-subid='point_"+d.devTpLabel+"_header']").after(html);
        $("#pointAccModal").modal('hide');
    };

    var updateDio=function () {
        var data=form.formInit($("#pointDioFormModal"));
        ajax.ajaxInit("/point/updateDioPoint",data,true);
        ajax.post(updateDioCal,"","devTbl");
    };
    var updateDioCal=function (d) {
        var selector=$("#devTbl tbody").find("tr[data-id='"+d.dioTpLabel+"']");
        selector.find("[name='pointName']").html(d.dioTpName);
        $("#pointDioModal").modal('hide');
    };

    var updateAio=function () {
        var data=form.formInit($("#pointAioFormModal"));
        ajax.ajaxInit("/point/updateAioPoint",data,true);
        ajax.post(updateAioCal,"","devTbl");
    };
    var updateAioCal=function (d) {
        var selector=$("#devTbl tbody").find("tr[data-id='"+d.aioTpLabel+"']");
        selector.find("[name='pointName']").html(d.aioTpName);
        $("#pointAioModal").modal('hide');
    };

    var updateAcc=function () {
        var data=form.formInit($("#pointAccFormModal"));
        ajax.ajaxInit("/point/updateAccPoint",data,true);
        ajax.post(updateAccCal,"","devTbl");
    };
    var updateAccCal=function (d) {
        var selector=$("#devTbl tbody").find("tr[data-id='"+d.accTpLabel+"']");
        selector.find("[name='pointName']").html(d.accTpName);
        $("#pointAccModal").modal('hide');
    };

    var getDetail=function (id,type) {
        var data={"id":id,"type":type};
        ajax.ajaxInit("/point/getDetail",data,false);
        var formModal="";
        if(type=="DIO"){
            formModal="pointDioFormModal";
        }else if (type =="AIO"){
            formModal="pointAioFormModal";
        }else if (type=="ACC"){
            formModal="pointAccFormModal";
        }
        ajax.post(detailCal,"",formModal,type);
    };
    var detailCal=function (d,type) {
        var formModal="";
        if(type=="DIO"){
            formModal=$("#pointDioFormModal");
        }else if (type =="AIO"){
            formModal=$("#pointAioFormModal");
        }else if (type=="ACC"){
            formModal=$("#pointAccFormModal");
        }
        form.setForm(formModal,d);
    };
    
    var copy=function () {
        var data=form.formInit($("#pointCopyFormModal"));
        ajax.ajaxInit("/point/copy",data,true);
        ajax.post(copyCal,"","devTbl");
    };
    var copyCal=function (d) {
        var html=pointDigTemplateHtml(d.pointTpLabel,d.pointTpName,d.devTpLabel,d.pointType,false);
        $("#devTbl").find("tr[data-subid='point_"+d.devTpLabel+"_header']").after(html);
        $("#pointCopyModal").modal('hide');
    };

    var del=function (thi) {
        if (confirm("确定要删除吗？")){
            var id=thi.data('id');
            var data={"id":id};
            ajax.ajaxInit("/point/delPointTemplate",data,true);
            ajax.post(delCal,thi,"devTbl");
        }
    };
    var delCal=function (d) {
        $(d[0]).parent().parent().remove();
        var devId=$(d[0]).data('devid');
        if ($("#devTbl").find("tr[data-subId='point_"+devId+"']").length==0){
            $("#devTbl").find("tr[data-subId='point_"+devId+"_header']").remove();
            var t=$("#devTbl").find("tr[data-id='"+devId+"']").find(".glyphicon");
            t.attr("name","devplus");
            t.removeClass("glyphicon-minus").addClass("glyphicon-plus");
        }
    };
    
    var isControl=function (modelSelector,stepSelector,wizard,index) {
        modelSelector.on('change',function(event) {
            var ev = event || window.event;
            var target = ev.target || ev.srcElement;
            if (target.nodeName.toLowerCase() == "select") {
                var attrName=$(target).attr("name");
                if (attrName=="isControl"){
                    var c=$(target).val();
                    if(c=="true"){
                        modalHelp.clearModal(stepSelector);
                        wizardHelp.wizardDisplay(wizard,index);
                    }else{
                        wizardHelp.wizardHide(wizard,index);
                    }
                }
            }
        });
    };

    var isLimit=function (modelSelector,stepSelector,wizard,index) {
        modelSelector.on('change',function(event) {
            var ev = event || window.event;
            var target = ev.target || ev.srcElement;
            if (target.nodeName.toLowerCase() == "select") {
                var attrName=$(target).attr("name");
                if (attrName=="isLimit"){
                    var c=$(target).val();
                    if(c==true || c=="true"){
                        modalHelp.clearModal(stepSelector);
                        wizardHelp.wizardDisplay(wizard,index);
                    }else{
                        wizardHelp.wizardHide(wizard,index);
                    }
                }
            }
        });
    };

    var limitNum=function (modelSelector) {
        modelSelector.on('change',function(event) {
            var ev = event || window.event;
            var target = ev.target || ev.srcElement;
            if (target.nodeName.toLowerCase() == "input") {
                var attrName=$(target).attr("name");
                if (attrName=="limitNum"){
                    var v=$(target).val();
                    if(v!=null && v!="" && Number(v)>=1 && Number(v)<=3){
                            for(var i=1;i<=3;i++){
                                modelSelector.find("input[name='limitUp"+i+"']").each(function(){
                                    $(this).show();
                                });
                                modelSelector.find("input[name='limitLow"+i+"']").each(function(){
                                    $(this).show();
                                });
                            }
                            for(var i=Number(v)+1;i<=3;i++){
                                modelSelector.find("input[name='limitUp"+i+"']").each(function(){
                                    $(this).hide();
                                });
                                modelSelector.find("input[name='limitLow"+i+"']").each(function(){
                                    $(this).hide();
                                });
                            }
                    }
                }
            }
        });
    };

    var pointValidate =function (modelSelector,selector,checkLimitNum) {
        if (checkLimitNum){
            popUp.close();
            var up1=modelSelector.find("input[name='limitUp1']").eq(0).val();
            var up2=modelSelector.find("input[name='limitUp2']").eq(0).val();
            var up3=modelSelector.find("input[name='limitUp3']").eq(0).val();
            var low1=modelSelector.find("input[name='limitLow1']").eq(0).val();
            var low2=modelSelector.find("input[name='limitLow2']").eq(0).val();
            var low3=modelSelector.find("input[name='limitLow3']").eq(0).val();

            if(stringUtils.isNotBlank(up1) && stringUtils.isNotBlank(up2)){
                if (Number(up1)>Number(up2)){
                    popUp.showPopUp(selector,"fail","","上限值1不能大于上限值2");
                    return false;
                }
            }
            if(stringUtils.isNotBlank(up2) && stringUtils.isNotBlank(up3)){
                if (Number(up2)>Number(up3)){
                    popUp.showPopUp(selector,"fail","","上限值2不能大于上限值3");
                    return false;
                }
            }
            if(stringUtils.isNotBlank(up1) && stringUtils.isNotBlank(up3)){
                if (Number(up1)>Number(up3)){
                    popUp.showPopUp(selector,"fail","","上限值1不能大于上限值3");
                    return false;
                }
            }
            if(stringUtils.isNotBlank(low1) && stringUtils.isNotBlank(low2)){
                if (Number(low1)>Number(low2)){
                    popUp.showPopUp(selector,"fail","","下限值2不能小于下限值1");
                    return false;
                }
            }

            if(stringUtils.isNotBlank(low2) && stringUtils.isNotBlank(low3)){
                if (Number(low2)>Number(low3)){
                    popUp.showPopUp(selector,"fail","","下限值3不能小于下限值2");
                    return false;
                }
            }
            if(stringUtils.isNotBlank(low1) && stringUtils.isNotBlank(low3)){
                if (Number(low1)>Number(low3)){
                    popUp.showPopUp(selector,"fail","","下限值3不能小于下限值1");
                    return false;
                }
            }

            if(stringUtils.isNotBlank(up1) && stringUtils.isNotBlank(low1)){
                if (Number(low1)>Number(up1)){
                    popUp.showPopUp(selector,"fail","","下限值1不能大于上限值1");
                    return false;
                }
            }
            if(stringUtils.isNotBlank(up2) && stringUtils.isNotBlank(low2)){
                if (Number(low2)>Number(up2)){
                    popUp.showPopUp(selector,"fail","","下限值2不能大于上限值2");
                    return false;
                }
            }
            if(stringUtils.isNotBlank(up3) && stringUtils.isNotBlank(low3)){
                if (Number(low3)>Number(up3)){
                    popUp.showPopUp(selector,"fail","","下限值3不能大于上限值3");
                    return false;
                }
            }
        }
        return true;
    };
     return {
         "operateModal":operateModal,
         "saveDio":saveDio,
         "saveAio":saveAio,
         "saveAcc":saveAcc,
         "searchPoint":searchPoint,
         "del":del,
         "updateDio":updateDio,
         "updateAio":updateAio,
         "updateAcc":updateAcc,
         "copy":copy,
         "isControl":isControl,
         "isLimit":isLimit,
         "limitNum":limitNum,
         "pointValidate":pointValidate
     }
});