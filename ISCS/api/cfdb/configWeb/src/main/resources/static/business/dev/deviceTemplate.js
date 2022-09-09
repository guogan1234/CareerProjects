/*
  设备模版js
 */
define(['devSelect','ajax','form','modalHelp','popUp'],function (devSelect,ajax,form,modalHelp,popUp) {
    var proSystemChange=function () {
        var proSystemId=$("#devModal").find("[name='proSystemId']").val();
        if (stringUtils.isNotBlank(proSystemId)){
            devSelect.selectAssignment(proSystemId,"DT",$("#devModal").find("[name='devTypeLabel']"));
        }
    };

    var operateModal=function () {
        $("#devModal").on('show.bs.modal',function (event) {
            $.fn.modal.Constructor.prototype.enforceFocus = function () { };
            var b = $(event.relatedTarget)
            var operate=b.data("operate");
            var modal = $(this);
            var types=[];
            types.push("ZY");
            types.push("DT");
            modalHelp.modalInit($("#proSystem").val(),types,$("#devModal"));
            if(operate=="add"){
                $("#devTemplateSaveModal").show();
                $("#devTemplateUpdateModal").addClass("hide");
                $("#devModalLabel").html("新增设备模版")
            }else if(operate=="update"){
               var id=b.data("id");
               //查询详情
                getDeviceDetail(id);
               modal.find("[name='devTpLabel']").prop("readOnly",true);
               $("#devTemplateSaveModal").hide();
               $("#devTemplateUpdateModal").removeClass("hide");
               $("#devModalLabel").html("修改设备模版")
            }else if(operate=="detail"){
                var id=b.data("id");
                getDeviceDetail(id);
                modalHelp.forbidModal($("#devModal"));
                $("#devModalLabel").html("查看设备模版")
                $("#devTemplateSaveModal").hide();
                $("#devTemplateUpdateModal").removeClass("hide");
                $("#devTemplateUpdateModal").addClass("hide");
            }
        });

        $("#devTemplateCopyModal").on('show.bs.modal',function (event) {
            $.fn.modal.Constructor.prototype.enforceFocus = function () { };
            var b = $(event.relatedTarget)
            var operate=b.data("operate");
            var modal = $(this);
            var types=[];
            modalHelp.modalInit("",types,$("#devTemplateCopyModal"));
            if(operate=="copy"){
                var id=b.data("id");
                modal.find("[name='sourceDevTpLabel']").val(id);
            }
        });

        $("#batchInstanceModal").on('show.bs.modal',function (event) {
            $.fn.modal.Constructor.prototype.enforceFocus = function () { };
            var b = $(event.relatedTarget)
            var modal = $(this);
            var types=[];
            types.push("DM");
            types.push("ST");
            types.push("RG");
            var zy=b.data("zy");
            modalHelp.modalInit(zy,types,$("#batchInstanceModal"));
            var id=b.data("id");
            modal.find("[name='devTpLabel']").val(id);

            modal.find("[name='proSystemId']").val(zy);
        });
    };
    var deviceTemplateHtml=function (label,name,proSystem) {
        var h=[];
        h.push('<tr data-id="'+label+'">');
        h.push('<td><span class="glyphicon glyphicon-plus" name="devplus" data-id="'+label+'" aria-hidden="true"></span></td>');
        h.push('<td>'+label+'</td>');
        h.push('<td><a href="#" data-toggle="modal" data-target="#devModal" data-operate="detail" data-id="'+label+'"  name="devTpName">'+name+'</a></td>');
        h.push(' <td colspan="3">');
        h.push('<a href="#" data-toggle="modal" data-operate="update" data-id="'+label+'" data-zy="'+proSystem+'" data-target="#devModal">修改 </a>');
        h.push('<a href="#" name="delDev" data-id="'+label+'">删除 </a>');
        h.push('<a href="#pointDioModal" data-toggle="modal" data-id="'+label+'" data-zy="'+proSystem+'" name="addPoint" data-operate="add" >新增点 </a>');
        h.push('<a href="#" data-toggle="modal"  data-target="#devTemplateCopyModal" data-id="'+label+'" data-operate="copy">复制 </a>');
        h.push('<a href="#" data-toggle="modal"  data-target="#batchInstanceModal" data-id="'+label+'" data-zy="'+proSystem+'">批量实例 </a>');
        h.push('<a href="#" name="searchInstance" data-id="'+label+'"  data-zy="'+proSystem+'">查询实例 </a>');
        h.push(' </td></tr>');
        return h.join("");
    };

    var searchDevice=function () {
      var proSystem=$("#proSystem").val();
      if (stringUtils.isBlank(proSystem)){
          popUp.showPopUp("devTbl","fail","查询设备模版","请选择一个专业");
          return;
      }
      var deviceName=$("#deviceName").val();
      var  data={"proSystemId":proSystem,"deviceName":deviceName};
      ajax.ajaxInit("/dev/devList",data,true);
      ajax.post(searchCal,"","devTbl");
    };
    var searchCal=function (en) {
        var html=[];
        for (var i=0;i<en.length;i++){
            html.push(deviceTemplateHtml(en[i].devTpLabel,en[i].devTpName,en[i].proSystemId));
        }
        $("#devTbl tbody").html(html.join(""));
    };

    var save=function () {
        var data=form.formInit($("#devTemplateFormModal"));
        ajax.ajaxInit("/dev/saveDevTemplate",data,true);
        ajax.post(saveCal,"","devTbl");
    };
    var saveCal=function (d) {
        var html=deviceTemplateHtml(d.devTpLabel,d.devTpName,d.proSystemId);
        if ($("#devTbl tbody").find("tr").length==0){
            $("#devTbl tbody").append(html);
        }else{
            $("#devTbl tbody").find("tr:first").before(html);
        }
        $("#devModal").modal('hide');
    };

    var batchInstance=function () {
        var data=form.formInit($("#batchInstanceFormModal"));
        ajax.ajaxInit("/dev/batchInstance",data,true);
        ajax.post(batchInstanceCal,"","devTbl");
        $("#batchInstanceModal").modal('hide');
        $("#loadingModal").modal('show');
    };
    var batchInstanceCal=function (d) {
        $("#loadingModal").modal('hide');
    };

    var copy=function () {
        var data=form.formInit($("#devTemplateCopyFormModal"));
        ajax.ajaxInit("/dev/copy",data,true);
        ajax.post(copyCal,"","devTbl");
    };
    var copyCal=function (d) {
        var html=deviceTemplateHtml(d.devTpLabel,d.devTpName,d.proSystemId);
        if ($("#devTbl tbody").find("tr").length==0){
            $("#devTbl tbody").append(html);
        }else{
            $("#devTbl tbody").find("tr:first").before(html);
        }
        $("#devTemplateCopyModal").modal('hide');
    };
    var update=function () {
        var data=form.formInit($("#devTemplateFormModal"));
        ajax.ajaxInit("/dev/updateDevTemplate",data,true);
        ajax.post(updateCal,"","devTbl");
    };
    var updateCal=function (d) {
        var selector=$("#devTbl tbody").find("tr[data-id='"+d.devTpLabel+"']");
        selector.find("[name='devTpName']").html(d.devTpName);
        $("#devModal").modal('hide');
    };
    var getDeviceDetail=function (id) {
        var data={"devTpLabel":id};
        ajax.ajaxInit("/dev/getDetail",data,false);
        ajax.post(detailCal,"","devTemplateFormModal");
    };
    var detailCal=function (d) {
        form.setForm($("#devTemplateFormModal"),d);
    };
    var del=function (thi) {
        if (confirm("确定要删除吗？")){
            var id=thi.data('id');
            var data={"devTpLabel":id};
            ajax.ajaxInit("/dev/delDevTemplate",data,true);
            ajax.post(delCal,thi,"devTbl");
        }
    };
    var delCal=function (d) {
        $(d[0]).parent().parent().remove();
        //删除点模板
       var id=$(d[0]).data('id');
        $("#devTbl tbody").find("tr[data-subid^='point_"+id+"']").each(function () {
           $(this).remove();
        });
    };
    return {
        "proSystemChange":proSystemChange,
        "save":save,
        "searchDevice":searchDevice,
        "getDeviceDetail":getDeviceDetail,
        "update":update,
        "del":del,
        "operateModal":operateModal,
        "copy":copy,
        "batchInstance":batchInstance
    }
});
