/**
 * Created by zhoujs on 2018/6/12.
 */
require(["../config/config"], function () {
    require(["commonService", "bootstrap", "validateHelp", "devSelect"], function (commonService, bootstrap, validateHelp, devSelect) {
        var options = {
            "formSelector": "permDepartmentFormModal",//表单选择器
            "modalSelector": "permDepartmentModal",//模态框选择器
            "action": "",//请求地址
            "showInfoSelector": "permDepartmentTbl",//显示消息选择器
            "searchSelector": "search",//查询选择器
            "tableSelector": "permDepartmentTbl",//表选择器
            "searchUrl": "/permDepartment/list",//查询地址
            "saveUrl": "/permDepartment/saveOrUpdate",//保存地址
            "updateUrl": "/permDepartment/saveOrUpdate",//修改地址
            "detailUrl": "/permDepartment/detail",//详情地址
            "delUrl": "/permDepartment/del",//删除地址
            "saveSelector": "permDepartmentSaveModal",//保存按钮选择器
            "updateSelector": "permDepartmentUpdateModal",//更新按钮选择器
            "labelSelector": "permDepartmentModalLabel",//模态框title选择器
            "autoAssignWhenAdd": true
        };

        var validateOptions = {
            "formSelector": "permDepartmentFormModal",//表单选择器
            "rules": ""
        };
        $(document).ready(function () {
            //初始化参数
            commonService.init(options);
            //初始化模态框
            //update时需要禁用的input
            var grayList = [];
            grayList.push("deptId");
            //加载需要的下拉框
            var types = [];
            // types.push("ZY");
            commonService.initModal(grayList, "", types);
            //初始化校验validate
            validateHelp.validateInit(validateOptions);
            //保存
            $("#permDepartmentSaveModal").click(function () {
                if (!validateHelp.valid()) {
                    return;
                }
                commonService.save();
            });
            //修改
            $("#permDepartmentUpdateModal").click(function () {
                if (!validateHelp.valid()) {
                    return;
                }
                commonService.update();
            });
            $("#searchPermDepartment").click(function () {
                commonService.search();
            });
            //删除
            $("#permDepartmentTbl tbody").on('click', function (event) {
                var ev = event || window.event;
                var target = ev.target || ev.srcElement;
                if (target.nodeName.toLowerCase() == "a") {
                    var attrName = $(target).attr("name");
                    if (attrName == "del") {
                        var id = $(target).data("id");
                        var data = {"deptId": id};
                        commonService.del(data, $(target));
                    }
                }
            });
        });
    });
});