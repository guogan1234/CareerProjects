/**
 * Created by zhoujs on 2018/6/12.
 */
require(["../config/config"],function(){
    require(["commonService","bootstrap","validateHelp"],function(commonService, bootstrap,validateHelp){
        var options={
            "formSelector":"alarmTypeFormModal",//表单选择器
            "modalSelector":"alarmTypeModal",//模态框选择器
            "action":"",//请求地址
            "showInfoSelector":"alarmTypeTbl",//显示消息选择器
            "searchSelector":"search",//查询选择器
            "tableSelector":"alarmTypeTbl",//表选择器
            "searchUrl":"/alarmType/list",//查询地址
            "saveUrl":"/alarmType/saveOrUpdate",//保存地址
            "updateUrl":"/alarmType/saveOrUpdate",//修改地址
            "detailUrl":"/alarmType/detail",//详情地址
            "delUrl":"/alarmType/del",//删除地址
            "saveSelector":"alarmTypeSaveModal",//保存按钮选择器
            "updateSelector":"alarmTypeUpdateModal",//更新按钮选择器
            "labelSelector":"alarmTypeModalLabel"//模态框title选择器
        };
        var validateOptions={
            "formSelector":"alarmTypeFormModal",//表单选择器
            "rules":""
        };
        $(document).ready(function(){
            //初始化参数
            commonService.init(options);
            //初始化模态框
            var grayList=[];
            grayList.push("typeId");
            var types=[];
            types.push("AL");
            types.push("AT");
            commonService.initModal(grayList,"",types);
            //初始化校验validate
            validateHelp.validateInit(validateOptions);
            //保存
            $("#alarmTypeSaveModal").click(function () {
                if(!validateHelp.valid()){
                    return;
                }
                //修改地址
                commonService.save();
            });
            //修改
            $("#alarmTypeUpdateModal").click(function () {
                commonService.update();
            });
            $("#searchAlarmType").click(function () {
                commonService.search();
            });
            //删除
            $("#alarmTypeTbl tbody").on('click',function(event){
                var ev= event || window.event;
                var target = ev.target || ev.srcElement;
                if(target.nodeName.toLowerCase()=="a"){
                    var attrName=$(target).attr("name");
                    if(attrName=="del"){
                        var id=$(target).data("id");
                        var data={"typeId":id};
                        // commonService.initAction("/domain/del");
                        commonService.del(data,$(target));
                    }
                }
            });
        });
    });
});