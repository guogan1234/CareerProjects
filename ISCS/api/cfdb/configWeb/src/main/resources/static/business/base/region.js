require(["../config/config"],function(){
    require(["commonService","bootstrap","validateHelp","devSelect"],function(commonService, bootstrap,validateHelp,devSelect){
        var options={
            "formSelector":"regionFormModal",//表单选择器
            "modalSelector":"regionModal",//模态框选择器
            "action":"",//请求地址
            "showInfoSelector":"regionTbl",//显示消息选择器
            "searchSelector":"search",//查询选择器
            "tableSelector":"regionTbl",//表选择器
            "searchUrl":"/region/list",//查询地址
            "saveUrl":"/region/saveOrUpdate",//保存地址
            "updateUrl":"/region/saveOrUpdate",//修改地址
            "detailUrl":"/region/detail",//详情地址
            "delUrl":"/region/del",//删除地址
            "saveSelector":"regionSaveModal",//保存按钮选择器
            "updateSelector":"regionUpdateModal",//更新按钮选择器
            "labelSelector":"regionModalLabel",//模态框title选择器
            "autoAssignWhenAdd":false
        };

        var validateOptions={
            "formSelector":"regionFormModal",//表单选择器
            "rules":""
        };
        $(document).ready(function(){
            devSelect.selectAssignment(null,"ZY",$("#proSystemId"));
            //初始化参数
            commonService.init(options);
            //初始化模态框
            //update时需要禁用的input
            var grayList=[];
            grayList.push("regionId");
            //加载需要的下拉框
            var types=[];
            types.push("ZY");
            commonService.initModal(grayList,"",types);
            //初始化校验validate
            validateHelp.validateInit(validateOptions);
            //保存
            $("#regionSaveModal").click(function () {
                if(!validateHelp.valid()){
                    return;
                }
                commonService.save();
            });
            //修改
            $("#regionUpdateModal").click(function () {
                if(!validateHelp.valid()){
                    return;
                }
                commonService.update();
            });
            $("#searchRegion").click(function () {
                commonService.search();
            });
            //删除
            $("#regionTbl tbody").on('click',function(event){
                var ev= event || window.event;
                var target = ev.target || ev.srcElement;
                if(target.nodeName.toLowerCase()=="a"){
                    var attrName=$(target).attr("name");
                    if(attrName=="del"){
                        var id=$(target).data("id");
                        var data={"regionId":id};
                        commonService.del(data,$(target));
                    }
                }
            });
        });
    });
});