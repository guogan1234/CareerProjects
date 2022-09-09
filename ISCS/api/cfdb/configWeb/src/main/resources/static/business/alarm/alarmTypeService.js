/**
 * Created by zhoujs on 2018/6/12.
 */
define(['ajax','form','modalHelp'],function (ajax,form,modalHelp) {
    var operateModal=function () {
        $("#alarmTypeModal").on('show.bs.modal',function (event) {
            $.fn.modal.Constructor.prototype.enforceFocus = function () { };
            var b = $(event.relatedTarget);
            var modal = $(this);
            var operate=b.data("operate");
            modalHelp.clearModal($("#alarmTypeFormModal"));
            if (operate=="add"){//新增操作
                $("#alarmTypeSaveModal").show();
                $("#alarmTypeUpdateModal").hide();
                $("#alarmTypeModalLabel").html("新增");
            }else if (operate=="update"){
                var id=b.data('id');
                getDetail(id);
                modal.find("[name='actId']").prop("readOnly",true);
                $("#alarmTypeSaveModal").hide();
                $("#alarmTypeUpdateModal").show();
                $("#alarmTypeModalLabel").html("修改");
            }else if(operate=="detail"){
                var id=b.data("id");
                getDetail(id);
                modalHelp.forbidModal($("#alarmTypeFormModal"));
                $("#alarmTypeModalLabel").html("查看");
                $("#alarmTypeSaveModal").hide();
                $("#alarmTypeUpdateModal").hide();
            }
        });
    };

    var alarmTypeHtml=function (typeId,typeName) {
        var h=[];
        h.push('<tr data-id="'+typeId+'">');
        h.push('<td>'+typeId+'</td>');
        h.push('<td><a href="#" name="actName" data-operate="detail" data-toggle="modal" data-target="#alarmTypeModal" data-id="'+typeId+'">'+typeName+'</a></td>');
        h.push('<td>');
        h.push('<a href="#" name="del" data-id="'+typeId+'">删除 </a>');
        h.push('<a href="#" data-toggle="modal" data-operate="update" data-target="#alarmTypeModal" data-id="'+typeId+'"> 修改</a>');
        h.push('</td></tr>');
        return h.join("");
    };

    var saveAlarmType=function () {
        var data=form.formInit($("#alarmTypeFormModal"));
        ajax.ajaxInit("/alarmType/saveOrUpdate",data,true);
        ajax.post(saveCal,"","alarmTypeTbl");
    };
    var saveCal=function (d) {
        var typeId=$("#typeId").val().trim();
        var typeName=$("#typeName").val().trim();
        searchAlarmType(typeId,typeName);
        $("#alarmTypeModal").modal('hide');
    };

    var updateAlarmType=function () {
        var data=form.formInit($("#alarmTypeFormModal"));
        data.operate="update";
        ajax.ajaxInit("/alarmType/saveOrUpdate",data,true);
        ajax.post(updateCal,"","alarmTypeTbl");
    };
    var updateCal=function (d) {
        var typeId=$("#typeId").val().trim();
        var typeName=$("#typeName").val().trim();
        searchAlarmType(typeId,typeName);
        $("#alarmTypeModal").modal('hide');
    };
    var getDetail=function (id) {
        var data={"typeId":id};
        ajax.ajaxInit("/alarmType/detail",data,false);
        ajax.post(detailCal,"","alarmTypeFormModal");
    };
    var detailCal=function (d) {
        form.setForm($("#alarmTypeFormModal"),d);
    };

    var searchAlarmType=function (alarmTypeId,alarmTypeName) {
        var  data={"typeId":alarmTypeId,"typeName":alarmTypeName};
        ajax.ajaxInit("/alarmType/list",data,true);
        ajax.post(searchCal,"","alarmTypeTbl");
    };
    var searchCal=function (en) {
        var html=[];
        for (var i=0;i<en.length;i++){
            html.push(alarmTypeHtml(en[i].typeId,en[i].typeName));
        }
        $("#alarmTypeTbl tbody").html(html.join(""));
    };

    var del=function (thi) {
        if (confirm("确定要删除吗？")){
            var id=thi.data('id');
            var data={"typeId":id};
            ajax.ajaxInit("/alarmType/del",data,true);
            ajax.post(delCal,thi,"alarmTypeTbl");
        }
    };
    var delCal=function (d) {
        $(d[0]).parent().parent().remove();
    };


    return{
        "saveAlarmType":saveAlarmType,
        "operateModal":operateModal,
        "searchAlarmType":searchAlarmType,
        "getDetail":getDetail,
        "updateAlarmType":updateAlarmType,
        "del":del
    }
});