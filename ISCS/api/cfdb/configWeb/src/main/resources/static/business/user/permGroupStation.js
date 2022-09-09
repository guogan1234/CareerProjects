require(["../config/config"], function () {
    require(["commonService", "bootstrap", "validateHelp"], function (commonService, bootstrap, validateHelp) {
        $(document).ready(function () {
            var options = {
                "formSelector": "permGroupStationFormModal",//表单选择器
                "modalSelector": "permGroupStationModal",//模态框选择器
                "action": "",//请求地址
                "showInfoSelector": "permGroupStationTbl",//显示消息选择器
                "searchSelector": "search",//查询选择器
                "tableSelector": "permGroupStationTbl",//表选择器
                "searchUrl": "/permGroupStation/list",//查询地址
                "saveUrl": "/permGroupStation/saveOrUpdate",//保存地址
                "updateUrl": "/permGroupStation/saveOrUpdate",//修改地址
                "detailUrl": "/permGroupStation/detail",//详情地址
                "delUrl": "/permGroupStation/del",//删除地址
                "saveSelector": "permGroupStationSaveModal",//保存按钮选择器
                "updateSelector": "permGroupStationUpdateModal",//更新按钮选择器
                "labelSelector": "permGroupStationModalLabel",//模态框title选择器
                "autoAssignWhenAdd": false
            };

            var validateOptions = {
                "formSelector": "permGroupStationFormModal",//表单选择器
                "rules": ""
            };
            var dualOptions = {
                "modalSelectSelector": "#permGroupStationModal [name='stationId']",
                "selectedLabel": "已选择",
                "nonSelectedLabel": "未选择"
            };
            commonService.init(options, dualOptions);

            var grayList = [];
            //加载需要的下拉框
            var types = [];
            types.push("ST");
            types.push("GROUP");
            commonService.initModal(grayList, "", types);
            //初始化校验validate
            validateHelp.validateInit(validateOptions);
            $("#permGroupStationSaveModal").on("click", function () {
                if(!validateHelp.valid()){
                    return;
                }
                var stationIds = $("#permGroupStationModal [name='stationId']").val();
                var groupId = $("#permGroupStationModal [name='groupId']").val();
                var d = {
                    "stationIds": stationIds.join(","),
                    "groupId": groupId
                };
                commonService.save(d);
            });
            $("#searchPermGroupStation").click(function () {
                commonService.search();
            });
            //删除
            $("#permGroupStationTbl tbody").on('click', function (event) {
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