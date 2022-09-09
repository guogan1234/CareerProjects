/**
 * Created by zhoujs on 2018/6/12.
 */
require(["../config/config"],function(){
    require(["commonService","bootstrap","validateHelp"],function(commonService, bootstrap,validateHelp){
        var options={
            "formSelector":"alarmLevelFormModal",//表单选择器
            "modalSelector":"alarmLevelModal",//模态框选择器
            "action":"",//请求地址
            "showInfoSelector":"alarmLevelTbl",//显示消息选择器
            "searchSelector":"search",//查询选择器
            "tableSelector":"alarmLevelTbl",//表选择器
            "searchUrl":"/alarmLevel/list",//查询地址
            "saveUrl":"/alarmLevel/saveOrUpdate",//保存地址
            "updateUrl":"/alarmLevel/saveOrUpdate",//修改地址
            "detailUrl":"/alarmLevel/detail",//详情地址
            "delUrl":"/alarmLevel/del",//删除地址
            "saveSelector":"alarmLevelSaveModal",//保存按钮选择器
            "updateSelector":"alarmLevelUpdateModal",//更新按钮选择器
            "labelSelector":"alarmLevelModalLabel",//模态框title选择器
            "autoAssignWhenAdd":true
        };

        var validateOptions={
            "formSelector":"alarmLevelFormModal",//表单选择器
            "rules":""
        };
        $(document).ready(function(){
            //初始化参数
            commonService.init(options);
            //初始化模态框
            //update时需要禁用的input
            var grayList=[];
            grayList.push("levelId");
            // grayList.push("levelName");
            //加载需要的下拉框
            var types=[];
            types.push("AC");
            commonService.initModal(grayList,"",types);
            //初始化校验validate
            validateHelp.validateInit(validateOptions);
            //保存
            $("#alarmLevelSaveModal").click(function () {
                if(!validateHelp.valid()){
                    return;
                }
                var serializeObj = {};
                var selector = $("#"+options.formSelector);
                $(selector.serializeArray()).each(function () {
                    if (stringUtils.isNotBlank(this.value)) {
                        serializeObj[this.name] = this.value;
                    }
                });
                var alarmActCheck = $("#alarmActCheck").val();
                if(stringUtils.isNotBlank(alarmActCheck)){
                    serializeObj["alarmAct"] = alarmActCheck.join(",");
                }
                serializeObj["soundFile"] = $("#soundFile").val();

                commonService.save(serializeObj);
            });
            //修改
            $("#alarmLevelUpdateModal").click(function () {
                if(!validateHelp.valid()){
                    return;
                }
                var serializeObj = {};
                var selector = $("#"+options.formSelector);
                $(selector.serializeArray()).each(function () {
                    if (stringUtils.isNotBlank(this.value)) {
                        serializeObj[this.name] = this.value;
                    }
                });
                var alarmActCheck = $("#alarmActCheck").val();
                if(stringUtils.isNotBlank(alarmActCheck)){
                    serializeObj["alarmAct"] = alarmActCheck.join(",");
                }
                serializeObj["soundFile"] = $("#soundFile").val();
                commonService.update(serializeObj);
            });
            $("#searchAlarmLevel").click(function () {
                commonService.search();
            });
            //删除
            $("#alarmLevelTbl tbody").on('click',function(event){
                var ev= event || window.event;
                var target = ev.target || ev.srcElement;
                if(target.nodeName.toLowerCase()=="a"){
                    var attrName=$(target).attr("name");
                    if(attrName=="del"){
                        var id=$(target).data("id");
                        var data={"levelId":id};
                        commonService.del(data,$(target));
                    }
                }
            });
        });
    });
});