/**
 * Created by zhoujs on 2018/6/12.
 */
require(["../config/config"], function () {
    require(["commonService", "bootstrap", "validateHelp", "devSelect"], function (commonService, bootstrap, validateHelp, devSelect) {
        var options = {
            "formSelector": "permGroupDefFormModal",//表单选择器
            "modalSelector": "permGroupDefModal",//模态框选择器
            "action": "",//请求地址
            "showInfoSelector": "permGroupDefTbl",//显示消息选择器
            "searchSelector": "search",//查询选择器
            "tableSelector": "permGroupDefTbl",//表选择器
            "searchUrl": "/permGroup/list",//查询地址
            "saveUrl": "/permGroup/saveOrUpdate",//保存地址
            "updateUrl": "/permGroup/saveOrUpdate",//修改地址
            "detailUrl": "/permGroup/detail",//详情地址
            "delUrl": "/permGroup/del",//删除地址
            "saveSelector": "permGroupDefSaveModal",//保存按钮选择器
            "updateSelector": "permGroupDefUpdateModal",//更新按钮选择器
            "labelSelector": "permGroupDefModalLabel",//模态框t`itle选择器
            "autoAssignWhenAdd": true
        };

        var validateOptions = {
            "formSelector": "permGroupDefFormModal",//表单选择器
            "rules": ""
        };
        $(document).ready(function () {
            //初始化参数
            commonService.init(options);
            //初始化模态框
            //update时需要禁用的input
            var grayList = [];
            grayList.push("groupId");
            //加载需要的下拉框
            var types = [];
            // types.push("ZY");
            commonService.initModal(grayList, "", types);
            //初始化校验validate
            validateHelp.validateInit(validateOptions);
            //保存
            $("#permGroupDefSaveModal").click(function () {
                if (!validateHelp.valid()) {
                    return;
                }
                commonService.save();
            });
            //修改
            $("#permGroupDefUpdateModal").click(function () {
                if (!validateHelp.valid()) {
                    return;
                }
                commonService.update();
            });
            $("#searchPermGroupDef").click(function () {
                commonService.search();
            });
            //删除
            $("#permGroupDefTbl tbody").on('click', function (event) {
                var ev = event || window.event;
                var target = ev.target || ev.srcElement;
                if (target.nodeName.toLowerCase() == "a") {
                    var attrName = $(target).attr("name");
                    if (attrName == "del") {
                        var id = $(target).data("id");
                        var data = {"groupId": id};
                        commonService.del(data, $(target));
                    }
                }
            });
        });
    });
});