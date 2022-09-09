require(["../config/config"],function(){
    require(["commonService","bootstrap","validateHelp"],function(commonService, bootstrap,validateHelp){
        var options={
            "formSelector":"devTypeFormModal",//表单选择器
            "modalSelector":"devTypeModal",//模态框选择器
            "action":"",//请求地址
            "showInfoSelector":"devTypeTbl",//显示消息选择器
            "searchSelector":"search",//查询选择器
            "tableSelector":"devTypeTbl",//表选择器
            "searchUrl":"/devType/list",//查询地址
            "saveUrl":"/devType/saveOrUpdate",//保存地址
            "updateUrl":"/devType/saveOrUpdate",//修改地址
            "detailUrl":"/devType/detail",//详情地址
            "delUrl":"/devType/del",//删除地址
            "saveSelector":"devTypeSaveModal",//保存按钮选择器
            "updateSelector":"devTypeUpdateModal",//更新按钮选择器
            "labelSelector":"devTypeModalLabel"//模态框title选择器
        };

        var validateOptions={
            "formSelector":"devTypeFormModal",//表单选择器
            "rules":""
        };
        $(document).ready(function(){
            //初始化参数
            commonService.init(options);
            //初始化模态框
            //update时需要禁用的input
            var grayList=[];
            grayList.push("devTypeLabel");
            //加载需要的下拉框
            var types=[];
            types.push("ZY");
            commonService.initModal(grayList,"",types);
            //初始化校验validate
            validateHelp.validateInit(validateOptions);
            //保存
            $("#devTypeSaveModal").click(function () {
                if(!validateHelp.valid()){
                    return;
                }
                commonService.save();
            });
            //修改
            $("#devTypeUpdateModal").click(function () {
                if(!validateHelp.valid()){
                    return;
                }
                commonService.update();
            });
            $("#searchDevType").click(function () {
                commonService.search();
            });
            //删除
            $("#devTypeTbl tbody").on('click',function(event){
                var ev= event || window.event;
                var target = ev.target || ev.srcElement;
                if(target.nodeName.toLowerCase()=="a"){
                    var attrName=$(target).attr("name");
                    if(attrName=="del"){
                        var id=$(target).data("id");
                        var data={"devTypeLabel":id};
                        commonService.del(data,$(target));
                    }
                }
            });
        });
    });
});