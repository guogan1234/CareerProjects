require(["../config/config"], function () {
    require(["commonService","bootstrap","validateHelp"], function (commonService,bootstrap,validateHelp) {
        $(document).ready(function () {
            var options={
                "formSelector":"permUserDepartmentFormModal",//表单选择器
                "modalSelector":"permUserDepartmentModal",//模态框选择器
                "action":"",//请求地址
                "showInfoSelector":"permUserDepartmentTbl",//显示消息选择器
                "searchSelector":"search",//查询选择器
                "tableSelector":"permUserDepartmentTbl",//表选择器
                "searchUrl":"/permUserDepartment/list",//查询地址
                "saveUrl":"/permUserDepartment/saveOrUpdate",//保存地址
                "updateUrl":"/permUserDepartment/saveOrUpdate",//修改地址
                "detailUrl":"/permUserDepartment/detail",//详情地址
                "delUrl":"/permUserDepartment/del",//删除地址
                "saveSelector":"permUserDepartmentSaveModal",//保存按钮选择器
                "updateSelector":"permUserDepartmentUpdateModal",//更新按钮选择器
                "labelSelector":"permUserDepartmentModalLabel",//模态框title选择器
                "autoAssignWhenAdd":false
            };
            var validateOptions = {
                "formSelector": "permUserDepartmentFormModal",//表单选择器
                "rules": ""
            };
            var dualOptions={
                "modalSelectSelector":"#permUserDepartmentModal [name='userId']",
                "selectedLabel":"已选择",
                "nonSelectedLabel":"未选择"
            };
            commonService.init(options,dualOptions);
            var grayList=[];
            //加载需要的下拉框
            var types=[];
            types.push("DEP");
            types.push("USER");
            commonService.initModal(grayList,"",types);
            //初始化校验validate
            validateHelp.validateInit(validateOptions);
            $("#permUserDepartmentSaveModal").on("click",function () {
                if(!validateHelp.valid()){
                    return;
                }

                var userIds=$("#permUserDepartmentModal [name='userId']").val();
                var deptId=$("#permUserDepartmentModal [name='deptId']").val();
                var d={
                    "userIds":userIds.join(","),
                    "deptId":deptId
                };
                commonService.save(d);
            });
            $("#searchPermUserDepartment").click(function () {
                commonService.search();
            });
            //删除
            $("#permUserDepartmentTbl tbody").on('click',function(event){
                var ev= event || window.event;
                var target = ev.target || ev.srcElement;
                if(target.nodeName.toLowerCase()=="a"){
                    var attrName=$(target).attr("name");
                    if(attrName=="del"){
                        var id=$(target).data("id");
                        var data={"relationId":id};
                        commonService.del(data,$(target));
                    }
                }
            });
        });
    });
});