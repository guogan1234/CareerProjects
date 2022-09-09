define(['devSelect','ajax','form','modalHelp','popUp'],function (devSelect,ajax,form,modalHelp,popUp) {
    var operateModal=function () {
        $('#signalDioModal').on('show.bs.modal', function (event) {
            $.fn.modal.Constructor.prototype.enforceFocus = function () { };
            var b = $(event.relatedTarget);
            var modal = $(this);
            var  zy=$("#proId").val();
            var operate=b.data("operate");
            var types=[];
            types.push("DTL");
            types.push("AAD");
            types.push("DG");
            modalHelp.modalInit(zy,types,$("#signalDioModal"));
            if(operate=="add"){//新增点
                var devId= b.data('id');
                if (!addValidate()){
                    return false;
                }
                modal.find("[name='devLabel']").val(devId);
                $("#saveDioSignal").show();
                $("#updateDioSignal").addClass("hide");
                $("#signalDioModalLabel").html("新增信号")
            }else if(operate=="update"){//修改点
                var signalId=b.data('id');
                var type=b.parent().parent().find("[name='hidType']").val();
                getDetail(signalId,type);
                modal.find("[name='dioLabel']").prop("readOnly",true);
                $("#saveDioSignal").hide();
                $("#updateDioSignal").removeClass("hide");
                $("#signalDioModalLabel").html("修改信号")
            }else if (operate=="detail"){//详情
                var signalId=b.data('id');
                var type=b.parent().parent().find("[name='hidType']").val();
                getDetail(signalId,type);
                modalHelp.forbidModal($("#signalDioModal"));
                $("#signalDioModalLabel").html("查看信号")
                $("#saveDioSignal").hide();
                $("#updateDioSignal").removeClass("hide");
                $("#updateDioSignal").addClass("hide");
            }
        });
        $('#signalAioModal').on('show.bs.modal', function (event) {
            $.fn.modal.Constructor.prototype.enforceFocus = function () { };
            var b = $(event.relatedTarget);
            var modal = $(this);
            var operate=b.data("operate");
            var  zy=$("#proId").val();
            var types=[];
            types.push("DTL");
            types.push("AAD");
            modalHelp.modalInit(zy,types,$("#signalAioModal"));
            if(operate=="add"){//新增点
                var devId= b.data('id');
                if (!addValidate()){
                    return false;
                }
                modal.find("[name='devLabel']").val(devId);
                $("#saveAioSignal").show();
                $("#updateAioSignal").addClass("hide");
                $("#signalAioModalLabel").html("新增信号")
            }else if(operate=="update"){//修改点
                var signalId=b.data('id');
                var type=b.parent().parent().find("[name='hidType']").val();
                getDetail(signalId,type);
                modal.find("[name='aioLabel']").prop("readOnly",true);
                $("#saveAioSignal").hide();
                $("#updateAioSignal").removeClass("hide");
                $("#signalAioModalLabel").html("修改信号")
            }else if (operate=="detail"){//详情
                var signalId=b.data('id');
                var type=b.parent().parent().find("[name='hidType']").val();
                getDetail(signalId,type);
                modalHelp.forbidModal($("#signalAioModal"));
                $("#signalAioModalLabel").html("查看信号")
                $("#saveAioSignal").hide();
                $("#updateAioSignal").removeClass("hide");
                $("#updateAioSignal").addClass("hide");
            }
        });

        $('#signalAccModal').on('show.bs.modal', function (event) {
            $.fn.modal.Constructor.prototype.enforceFocus = function () { };
            var b = $(event.relatedTarget);
            var modal = $(this);
            var operate=b.data("operate");
            var  zy=$("#proId").val();
            var types=[];
            types.push("DTL");
            types.push("AAD");
            modalHelp.modalInit(zy,types,$("#signalAccModal"));
            if(operate=="add"){//新增点
                var devId= b.data('id');
                if (!addValidate()){
                    return false;
                }
                modal.find("[name='devLabel']").val(devId);
                $("#saveAccSignal").show();
                $("#updateAccSignal").addClass("hide");
                $("#signalAccModalLabel").html("新增信号")
            }else if(operate=="update"){//修改点
                var signalId=b.data('id');
                var type=b.parent().parent().find("[name='hidType']").val();
                getDetail(signalId,type);
               // modal.find("[name='accLabel']").prop("readOnly",true);
                $("#saveAccSignal").hide();
                $("#updateAccSignal").removeClass("hide");
                $("#signalAccModalLabel").html("修改信号")
            }else if (operate=="detail"){//详情
                var signalId=b.data('id');
                var type=b.parent().parent().find("[name='hidType']").val();
                getDetail(signalId,type);
                modalHelp.forbidModal($("#signalAccModal"));
                $("#signalAccModalLabel").html("查看信号")
                $("#saveAccSignal").hide();
                $("#updateAccSignal").removeClass("hide");
                $("#updateAccSignal").addClass("hide");
            }
        });

        $('#signalInheritDioModal').on('show.bs.modal', function (event) {
            $.fn.modal.Constructor.prototype.enforceFocus = function () { };
            var b = $(event.relatedTarget);
            var modal = $(this);
            var operate=b.data("operate");
            var types=[];
           // types.push("DTL");
           // types.push("AAD");
            modalHelp.modalInit("",types,$("#signalInheritDioModal"));
          if(operate=="update"){//修改点
                var signalId=b.data('id');
                var type=b.parent().parent().find("[name='hidType']").val();
                getDetail(signalId,type,true);
                modal.find("[name='dioLabel']").prop("readOnly",true);
                modal.find("[name='dioName']").prop("readOnly",true);
                modal.find("[name='dioTpLabel']").prop("readOnly",true);
                modal.find("[name='dioTpName']").prop("readOnly",true);
                $("#saveInheritDioSignal").hide();
                $("#updateInheritDioSignal").removeClass("hide");
                $("#signalInheritDioModalLabel").html("修改信号")
            }else if (operate=="detail"){//详情
                var signalId=b.data('id');
                var type=b.parent().parent().find("[name='hidType']").val();
                getDetail(signalId,type,true);
                modalHelp.forbidModal($("#signalInheritDioModal"));
                $("#signalInheritDioModalLabel").html("查看信号")
                $("#saveInheritDioSignal").hide();
                $("#updateInheritDioSignal").removeClass("hide");
                $("#updateInheritDioSignal").addClass("hide");
            }
        });

        $('#signalInheritAioModal').on('show.bs.modal', function (event) {
            $.fn.modal.Constructor.prototype.enforceFocus = function () { };
            var b = $(event.relatedTarget);
            var modal = $(this);
            var operate=b.data("operate");
            var types=[];
            // types.push("DTL");
            // types.push("AAD");
            modalHelp.modalInit("",types,$("#signalInheritAioModal"));
            if(operate=="update"){//修改点
                var signalId=b.data('id');
                var type=b.parent().parent().find("[name='hidType']").val();
                getDetail(signalId,type,true);
                modal.find("[name='aioLabel']").prop("readOnly",true);
                modal.find("[name='aioName']").prop("readOnly",true);
                modal.find("[name='aioTpLabel']").prop("readOnly",true);
                modal.find("[name='aioTpName']").prop("readOnly",true);
                $("#saveInheritAioSignal").hide();
                $("#updateInheritAioSignal").removeClass("hide");
                $("#signalInheritAioModalLabel").html("修改信号")
            }else if (operate=="detail"){//详情
                var signalId=b.data('id');
                var type=b.parent().parent().find("[name='hidType']").val();
                getDetail(signalId,type,true);
                modalHelp.forbidModal($("#signalInheritAioModal"));
                $("#signalInheritAioModalLabel").html("查看信号")
                $("#saveInheritAioSignal").hide();
                $("#updateInheritAioSignal").removeClass("hide");
                $("#updateInheritAioSignal").addClass("hide");
            }
        });

        $('#signalInheritAccModal').on('show.bs.modal', function (event) {
            $.fn.modal.Constructor.prototype.enforceFocus = function () { };
            var b = $(event.relatedTarget);
            var modal = $(this);
            var operate=b.data("operate");
            var types=[];
            // types.push("DTL");
            // types.push("AAD");
            modalHelp.modalInit("",types,$("#signalInheritAccModal"));
            if(operate=="update"){//修改点
                var signalId=b.data('id');
                var type=b.parent().parent().find("[name='hidType']").val();
                getDetail(signalId,type,true);
                modal.find("[name='accLabel']").prop("readOnly",true);
                modal.find("[name='accName']").prop("readOnly",true);
                modal.find("[name='accTpLabel']").prop("readOnly",true);
                modal.find("[name='accTpName']").prop("readOnly",true);
                $("#saveInheritAccSignal").hide();
                $("#updateInheritAccSignal").removeClass("hide");
                $("#signalInheritAccModalLabel").html("修改信号")
            }else if (operate=="detail"){//详情
                var signalId=b.data('id');
                var type=b.parent().parent().find("[name='hidType']").val();
                getDetail(signalId,type,true);
                modalHelp.forbidModal($("#signalInheritAccModal"));
                $("#signalInheritAccModalLabel").html("查看信号")
                $("#saveInheritAccSignal").hide();
                $("#updateInheritAccSignal").removeClass("hide");
                $("#updateInheritAccSignal").addClass("hide");
            }
        });
    };

    var addValidate=function () {
        //得到点模版类型
        var signalSelect= $("#signalSelect").val();
        if (stringUtils.isBlank(signalSelect)){
            popUp.showPopUp("devInstanceTbl","fail","新增信号模版","请选择一种信号模版类型");
            return false;
        }
        return true;
    };
    var signalInstanceHtml=function(signalId,signalName,type,devInstanceId,needHeader,ifSpecail){
        var h=[];
        if (needHeader) {
            h.push('<tr data-subid="signal_' + devInstanceId + '_header" style="background-color: #b7d8e457;">');
            h.push('<th></th><th></th><th>信号标签</th><th>信号名称</th><th>信号类型</th><th>普通操作</th></tr>');
        }
        h.push('<tr  data-subid="signal_'+devInstanceId+'" data-id="'+signalId+'" style="background-color: #b7d8e457;">');
        h.push('<td></td><td><input type="hidden" name="hidType" value="'+type+'"></td>');
        h.push('<td>'+signalId+'</td>');
        if (type ==1){
            if (ifSpecail){
                h.push('<td><a href="#" data-id="'+signalId+'" data-operate="detail" data-target="#signalDioModal" data-toggle="modal" name="signalName">'+signalName+'</a></td>');
                h.push('<td>离散量信号</td>');
            }else {
                h.push('<td><a href="#" data-id="'+signalId+'" data-operate="detail" data-target="#signalInheritDioModal" data-toggle="modal" name="signalName">'+signalName+'</a></td>');
                h.push('<td>继承离散量信号</td>');
            }
        }else if (type==2){
            if (ifSpecail){
                h.push('<td><a href="#" data-id="'+signalId+'" data-operate="detail" data-target="#signalAioModal" data-toggle="modal" name="signalName">'+signalName+'</a></td>');
                h.push('<td>模拟量信号</td>');
            }else {
                h.push('<td><a href="#" data-id="'+signalId+'" data-operate="detail" data-target="#signalInheritAioModal" data-toggle="modal" name="signalName">'+signalName+'</a></td>');
                h.push('<td>继承模拟量信号</td>');
            }
        }else if (type==3){
            if (ifSpecail){
                h.push('<td><a href="#" data-id="'+signalId+'" data-operate="detail" data-target="#signalAccModal" data-toggle="modal" name="signalName">'+signalName+'</a></td>');
                h.push('<td>累积量信号</td>');
            }else {
                h.push('<td><a href="#" data-id="'+signalId+'" data-operate="detail" data-target="#signalInheritAccModal" data-toggle="modal" name="signalName">'+signalName+'</a></td>');
                h.push('<td>继承累积量信号</td>');
            }
        }else if(type==4){
        }
        h.push('<td>');
        if (type ==1){
            if (ifSpecail) {
                h.push('<a href="#" data-id="' + signalId + '" data-toggle="modal" data-target="#signalDioModal" data-operate="update">修改</a>');
                h.push('<a href="#" data-id="'+signalId+'" data-devinstanceid="'+devInstanceId+'" name="signalDel"> 删除</a>');
            }else{
                h.push('<a href="#" data-id="' + signalId + '" data-toggle="modal" data-target="#signalInheritDioModal" data-operate="update">修改</a>');
            }
        }else if (type==2){
            if (ifSpecail) {
                h.push('<a href="#" data-id="' + signalId + '" data-toggle="modal" data-target="#signalAioModal" data-operate="update">修改</a>');
                h.push('<a href="#" data-id="'+signalId+'" data-devinstanceid="'+devInstanceId+'" name="signalDel"> 删除</a>');
            }else{
                h.push('<a href="#" data-id="' + signalId + '" data-toggle="modal" data-target="#signalInheritAioModal" data-operate="update">修改</a>');
            }
        }else if (type==3){
            if (ifSpecail) {
                h.push('<a href="#" data-id="' + signalId + '" data-toggle="modal" data-target="#signalAccModal" data-operate="update">修改</a>');
                h.push('<a href="#" data-id="'+signalId+'" data-devinstanceid="'+devInstanceId+'" name="signalDel"> 删除</a>');
            }else {
                h.push('<a href="#" data-id="' + signalId + '" data-toggle="modal" data-target="#signalInheritAccModal" data-operate="update">修改</a>');
            }
        }else if(type==4){
        }
        h.push('</td></tr>');
        return h.join("");
    };

    var searchSignal=function (id) {
        var  data={"devLabel":id};
        ajax.ajaxInit("/signalInstance/getSignaInsatnceList",data,true);
        ajax.post(searchCal,"","devInstanceTbl");
    };
    var searchCal=function (en) {
        if (en.length>0){
            var html=[];
            for (var i=0;i<en.length;i++){
                if (i ==0){
                    html.push(signalInstanceHtml(en[i].signaId,en[i].signaName,en[i].signaType,en[i].devInstanceId,true,en[i].ifSpecail));
                }else {
                    html.push(signalInstanceHtml(en[i].signaId,en[i].signaName,en[i].signaType,en[i].devInstanceId,false,en[i].ifSpecail));
                }
            }
            $("#devInstanceTbl tbody").find("tr[data-id='"+en[0].devInstanceId+"']").after(html.join(""));
        }
    };

    var saveDioSignal=function () {
        var data=form.formInit($("#signalDioFormModal"));
        ajax.ajaxInit("/signalInstance/saveDioSignalInstance",data,true);
        ajax.post(saveDioSignalCal,"","devInstanceTbl");
    };
    var saveDioSignalCal=function (d) {
        var html=signalInstanceHtml(d.dioLabel,d.dioName,1,d.devLabel,false,true);
        $("#devInstanceTbl").find("tr[data-subid='signal_"+d.devLabel+"_header']").after(html);
        $("#signalDioModal").modal('hide');
    };

    var saveAioSignal=function () {
        var data=form.formInit($("#signalAioFormModal"));
        ajax.ajaxInit("/signalInstance/saveAioSignalInstance",data,true);
        ajax.post(saveAioSignalCal,"","devInstanceTbl");
    };
    var saveAioSignalCal=function (d) {
        var html=signalInstanceHtml(d.aioLabel,d.aioName,2,d.devLabel,false,true);
        $("#devInstanceTbl").find("tr[data-subid='signal_"+d.devLabel+"_header']").after(html);
        $("#signalAioModal").modal('hide');
    };

    var saveAccSignal=function () {
        var data=form.formInit($("#signalAccFormModal"));
        ajax.ajaxInit("/signalInstance/saveAccSignalInstance",data,true);
        ajax.post(saveAccSignalCal,"","devInstanceTbl");
    };
    var saveAccSignalCal=function (d) {
        var html=signalInstanceHtml(d.accLabel,d.accName,3,d.devLabel,false,true);
        $("#devInstanceTbl").find("tr[data-subid='signal_"+d.devLabel+"_header']").after(html);
        $("#signalAccModal").modal('hide');
    };

    var updateDioSignal=function () {
        var data=form.formInit($("#signalDioFormModal"));
        ajax.ajaxInit("/signalInstance/updateDioSignalInstance",data,true);
        ajax.post(updateDioSignalCal,"","devInstanceTbl");
    };
    var updateDioSignalCal=function (d) {
        var selector=$("#devInstanceTbl tbody").find("tr[data-id='"+d.dioLabel+"']");
        selector.find("[name='signalName']").html(d.dioName);
        $("#signalDioModal").modal('hide');
    };

    var updateAioSignal=function () {
        var data=form.formInit($("#signalAioFormModal"));
        ajax.ajaxInit("/signalInstance/updateAioSignalInstance",data,true);
        ajax.post(updateAioSignalCal,"","devInstanceTbl");
    };
    var updateAioSignalCal=function (d) {
        var selector=$("#devInstanceTbl tbody").find("tr[data-id='"+d.aioLabel+"']");
        selector.find("[name='signalName']").html(d.aioName);
        $("#signalAioModal").modal('hide');
    };

    var updateAccSignal=function () {
        var data=form.formInit($("#signalAccFormModal"));
        ajax.ajaxInit("/signalInstance/updateAccSignalInstance",data,true);
        ajax.post(updateAccSignalCal,"","devInstanceTbl");
    };
    var updateAccSignalCal=function (d) {
        var selector=$("#devInstanceTbl tbody").find("tr[data-id='"+d.accLabel+"']");
        selector.find("[name='signalName']").html(d.accName);
        $("#signalAccModal").modal('hide');
    };


    var updateInheritDioSignal=function () {
        var data=form.formInit($("#signalInheritDioFormModal"));
        ajax.ajaxInit("/signalInstance/updateInheritDioSignalInstance",data,true);
        ajax.post(updateInheritDioSignalCal,"","devInstanceTbl");
    };
    var updateInheritDioSignalCal=function (d) {
        $("#signalInheritDioModal").modal('hide');
    };

    var updateInheritAioSignal=function () {
        var data=form.formInit($("#signalInheritAioFormModal"));
        ajax.ajaxInit("/signalInstance/updateInheritAioSignalInstance",data,true);
        ajax.post(updateInheritAioSignalCal,"","devInstanceTbl");
    };
    var updateInheritAioSignalCal=function (d) {
        $("#signalInheritAioModal").modal('hide');
    };

    var updateInheritAccSignal=function () {
        var data=form.formInit($("#signalInheritAccFormModal"));
        ajax.ajaxInit("/signalInstance/updateInheritAccSignalInstance",data,true);
        ajax.post(updateInheritAccSignalCal,"","devInstanceTbl");
    };
    var updateInheritAccSignalCal=function (d) {
        $("#signalInheritAccModal").modal('hide');
    };



    var getDetail=function (id,type,inherit) {
        var data={"signalLabel":id,"signalType":type};
        ajax.ajaxInit("/signalInstance/getDetail",data,false);
        var formModal="";
        if(type==1){
            formModal="signalDioFormModal";
        }else if (type ==2){
            formModal="signalAioFormModal";
        }else if (type==3){
            formModal="signalAccFormModal";
        }
        if(inherit){
            if(type==1){
                formModal="signalInheritDioFormModal";
            }else if (type ==2){
                formModal="signalInheritAioFormModal";
            }else if (type==3){
                formModal="signalInheritAccFormModal";
            }
        }
        ajax.post(detailCal,"",formModal,type,inherit);
    };
    var detailCal=function (d,type,inherit) {
        console.log(d);
        var formModal="";
        if(type==1){
            formModal=$("#signalDioFormModal");
        }else if (type ==2){
            formModal=$("#signalAioFormModal");
        }else if (type==3){
            formModal=$("#signalAccFormModal");
        }
        if (inherit){
            if(type==1){
                formModal=$("#signalInheritDioFormModal");
            }else if (type ==2){
                formModal=$("#signalInheritAioFormModal");
            }else if (type==3){
                formModal=$("#signalInheritAccFormModal");
            }
        }
        form.setForm(formModal,d);
    };

    var del=function (thi) {
        if (confirm("确定要删除吗？")){
            var id=thi.data('id');
            var data={"signalLabel":id};
            ajax.ajaxInit("/signalInstance/delSignalInstance",data,true);
            ajax.post(delCal,thi,"devInstanceTbl");
        }
    };
    var delCal=function (d) {
        $(d[0]).parent().parent().remove();
        var devInstanceId=$(d[0]).data('devinstanceid');
        if ($("#devInstanceId").find("tr[data-subId='signal_"+devInstanceId+"']").length==0){
            $("#devInstanceId").find("tr[data-subId='signal_"+devInstanceId+"_header']").remove();
            var t=$("#devInstanceId").find("tr[data-id='"+devInstanceId+"']").find(".glyphicon");
            t.attr("name","devplus");
            t.removeClass("glyphicon-minus").addClass("glyphicon-plus");
        }
    };
    return {
        "operateModal":operateModal,
        "saveDioSignal":saveDioSignal,
        "saveAioSignal":saveAioSignal,
        "saveAccSignal":saveAccSignal,
        "updateDioSignal":updateDioSignal,
        "updateAioSignal":updateAioSignal,
        "updateAccSignal":updateAccSignal,
        "updateInheritDioSignal":updateInheritDioSignal,
        "updateInheritAioSignal":updateInheritAioSignal,
        "updateInheritAccSignal":updateInheritAccSignal,
        "searchSignal":searchSignal,
        "getDetail":getDetail,
        "del":del
    }
});