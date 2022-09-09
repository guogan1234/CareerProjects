require(["../config/config"],function(){
    require(["commonService","bootstrap","validateHelp","devSelect"],function(commonService, bootstrap,validateHelp,devSelect){
        var options={
            "formSelector":"stationFormModal",//表单选择器
            "modalSelector":"stationModal",//模态框选择器
            "action":"",//请求地址
            "showInfoSelector":"stationTbl",//显示消息选择器
            "searchSelector":"search",//查询选择器
            "tableSelector":"stationTbl",//表选择器
            "searchUrl":"/station/list",//查询地址
            "saveUrl":"/station/saveOrUpdate",//保存地址
            "updateUrl":"/station/saveOrUpdate",//修改地址
            "detailUrl":"/station/detail",//详情地址
            "delUrl":"/station/del",//删除地址
            "saveSelector":"stationSaveModal",//保存按钮选择器
            "updateSelector":"stationUpdateModal",//更新按钮选择器
            "labelSelector":"stationModalLabel",//模态框title选择器
            "autoAssignWhenAdd":false
        };

        var validateOptions={
            "formSelector":"stationFormModal",//表单选择器
            "rules":""
        };
        $(document).ready(function(){
            //devSelect.selectAssignment(null,"ZY",$("#stationId"));
            //初始化参数
            commonService.init(options);
            //初始化模态框
            //update时需要禁用的input
            var grayList=[];
            grayList.push("stationId");
            //加载需要的下拉框
            var types=[];
            types.push("DM");
            // types.push("ZY");
            commonService.initModal(grayList,"",types);
            //初始化校验validate
            validateHelp.validateInit(validateOptions);
            //保存
            $("#stationSaveModal").click(function () {
                if(!validateHelp.valid()){
                    return;
                }
                commonService.save();
            });
            //修改
            $("#stationUpdateModal").click(function () {
                if(!validateHelp.valid()){
                    return;
                }
                commonService.update();
            });
            $("#searchStation").click(function () {
                commonService.search();
            });
            //删除
            $("#stationTbl tbody").on('click',function(event){
                var ev= event || window.event;
                var target = ev.target || ev.srcElement;
                if(target.nodeName.toLowerCase()=="a"){
                    var attrName=$(target).attr("name");
                    if(attrName=="del"){
                        var id=$(target).data("id");
                        var data={"stationId":id};
                        commonService.del(data,$(target));
                    }
                }
            });
        });
    });
});