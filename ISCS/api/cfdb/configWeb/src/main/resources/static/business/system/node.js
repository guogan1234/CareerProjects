require(["../config/config"],function(){
    require(["commonService","bootstrap","validateHelp","devSelect"],function(commonService, bootstrap,validateHelp,devSelect){
        var options={
            "formSelector":"nodeFormModal",//表单选择器
            "modalSelector":"nodeModal",//模态框选择器
            "action":"",//请求地址
            "showInfoSelector":"nodeTbl",//显示消息选择器
            "searchSelector":"search",//查询选择器
            "tableSelector":"nodeTbl",//表选择器
            "searchUrl":"/node/list",//查询地址
            "saveUrl":"/node/saveOrUpdate",//保存地址
            "updateUrl":"/node/saveOrUpdate",//修改地址
            "detailUrl":"/node/detail",//详情地址
            "delUrl":"/node/del",//删除地址
            "saveSelector":"nodeSaveModal",//保存按钮选择器
            "updateSelector":"nodeUpdateModal",//更新按钮选择器
            "labelSelector":"nodeModalLabel",//模态框title选择器
            "autoAssignWhenAdd":false,
            "autoHideSelectors":"nic1Name,nic1Mask,nic1Address,nic2Name,nic2Mask,nic2Address,nic3Name,nic3Mask,nic3Address,nic4Name,nic4Mask,nic4Address",
            "controlNumSelector":"nicNum"
        };

        var validateOptions={
            "formSelector":"nodeFormModal",//表单选择器
            "rules":{
                nic1Mask:{
                    checkMask:[]
                },
                nic1Address:{
                    checkMac:[]
                },
                nic2Mask:{
                    checkMask:[]
                },
                nic2Address:{
                    checkMac:[]
                },
                nic3Mask:{
                    checkMask:[]
                },
                nic3Address:{
                    checkMac:[]
                },
                nic4Mask:{
                    checkMask:[]
                },
                nic4Address:{
                    checkMac:[]
                }
            }
        };
        $(document).ready(function(){
            //devSelect.selectAssignment(null,"ZY",$("#proSystemId"));
            //初始化参数
            commonService.init(options);
            //初始化模态框
            //update时需要禁用的input
            var grayList=[];
            grayList.push("nodeId");
            //加载需要的下拉框
            var types=[];
            // types.push("ZY");
            commonService.initModal(grayList,"",types);
            //初始化校验validate
            validateHelp.validateInit(validateOptions);
            //保存
            $("#nodeSaveModal").click(function () {
                if(!validateHelp.valid()){
                    return;
                }
                commonService.save();
            });
            //修改
            $("#nodeUpdateModal").click(function () {
                if(!validateHelp.valid()){
                    return;
                }

                commonService.update();
            });
            $("#searchNode").click(function () {
                commonService.search();
            });
            //删除
            $("#nodeTbl tbody").on('click',function(event){
                var ev= event || window.event;
                var target = ev.target || ev.srcElement;
                if(target.nodeName.toLowerCase()=="a"){
                    var attrName=$(target).attr("name");
                    if(attrName=="del"){
                        var id=$(target).data("id");
                        var data={"nodeId":id};
                        commonService.del(data,$(target));
                    }
                }
            });

            $("#nodeModal").on('change',function(event) {
                var ev = event || window.event;
                var target = ev.target || ev.srcElement;
                if (target.nodeName.toLowerCase() == "input") {
                    var attrName=$(target).attr("name");
                    if (attrName=="nicNum"){
                        var v=$(target).val();
                        if(v!=null && v!="" && Number(v)>=1 && Number(v)<=4){
                            for(var i=1;i<=4;i++){
                                $("#nodeModal").find("input[name='nic"+i+"Name']").each(function(){
                                    $(this).parent().parent().show();
                                });
                                $("#nodeModal").find("input[name='nic"+i+"Mask']").each(function(){
                                    $(this).parent().parent().show();
                                });
                            }
                            for(var i=Number(v)+1;i<=4;i++){
                                $("#nodeModal").find("input[name='nic"+i+"Name']").each(function(){
                                    $(this).parent().parent().hide();
                                });
                                $("#nodeModal").find("input[name='nic"+i+"Mask']").each(function(){
                                    $(this).parent().parent().hide();
                                });
                            }
                        }
                    }
                }
            });
        });
    });
});