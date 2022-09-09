require(["../config/config"],function(){
    require(["commonService","bootstrap","validateHelp","devSelect"],function(commonService, bootstrap,validateHelp,devSelect){
        var options={
            "formSelector":"proSystemFormModal",//表单选择器
            "modalSelector":"proSystemModal",//模态框选择器
            "action":"",//请求地址
            "showInfoSelector":"proSystemTbl",//显示消息选择器
            "searchSelector":"search",//查询选择器
            "tableSelector":"proSystemTbl",//表选择器
            "searchUrl":"/proSystem/list",//查询地址
            "saveUrl":"/proSystem/saveOrUpdate",//保存地址
            "updateUrl":"/proSystem/saveOrUpdate",//修改地址
            "detailUrl":"/proSystem/detail",//详情地址
            "delUrl":"/proSystem/del",//删除地址
            "saveSelector":"proSystemSaveModal",//保存按钮选择器
            "updateSelector":"proSystemUpdateModal",//更新按钮选择器
            "labelSelector":"proSystemModalLabel",//模态框title选择器
            "autoAssignWhenAdd":false
        };

        var validateOptions={
            "formSelector":"proSystemFormModal",//表单选择器
            "rules":""
        };
        $(document).ready(function(){
            //devSelect.selectAssignment(null,"ZY",$("#proSystemId"));
            //初始化参数
            commonService.init(options);
            //初始化模态框
            //update时需要禁用的input
            var grayList=[];
            grayList.push("proSystemId");
            //加载需要的下拉框
            var types=[];
            // types.push("ZY");
            commonService.initModal(grayList,"",types);
            //初始化校验validate
            validateHelp.validateInit(validateOptions);
            //保存
            $("#proSystemSaveModal").click(function () {
                if(!validateHelp.valid()){
                    return;
                }
                commonService.save();
            });
            //修改
            $("#proSystemUpdateModal").click(function () {
                if(!validateHelp.valid()){
                    return;
                }
                commonService.update();
            });
            $("#searchProSystem").click(function () {
                commonService.search();
            });
            //删除
            $("#proSystemTbl tbody").on('click',function(event){
                var ev= event || window.event;
                var target = ev.target || ev.srcElement;
                if(target.nodeName.toLowerCase()=="a"){
                    var attrName=$(target).attr("name");
                    if(attrName=="del"){
                        var id=$(target).data("id");
                        var data={"proSystemId":id};
                        commonService.del(data,$(target));
                    }
                }
            });
        });
    });
});