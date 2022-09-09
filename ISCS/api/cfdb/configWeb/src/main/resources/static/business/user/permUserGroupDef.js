require(["../config/config"], function () {
    require(["commonService", "bootstrap", "validateHelp"], function (commonService, bootstrap, validateHelp) {
        $(document).ready(function () {
            var options = {
                "formSelector": "permUserGroupFormModal",//表单选择器
                "modalSelector": "permUserGroupModal",//模态框选择器
                "action": "/permUserGroup/changeLead",//请求地址
                "showInfoSelector": "permUserGroupTbl",//显示消息选择器
                "searchSelector": "search",//查询选择器
                "tableSelector": "permUserGroupTbl",//表选择器
                "searchUrl": "/permUserGroup/list",//查询地址
                "saveUrl": "/permUserGroup/saveOrUpdate",//保存地址
                "updateUrl": "/permUserGroup/saveOrUpdate",//修改地址
                "detailUrl": "/permUserGroup/detail",//详情地址
                "delUrl": "/permUserGroup/del",//删除地址
                "saveSelector": "permUserGroupSaveModal",//保存按钮选择器
                "updateSelector": "permUserGroupUpdateModal",//更新按钮选择器
                "labelSelector": "permUserGroupModalLabel",//模态框title选择器
                "autoAssignWhenAdd": false
            };
            var validateOptions = {
                "formSelector": "permUserGroupFormModal",//表单选择器
                "rules": ""
            };

            var dualOptions = {
                "modalSelectSelector": "#permUserGroupModal [name='userId']",
                "selectedLabel": "已选择",
                "nonSelectedLabel": "未选择"
            };
            commonService.init(options, dualOptions);
            commonService.initAction(options.action);

            var grayList = [];
            //加载需要的下拉框
            var types = [];
            types.push("USER");
            types.push("GROUP");
            commonService.initModal(grayList, "", types);
            //初始化校验validate
            validateHelp.validateInit(validateOptions);
            $("#permUserGroupSaveModal").on("click", function () {
                if(!validateHelp.valid()){
                    return false;
                }
                var userIds = $("#permUserGroupModal [name='userId']").val();
                var groupId = $("#permUserGroupModal [name='groupId']").val();
                var d = {
                    "userIds": userIds.join(","),
                    "groupId": groupId
                };
                commonService.save(d);
            });
            $("#searchPermUserGroup").click(function () {
                commonService.search();
            });
            //删除
            $("#permUserGroupTbl tbody").on('click', function (event) {
                var ev = event || window.event;
                var target = ev.target || ev.srcElement;
                if (target.nodeName.toLowerCase() == "a") {
                    var attrName = $(target).attr("name");
                    if (attrName == "del") {
                        var id = $(target).data("id");
                        var data = {"relationId": id};
                        commonService.del(data, $(target));
                    } else if (attrName == "cancelLead") {
                        var id = $(target).data("id");
                        var params = id.split(",");
                        var data = {};
                        data.relationId = params[0];
                        data.groupId = params[1];
                        data.ifLead = 0;
                        commonService.commonM(data);
                    } else if (attrName == "putLead") {
                        var id = $(target).data("id");
                        var params = id.split(",");
                        var data = {};
                        data.relationId = params[0];
                        data.groupId = params[1];
                        data.ifLead = 1;
                        commonService.commonM(data);
                    }
                }
            });
        });
    });
});