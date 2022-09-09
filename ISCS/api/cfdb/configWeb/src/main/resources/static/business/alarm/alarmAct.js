/**
 * Created by zhoujs on 2018/6/12.
 */
require(["../config/config"],function(){
    require(["commonService","bootstrap","validateHelp","devSelect"],function(commonService, bootstrap,validateHelp,devSelect){
        var options={
            "formSelector":"alarmActFormModal",//表单选择器
            "modalSelector":"alarmActModal",//模态框选择器
            "action":"",//请求地址
            "showInfoSelector":"alarmActTbl",//显示消息选择器
            "searchSelector":"search",//查询选择器
            "tableSelector":"alarmActTbl",//表选择器
            "searchUrl":"/alarmAct/list",//查询地址
            "saveUrl":"/alarmAct/saveOrUpdate",//保存地址
            "updateUrl":"/alarmAct/saveOrUpdate",//修改地址
            "detailUrl":"/alarmAct/detail",//详情地址
            "delUrl":"/alarmAct/del",//删除地址
            "saveSelector":"alarmActSaveModal",//保存按钮选择器
            "updateSelector":"alarmActUpdateModal",//更新按钮选择器
            "labelSelector":"alarmActModalLabel",//模态框title选择器
            "autoAssignWhenAdd":true
        };

        var validateOptions={
            "formSelector":"alarmActFormModal",//表单选择器
            "rules":""
        };
        $(document).ready(function(){
            //初始化参数
            commonService.init(options);
            //初始化模态框
            //update时需要禁用的input
            var grayList=[];
            grayList.push("actId");
            //加载需要的下拉框
            var types=[];
            // types.push("ZY");
            commonService.initModal(grayList,"",types);
            //初始化校验validate
            validateHelp.validateInit(validateOptions);
            //保存
            $("#alarmActSaveModal").click(function () {
                if(!validateHelp.valid()){
                    return;
                }
                commonService.save();
            });
            //修改
            $("#alarmActUpdateModal").click(function () {
                if(!validateHelp.valid()){
                    return;
                }
                commonService.update();
            });
            $("#searchAlarmAct").click(function () {
                commonService.search();
            });
            //删除
            $("#alarmActTbl tbody").on('click',function(event){
                var ev= event || window.event;
                var target = ev.target || ev.srcElement;
                if(target.nodeName.toLowerCase()=="a"){
                    var attrName=$(target).attr("name");
                    if(attrName=="del"){
                        var id=$(target).data("id");
                        var data={"actId":id};
                        commonService.del(data,$(target));
                    }
                }
            });
        });
    });
});