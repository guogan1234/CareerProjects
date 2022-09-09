define(['devSelect','ajax','form','modalHelp'],function (devSelect,ajax,form,modalHelp) {
    var operateModal=function () {
        $("#devInstanceModal").on('show.bs.modal',function (event) {
            $.fn.modal.Constructor.prototype.enforceFocus = function () { };
            var b = $(event.relatedTarget)
            var modal = $(this);
            var  zy=$("#proId").val();
            var operate=b.data("operate");
            var types=[];
            types.push("DM");
            types.push("RG");
            modalHelp.modalInit(zy,types,$("#devInstanceModal"));
            if (operate=="add"){//新增操作
                var zy=$("#proId").val();
                modal.find("[name='proSystemId']").val(zy);
                modal.find("[name='devTpLabel']").val($("#devId").val());
                $("#devInstanceSaveModal").show();
                $("#devInstanceUpdateModal").addClass("hide");
                $("#devInstanceModalLabel").html("新增设备实例")
            }else if (operate=="update"){
                var id=b.data('id');
                getDetail(id);
                modal.find("[name='devLabel']").prop("readOnly",true);
                $("#devInstanceSaveModal").hide();
                $("#devInstanceUpdateModal").removeClass("hide");
                $("#devInstanceModalLabel").html("修改设备实例")
            }else if(operate=="detail"){
                var id=b.data("id");
                getDetail(id);
                modalHelp.forbidModal($("#devInstanceModal"));
                $("#devInstanceModalLabel").html("查看设备实例")
                $("#devInstanceSaveModal").hide();
                $("#devInstanceUpdateModal").removeClass("hide");
                $("#devInstanceUpdateModal").addClass("hide");
            }
        });

        $("#devCopyModal").on('show.bs.modal',function (event) {
            $.fn.modal.Constructor.prototype.enforceFocus = function () { };
            var b = $(event.relatedTarget)
            var modal = $(this);
            var operate=b.data("operate");
            var types=[];
            modalHelp.modalInit("",types,$("#devCopyModal"));
            if(operate=="copy"){
                var id=b.data("id");
                modal.find("[name='sourceInstanceLabel']").val(id);
            }
        });

        $("#stationCopyModal").on('show.bs.modal',function (event) {
            $.fn.modal.Constructor.prototype.enforceFocus = function () { };
            var b = $(event.relatedTarget)
            var modal = $(this);
            var operate=b.data("operate");
            var types=[];
            types.push("DM");
            types.push("ST");
            modalHelp.modalInit("",types,$("#stationCopyModal"));
            // if(operate=="stationCopy"){
            //     var id=b.data("id");
            //     modal.find("[name='sourceInstanceLabel']").val(id);
            // }
        });
    };

    var devInstanceHtml=function (label,name) {
        var h=[];
        h.push('<tr data-id="'+label+'">');
        h.push('<td><span class="glyphicon glyphicon-plus" data-id="'+label+'"  name="plusDevInstance" aria-hidden="true"></span></td>')
        h.push('<td>'+label+'</td>');
        h.push('<td><a href="#" name="devInstanceName" data-operate="detail" data-toggle="modal" data-target="#devInstanceModal" data-id="'+label+'">'+name+'</a></td>');
        h.push('<td colspan="3">');
        h.push('<a href="#" data-toggle="modal" data-operate="update" data-target="#devInstanceModal" data-id="'+label+'" name="modifyDevInstance"> 修改</a>');
        h.push('<a href="#" name="delDevInstance" data-id="'+label+'"> 删除</a>');
        h.push('<a href="#signalDigModal" data-operate="add" name="addSignal"  data-toggle="modal" data-id="'+label+'"> 新增信号实例</a>');
        h.push('<a href="#" data-operate="copy" data-target="#devCopyModal" data-toggle="modal" data-id="'+label+'"> 复制设备</a>');
        h.push('</td></tr>');
        return h.join("");
    };

    var saveInstance=function () {
        var data=form.formInit($("#devInstanceFormModal"));
        ajax.ajaxInit("/devInstance/addDev",data,true);
        ajax.post(saveCal,"","devInstanceTbl");
    };
    var saveCal=function (d) {
        var html=devInstanceHtml(d.devLabel,d.devName);
        if ($("#devInstanceTbl tbody").find("tr").length==0){
            $("#devInstanceTbl tbody").append(html);
        }else{
            $("#devInstanceTbl tbody").find("tr:first").before(html);
        }
        $("#devInstanceModal").modal('hide');
    };

    var copyStation=function () {
        var data=form.formInit($("#stationCopyFormModal"));
        ajax.ajaxInit("/devInstance/copyStation",data,true);
        ajax.post(copyStationCal,"","devInstanceTbl");
    };
    var copyStationCal=function (d) {
        $("#stationCopyModal").modal('hide');
    };

    var copy=function () {
        var data=form.formInit($("#devCopyFormModal"));
        ajax.ajaxInit("/devInstance/copyDevInstance",data,true);
        ajax.post(copyCal,"","devInstanceTbl");
    };
    var copyCal=function (d) {
        var html=devInstanceHtml(d.devLabel,d.devName);
        if ($("#devInstanceTbl tbody").find("tr").length==0){
            $("#devInstanceTbl tbody").append(html);
        }else{
            $("#devInstanceTbl tbody").find("tr:first").before(html);
        }
        $("#devCopyModal").modal('hide');
    };

    var updateInstance=function () {
        var data=form.formInit($("#devInstanceFormModal"));
        ajax.ajaxInit("/devInstance/updateDev",data,true);
        ajax.post(updateCal,"","devInstanceTbl");
    };
    var updateCal=function (d) {
        var selector=$("#devInstanceTbl tbody").find("tr[data-id='"+d.devLabel+"']");
        selector.find("[name='devInstanceName']").html(d.devName);
        $("#devInstanceModal").modal('hide');
    };

    var delInstance=function (thi) {
        if (confirm("确定要删除吗？")){
            var id=thi.data('id');
            var data={"devLabel":id};
            ajax.ajaxInit("/devInstance/delDev",data,true);
            ajax.post(delCal,thi,"devInstanceTbl");
        }
    };
    var delCal=function (d) {
        $(d[0]).parent().parent().remove();
        var id=$(d[0]).data('id');
        $("#devInstanceTbl tbody").find("tr[data-subid^='signal_"+id+"']").each(function () {
            $(this).remove();
        });
    };

    var getDetail=function (id) {
        var data={"devLabel":id};
        ajax.ajaxInit("/devInstance/getDevDetail",data,false);
        ajax.post(detailCal,"","devInstanceFormModal");
    };
    var detailCal=function (d) {
        form.setForm($("#devInstanceFormModal"),d);
    };

    var searchInstance=function (devId,stationId) {
        var  data={"devTpLabel":devId,"stationId":stationId};
        ajax.ajaxInit("/devInstance/getDevList",data,true);
        ajax.post(searchCal,"","devInstanceTbl");
    };
    var searchCal=function (en) {
        var html=[];
        for (var i=0;i<en.length;i++){
            html.push(devInstanceHtml(en[i].devLabel,en[i].devName));
        }
        $("#devInstanceTbl tbody").html(html.join(""));
    };
    return{
        "saveInstance":saveInstance,
        "operateModal":operateModal,
        "searchInstance":searchInstance,
        "getDetail":getDetail,
        "updateInstance":updateInstance,
        "delInstance":delInstance,
        "copy":copy,
        "copyStation":copyStation
    }
});