require(["../config/config"],function(){
    require(["commonService","bootstrap","validateHelp","devSelect"],function(commonService, bootstrap,validateHelp,devSelect){
        var options={
            "formSelector":"permSecurityFormModal",//表单选择器
            "modalSelector":"permSecurityModal",//模态框选择器
            "action":"",//请求地址
            "showInfoSelector":"permSecurityTbl",//显示消息选择器
            "searchSelector":"search",//查询选择器
            "tableSelector":"permSecurityTbl",//表选择器
            "searchUrl":"/permSecurity/list",//查询地址
            "saveUrl":"/permSecurity/saveOrUpdate",//保存地址
            "updateUrl":"/permSecurity/saveOrUpdate",//修改地址
            "detailUrl":"/permSecurity/detail",//详情地址
            "delUrl":"/permSecurity/del",//删除地址
            "saveSelector":"permSecuritySaveModal",//保存按钮选择器
            "updateSelector":"permSecurityUpdateModal",//更新按钮选择器
            "labelSelector":"permSecurityModalLabel",//模态框title选择器
            "autoAssignWhenAdd":false
        };

        var validateOptions={
            "formSelector":"permSecurityFormModal",//表单选择器
            "rules":""
        };
        $(document).ready(function(){
            //devSelect.selectAssignment(null,"ZY",$("#proSystemId"));
            //初始化参数
            commonService.init(options);
            //初始化模态框
            //update时需要禁用的input
            var grayList=[];
            grayList.push("permSecurityId");
            //加载需要的下拉框
            var types=[];
            // types.push("ZY");
            commonService.initModal(grayList,"",types);
            //初始化校验validate
            validateHelp.validateInit(validateOptions);
            //保存
            $("#permSecuritySaveModal").click(function () {
                if(!validateHelp.valid()){
                    return;
                }
                commonService.save();
            });
            //修改
            $("#permSecurityUpdateModal").click(function () {
                if(!validateHelp.valid()){
                    return;
                }
                commonService.update();
            });
            $("#searchPermSecurity").click(function () {
                commonService.search();
            });
            //删除
            $("#permSecurityTbl tbody").on('click',function(event){
                var ev= event || window.event;
                var target = ev.target || ev.srcElement;
                if(target.nodeName.toLowerCase()=="a"){
                    var attrName=$(target).attr("name");
                    if(attrName=="del"){
                        var id=$(target).data("id");
                        var data={"permSecurityId":id};
                        commonService.del(data,$(target));
                    }
                }
            });
        });
    });
});