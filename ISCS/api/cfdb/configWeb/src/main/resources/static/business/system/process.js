require(["../config/config"],function(){
    require(["commonService","bootstrap","validateHelp","devSelect"],function(commonService, bootstrap,validateHelp,devSelect){
        var options={
            "formSelector":"processFormModal",//表单选择器
            "modalSelector":"processModal",//模态框选择器
            "action":"",//请求地址
            "showInfoSelector":"processTbl",//显示消息选择器
            "searchSelector":"search",//查询选择器
            "tableSelector":"processTbl",//表选择器
            "searchUrl":"/process/list",//查询地址
            "saveUrl":"/process/saveOrUpdate",//保存地址
            "updateUrl":"/process/saveOrUpdate",//修改地址
            "detailUrl":"/process/detail",//详情地址
            "delUrl":"/process/del",//删除地址
            "saveSelector":"processSaveModal",//保存按钮选择器
            "updateSelector":"processUpdateModal",//更新按钮选择器
            "labelSelector":"processModalLabel",//模态框title选择器
            "autoAssignWhenAdd":false
        };

        var validateOptions={
            "formSelector":"processFormModal",//表单选择器
            "rules":""
        };
        $(document).ready(function(){
            //devSelect.selectAssignment(null,"ZY",$("#proSystemId"));
            //初始化参数
            commonService.init(options);
            //初始化模态框
            //update时需要禁用的input
            var grayList=[];
            grayList.push("procId");
            //加载需要的下拉框
            var types=[];
            // types.push("ZY");
            commonService.initModal(grayList,"",types);
            //初始化校验validate
            validateHelp.validateInit(validateOptions);
            //保存
            $("#processSaveModal").click(function () {
                if(!validateHelp.valid()){
                    return;
                }
                commonService.save();
            });
            //修改
            $("#processUpdateModal").click(function () {
                if(!validateHelp.valid()){
                    return;
                }
                commonService.update();
            });
            $("#searchProcess").click(function () {
                commonService.search();
            });
            //删除
            $("#processTbl tbody").on('click',function(event){
                var ev= event || window.event;
                var target = ev.target || ev.srcElement;
                if(target.nodeName.toLowerCase()=="a"){
                    var attrName=$(target).attr("name");
                    if(attrName=="del"){
                        var id=$(target).data("id");
                        var data={"procId":id};
                        commonService.del(data,$(target));
                    }
                }
            });
        });
    });
});