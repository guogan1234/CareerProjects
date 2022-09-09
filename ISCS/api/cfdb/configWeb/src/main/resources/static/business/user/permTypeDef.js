/**
 * Created by zhoujs on 2018/6/12.
 */
require(["../config/config"], function () {
    require(["commonService", "bootstrap", "validateHelp", "devSelect"], function (commonService, bootstrap, validateHelp, devSelect) {
        var options = {
            "formSelector": "permTypeDefFormModal",//表单选择器
            "modalSelector": "permTypeDefModal",//模态框选择器
            "action": "",//请求地址
            "showInfoSelector": "permTypeDefTbl",//显示消息选择器
            "searchSelector": "search",//查询选择器
            "tableSelector": "permTypeDefTbl",//表选择器
            "searchUrl": "/permType/list",//查询地址
            "saveUrl": "/permType/saveOrUpdate",//保存地址
            "updateUrl": "/permType/saveOrUpdate",//修改地址
            "detailUrl": "/permType/detail",//详情地址
            "delUrl": "/permType/del",//删除地址
            "saveSelector": "permTypeDefSaveModal",//保存按钮选择器
            "updateSelector": "permTypeDefUpdateModal",//更新按钮选择器
            "labelSelector": "permTypeDefModalLabel",//模态框t`itle选择器
            "autoAssignWhenAdd": true
        };

        var validateOptions = {
            "formSelector": "permTypeDefFormModal",//表单选择器
            "rules": ""
        };
        $(document).ready(function () {
            //初始化参数
            commonService.init(options);
            //初始化模态框
            //update时需要禁用的input
            var grayList = [];
            grayList.push("permTypeId");
            //加载需要的下拉框
            var types = [];
            // types.push("ZY");
            commonService.initModal(grayList, "", types);
            //初始化校验validate
            validateHelp.validateInit(validateOptions);
            //保存
            $("#permTypeDefSaveModal").click(function () {
                if (!validateHelp.valid()) {
                    return;
                }
                commonService.save();
            });
            //修改
            $("#permTypeDefUpdateModal").click(function () {
                if (!validateHelp.valid()) {
                    return;
                }
                commonService.update();
            });
            $("#searchPermTypeDef").click(function () {
                commonService.search();
            });
            //删除
            $("#permTypeDefTbl tbody").on('click', function (event) {
                var ev = event || window.event;
                var target = ev.target || ev.srcElement;
                if (target.nodeName.toLowerCase() == "a") {
                    var attrName = $(target).attr("name");
                    if (attrName == "del") {
                        var id = $(target).data("id");
                        var data = {"permTypeId": id};
                        commonService.del(data, $(target));
                    }
                }
            });
        });
    });
});