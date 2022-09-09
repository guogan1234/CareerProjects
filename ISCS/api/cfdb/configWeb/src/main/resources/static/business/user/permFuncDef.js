require(["../config/config"],function(){
    require(["commonService","bootstrap","validateHelp","devSelect"],function(commonService, bootstrap,validateHelp,devSelect){
        var options={
            "formSelector":"permFuncFormModal",//表单选择器
            "modalSelector":"permFuncModal",//模态框选择器
            "action":"",//请求地址
            "showInfoSelector":"permFuncTbl",//显示消息选择器
            "searchSelector":"search",//查询选择器
            "tableSelector":"permFuncTbl",//表选择器
            "searchUrl":"/permFunc/list",//查询地址
            "saveUrl":"/permFunc/saveOrUpdate",//保存地址
            "updateUrl":"/permFunc/saveOrUpdate",//修改地址
            "detailUrl":"/permFunc/detail",//详情地址
            "delUrl":"/permFunc/del",//删除地址
            "saveSelector":"permFuncSaveModal",//保存按钮选择器
            "updateSelector":"permFuncUpdateModal",//更新按钮选择器
            "labelSelector":"permFuncModalLabel",//模态框title选择器
            "autoAssignWhenAdd":false
        };

        var validateOptions={
            "formSelector":"permFuncFormModal",//表单选择器
            "rules":""
        };
        $(document).ready(function(){
            //devSelect.selectAssignment(null,"ZY",$("#proSystemId"));
            //初始化参数
            commonService.init(options);
            //初始化模态框
            //update时需要禁用的input
            var grayList=[];
            grayList.push("permId");
            //加载需要的下拉框
            var types=[];
             types.push("PT");
            commonService.initModal(grayList,"",types);
            //初始化校验validate
            validateHelp.validateInit(validateOptions);
            //保存
            $("#permFuncSaveModal").click(function () {
                if(!validateHelp.valid()){
                    return;
                }
                commonService.save();
            });
            //修改
            $("#permFuncUpdateModal").click(function () {
                if(!validateHelp.valid()){
                    return;
                }
                commonService.update();
            });
            $("#searchPermFunc").click(function () {
                commonService.search();
            });
            //删除
            $("#permFuncTbl tbody").on('click',function(event){
                var ev= event || window.event;
                var target = ev.target || ev.srcElement;
                if(target.nodeName.toLowerCase()=="a"){
                    var attrName=$(target).attr("name");
                    if(attrName=="del"){
                        var id=$(target).data("id");
                        var data={"permId":id};
                        commonService.del(data,$(target));
                    }
                }
            });
        });
    });
});