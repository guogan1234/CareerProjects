require(["../config/config"], function () {
    require(["commonService", "bootstrap", "validateHelp"], function (commonService, bootstrap, validateHelp) {
        $(document).ready(function () {
            var options = {
                "formSelector": "permSpecGroupRegionRealationFormModal",//表单选择器
                "modalSelector": "permSpecGroupRegionRealationModal",//模态框选择器
                "action": "/permSpecGroupRegionRealation/changeLead",//请求地址
                "showInfoSelector": "permSpecGroupRegionRealationTbl",//显示消息选择器
                "searchSelector": "search",//查询选择器
                "tableSelector": "permSpecGroupRegionRealationTbl",//表选择器
                "searchUrl": "/permSpecGroupRegionRealation/list",//查询地址
                "saveUrl": "/permSpecGroupRegionRealation/saveOrUpdate",//保存地址
                "updateUrl": "/permSpecGroupRegionRealation/saveOrUpdate",//修改地址
                "detailUrl": "/permSpecGroupRegionRealation/detail",//详情地址
                "delUrl": "/permSpecGroupRegionRealation/del",//删除地址
                "saveSelector": "permSpecGroupRegionRealationSaveModal",//保存按钮选择器
                "updateSelector": "permSpecGroupRegionRealationUpdateModal",//更新按钮选择器
                "labelSelector": "permSpecGroupRegionRealationModalLabel",//模态框title选择器
                "autoAssignWhenAdd": false
            };
            var validateOptions = {
                "formSelector": "permSpecGroupRegionRealationFormModal",//表单选择器
                "rules": ""
            };

            var dualOptions = {
                "modalSelectSelector": "#permSpecGroupRegionRealationModal [name='regionId']",
                "selectedLabel": "已选择",
                "nonSelectedLabel": "未选择"
            };
            commonService.init(options, dualOptions);
            commonService.initAction(options.action);

            var grayList = [];
            //加载需要的下拉框
            var types = [];
            types.push("SF");
            types.push("RG");
            types.push("GROUP");
            commonService.initModal(grayList, "", types);
            //初始化校验validate
            validateHelp.validateInit(validateOptions);
            $("#permSpecGroupRegionRealationSaveModal").on("click", function () {
                if(!validateHelp.valid()){
                    return;
                }
                var specIds = $("#permSpecGroupRegionRealationModal [name='permId']").val();
                var groupId = $("#permSpecGroupRegionRealationModal [name='groupId']").val();
                var regionIds = $("#permSpecGroupRegionRealationModal [name='regionId']").val();
                var permDefine = $("#permSpecGroupRegionRealationModal [name='permDefine']").val();
                var d = {
                    "permIds": specIds.join(","),
                    "regionIds": regionIds.join(","),
                    "groupId": groupId,
                    "permDefine":permDefine
                };
                commonService.save(d);
            });
            $("#searchPermSpecGroupRegionRealation").click(function () {
                commonService.search();
            });
            //删除
            $("#permSpecGroupRegionRealationTbl tbody").on('click', function (event) {
                var ev = event || window.event;
                var target = ev.target || ev.srcElement;
                if (target.nodeName.toLowerCase() == "a") {
                    var attrName = $(target).attr("name");
                    if (attrName == "del") {
                        var id = $(target).data("id");
                        var data = {"relationId": id};
                        commonService.del(data, $(target));
                    }
                }
            });
        });
    });
});