require(["../config/config"],function(){
    require(["commonService","bootstrap","validateHelp","devSelect"],function(commonService, bootstrap,validateHelp,devSelect){
        var options={
            "formSelector":"diTextFormModal",//表单选择器
            "modalSelector":"diTextModal",//模态框选择器
            "action":"",//请求地址
            "showInfoSelector":"diTextTbl",//显示消息选择器
            "searchSelector":"search",//查询选择器
            "tableSelector":"diTextTbl",//表选择器
            "searchUrl":"/diText/list",//查询地址
            "saveUrl":"/diText/saveOrUpdate",//保存地址
            "updateUrl":"/diText/saveOrUpdate",//修改地址
            "detailUrl":"/diText/detail",//详情地址
            "delUrl":"/diText/del",//删除地址
            "saveSelector":"diTextSaveModal",//保存按钮选择器
            "updateSelector":"diTextUpdateModal",//更新按钮选择器
            "labelSelector":"diTextModalLabel",//模态框title选择器
            "autoAssignWhenAdd":true
        };

        var validateOptions={
            "formSelector":"diTextFormModal",//表单选择器
            "rules":""
        };
        $(document).ready(function(){
            devSelect.selectAssignment(null,"ZY",$("#proSystemId"));
            //初始化参数
            commonService.init(options);
            //初始化模态框
            //update时需要禁用的input
            var grayList=[];
            grayList.push("diTextLabel");
            grayList.push("diValue");
            //加载需要的下拉框
            var types=[];
            types.push("ZY");
            commonService.initModal(grayList,"",types);
            //初始化校验validate
            validateHelp.validateInit(validateOptions);
            //保存
            $("#diTextSaveModal").click(function () {
                if(!validateHelp.valid()){
                    return;
                }
                commonService.save();
            });
            //修改
            $("#diTextUpdateModal").click(function () {
                if(!validateHelp.valid()){
                    return;
                }
                commonService.update();
            });
            $("#searchDiText").click(function () {
                commonService.search();
            });
            //删除
            $("#diTextTbl tbody").on('click',function(event){
                var ev= event || window.event;
                var target = ev.target || ev.srcElement;
                if(target.nodeName.toLowerCase()=="a"){
                    var attrName=$(target).attr("name");
                    if(attrName=="del"){
                        var id=$(target).data("id");
                        var ids=id.split(",");
                        var data={"diTextLabel":ids[0],"diValue":ids[1]};
                        commonService.del(data,$(target));
                    }
                }
            });
        });
    });
});