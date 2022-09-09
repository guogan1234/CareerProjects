require(["../config/config"],function(){
    require(["commonService","bootstrap","validateHelp","devSelect"],function(commonService, bootstrap,validateHelp,devSelect){
        var options={
            "formSelector":"domainFormModal",//表单选择器
            "modalSelector":"domainModal",//模态框选择器
            "action":"",//请求地址
            "showInfoSelector":"domainTbl",//显示消息选择器
            "searchSelector":"search",//查询选择器
            "tableSelector":"domainTbl",//表选择器
            "searchUrl":"/domain/list",//查询地址
            "saveUrl":"/domain/saveOrUpdate",//保存地址
            "updateUrl":"/domain/saveOrUpdate",//修改地址
            "detailUrl":"/domain/detail",//详情地址
            "delUrl":"/domain/del",//删除地址
            "saveSelector":"domainSaveModal",//保存按钮选择器
            "updateSelector":"domainUpdateModal",//更新按钮选择器
            "labelSelector":"domainModalLabel",//模态框title选择器
            "autoAssignWhenAdd":false
        };

        var validateOptions={
            "formSelector":"domainFormModal",//表单选择器
            "rules":""
        };
        $(document).ready(function(){
            //devSelect.selectAssignment(null,"ZY",$("#proSystemId"));
            //初始化参数
            commonService.init(options);
            //初始化模态框
            //update时需要禁用的input
            var grayList=[];
            grayList.push("domainId");
            //加载需要的下拉框
            var types=[];
            // types.push("ZY");
            commonService.initModal(grayList,"",types);
            //初始化校验validate
            validateHelp.validateInit(validateOptions);
            //保存
            $("#domainSaveModal").click(function () {
                if(!validateHelp.valid()){
                    return;
                }
                commonService.save();
            });
            //修改
            $("#domainUpdateModal").click(function () {
                if(!validateHelp.valid()){
                    return;
                }
                commonService.update();
            });
            $("#searchDomain").click(function () {
                commonService.search();
            });
            //删除
            $("#domainTbl tbody").on('click',function(event){
                var ev= event || window.event;
                var target = ev.target || ev.srcElement;
                if(target.nodeName.toLowerCase()=="a"){
                    var attrName=$(target).attr("name");
                    if(attrName=="del"){
                        var id=$(target).data("id");
                        var data={"domainId":id};
                        commonService.del(data,$(target));
                    }
                }
            });
        });
    });
});