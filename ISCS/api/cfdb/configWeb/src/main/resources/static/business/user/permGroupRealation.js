require(["../config/config"], function () {
    require(["commonService","bootstrap","validateHelp"], function (commonService,bootstrap,validateHelp) {
        $(document).ready(function () {
            var options={
                "formSelector":"permGroupRealationFormModal",//表单选择器
                "modalSelector":"permGroupRealationModal",//模态框选择器
                "action":"",//请求地址
                "showInfoSelector":"permGroupRealationTbl",//显示消息选择器
                "searchSelector":"search",//查询选择器
                "tableSelector":"permGroupRealationTbl",//表选择器
                "searchUrl":"/permGroupRealation/list",//查询地址
                "saveUrl":"/permGroupRealation/saveOrUpdate",//保存地址
                "updateUrl":"/permGroupRealation/saveOrUpdate",//修改地址
                "detailUrl":"/permGroupRealation/detail",//详情地址
                "delUrl":"/permGroupRealation/del",//删除地址
                "saveSelector":"permGroupRealationSaveModal",//保存按钮选择器
                "updateSelector":"permGroupRealationUpdateModal",//更新按钮选择器
                "labelSelector":"permGroupRealationModalLabel",//模态框title选择器
                "autoAssignWhenAdd":false
            };
            var validateOptions = {
                "formSelector": "permGroupRealationFormModal",//表单选择器
                "rules": ""
            };
            var dualOptions={
                "modalSelectSelector":"#permGroupRealationModal [name='permId']",
                "selectedLabel":"已选择",
                "nonSelectedLabel":"未选择"
            };
            commonService.init(options,dualOptions);
            var grayList=[];
            //加载需要的下拉框
            var types=[];
            types.push("CF");
            types.push("GROUP");
            commonService.initModal(grayList,"",types);
            //初始化校验validate
            validateHelp.validateInit(validateOptions);
            $("#permGroupRealationSaveModal").on("click",function () {
                if(!validateHelp.valid()){
                    return;
                }
                var permIds=$("#permGroupRealationModal [name='permId']").val();
                var groupId=$("#permGroupRealationModal [name='groupId']").val();
                var d={
                    "permIds":permIds.join(","),
                    "groupId":groupId
                };
                commonService.save(d);
            });
            $("#searchPermGroupRealation").click(function () {
                commonService.search();
            });
            //删除
            $("#permGroupRealationTbl tbody").on('click',function(event){
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