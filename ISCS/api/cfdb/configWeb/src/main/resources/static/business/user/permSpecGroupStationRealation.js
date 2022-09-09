require(["../config/config"], function () {
    require(["commonService", "bootstrap", "validateHelp"], function (commonService, bootstrap, validateHelp) {
        $(document).ready(function () {
            var options = {
                "formSelector": "permSpecGroupStationRealationFormModal",//表单选择器
                "modalSelector": "permSpecGroupStationRealationModal",//模态框选择器
                "action": "",//请求地址
                "showInfoSelector": "permSpecGroupStationRealationTbl",//显示消息选择器
                "searchSelector": "search",//查询选择器
                "tableSelector": "permSpecGroupStationRealationTbl",//表选择器
                "searchUrl": "/permSpecGroupStationRealation/list",//查询地址
                "saveUrl": "/permSpecGroupStationRealation/saveOrUpdate",//保存地址
                "updateUrl": "/permSpecGroupStationRealation/saveOrUpdate",//修改地址
                "detailUrl": "/permSpecGroupStationRealation/detail",//详情地址
                "delUrl": "/permSpecGroupStationRealation/del",//删除地址
                "saveSelector": "permSpecGroupStationRealationSaveModal",//保存按钮选择器
                "updateSelector": "permSpecGroupStationRealationUpdateModal",//更新按钮选择器
                "labelSelector": "permSpecGroupStationRealationModalLabel",//模态框title选择器
                "autoAssignWhenAdd": false
            };
            var validateOptions = {
                "formSelector": "permSpecGroupStationRealationFormModal",//表单选择器
                "rules": ""
            };

            var dualOptions = {
                "modalSelectSelector": "#permSpecGroupStationRealationModal [name='stationId']",
                "selectedLabel": "已选择",
                "nonSelectedLabel": "未选择"
            };
            commonService.init(options, dualOptions);

            var grayList = [];
            //加载需要的下拉框
            var types = [];
            types.push("ST");
            types.push("SF");
            types.push("GROUP");
            commonService.initModal(grayList, "", types);
            //初始化校验validate
            validateHelp.validateInit(validateOptions);
            $("#permSpecGroupStationRealationSaveModal").on("click", function () {
                if(!validateHelp.valid()){
                    return false;
                }
                var permIds = $("#permSpecGroupStationRealationModal [name='permId']").val();
                var groupId = $("#permSpecGroupStationRealationModal [name='groupId']").val();
                var stationIds = $("#permSpecGroupStationRealationModal [name='stationId']").val();
                var permDefine = $("#permSpecGroupStationRealationModal [name='permDefine']").val();
                var d = {
                    "stationIds": stationIds.join(","),
                    "permIds": permIds.join(","),
                    "groupId": groupId,
                    "permDefine": permDefine
                };
                commonService.save(d);
            });
            $("#searchPermSpecGroupStationRealation").click(function () {
                commonService.search();
            });
            //删除
            $("#permSpecGroupStationRealationTbl tbody").on('click', function (event) {
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