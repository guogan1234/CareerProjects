require(["../config/config"],function(){
    require(["commonService","bootstrap","validateHelp","devSelect"],function(commonService, bootstrap,validateHelp,devSelect){
        var options={
            "formSelector":"userFormModal",//表单选择器
            "modalSelector":"userModal",//模态框选择器
            "action":"",//请求地址
            "showInfoSelector":"userTbl",//显示消息选择器
            "searchSelector":"search",//查询选择器
            "tableSelector":"userTbl",//表选择器
            "searchUrl":"/user/list",//查询地址
            "saveUrl":"/user/saveOrUpdate",//保存地址
            "updateUrl":"/user/saveOrUpdate",//修改地址
            "detailUrl":"/user/detail",//详情地址
            "delUrl":"/user/del",//删除地址
            "saveSelector":"userSaveModal",//保存按钮选择器
            "updateSelector":"userUpdateModal",//更新按钮选择器
            "labelSelector":"userModalLabel",//模态框title选择器
            "autoAssignWhenAdd":false
        };

        var validateOptions={
            "formSelector":"userFormModal",//表单选择器
            "rules":""
        };
        $(document).ready(function(){
            //devSelect.selectAssignment(null,"ZY",$("#proSystemId"));
            //初始化参数
            commonService.init(options);
            //初始化模态框
            //update时需要禁用的input
            var grayList=[];
            grayList.push("userId");
            grayList.push("userPassword");
            //加载需要的下拉框
            var types=[];
            // types.push("ZY");
            commonService.initModal(grayList,"",types);
            //初始化校验validate
            validateHelp.validateInit(validateOptions);
            //保存
            $("#userSaveModal").click(function () {
                if(!validateHelp.valid()){
                    return;
                }
                commonService.save();
            });
            //修改
            $("#userUpdateModal").click(function () {
                if(!validateHelp.valid()){
                    return;
                }
                commonService.update();
            });
            $("#searchUser").click(function () {
                commonService.search();
            });
            //删除
            $("#userTbl tbody").on('click',function(event){
                var ev= event || window.event;
                var target = ev.target || ev.srcElement;
                if(target.nodeName.toLowerCase()=="a"){
                    var attrName=$(target).attr("name");
                    if(attrName=="del"){
                        var id=$(target).data("id");
                        var data={"userId":id};
                        commonService.del(data,$(target));
                    }else if (attrName=="forbid"){//失效
                        var id=$(target).data("id");
                        var data={"userId":id};
                        var action="/user/forbid";
                        commonService.initAction(action);
                        commonService.commonM(data);
                    }else if (attrName=="lock"){//锁定
                        var id=$(target).data("id");
                        var data={"userId":id};
                        var action="/user/lock";
                        commonService.initAction(action);
                        commonService.commonM(data);
                    }else if (attrName=="cancelLock"){//解除锁定
                        var id=$(target).data("id");
                        var data={"userId":id};
                        var action="/user/cancelLock";
                        commonService.initAction(action);
                        commonService.commonM(data);
                    }
                }
            });
        });
    });
});