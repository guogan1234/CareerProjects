define(['ajax', 'form', 'art-template', 'modalHelp', "timeUtils", "dualListHelp"], function (ajax, form, template, modalHelp, timeUtils, dualListHelp) {
    //配置
    var options = {
        "formSelector": "",//表单选择器
        "modalSelector": "",//模态框选择器
        "action": "",//请求地址
        "showInfoSelector": "",//显示消息选择器
        "searchSelector": "",//查询选择器
        "tableSelector": "",
        "searchUrl": "",
        "saveUrl": "",//保存地址
        "updateUrl": "",//修改地址
        "detailUrl": "",//详情地址
        "delUrl": "",//删除地址
        "saveSelector": "",//保存按钮选择器
        "updateSelector": "",//更新按钮选择器
        "labelSelector": "",//模态框title选择器
        "autoAssignWhenAdd": "",
        "autoHideSelectors": "",
        "controlNumSelector": ""
    };
    var dualOptions = {
        "modalSelectSelector": "",
        "selectedLabel": "",
        "nonSelectedLabel": "",
        "modalSelector": ""
    };

    var init = function (option, dualOption) {
        options.formSelector = option.formSelector;
        options.modalSelector = option.modalSelector;
        options.showInfoSelector = option.showInfoSelector;
        options.searchSelector = option.searchSelector;
        options.tableSelector = option.tableSelector;
        options.searchUrl = option.searchUrl;
        options.saveSelector = option.saveSelector;
        options.updateSelector = option.updateSelector;
        options.labelSelector = option.labelSelector;
        options.saveUrl = option.saveUrl;
        options.updateUrl = option.updateUrl;
        options.detailUrl = option.detailUrl;
        options.delUrl = option.delUrl;
        options.autoAssignWhenAdd = option.autoAssignWhenAdd;
        options.autoHideSelectors = option.autoHideSelectors;
        options.controlNumSelector = option.controlNumSelector;
        if (dualOption != null && dualOption != "" && dualOption.modalSelectSelector != "") {
            dualOptions.modalSelectSelector = dualOption.modalSelectSelector;
            dualOptions.selectedLabel = dualOption.selectedLabel;
            dualOptions.nonSelectedLabel = dualOption.nonSelectedLabel;
        }
    };
    var initAction = function (action) {
        options.action = action;
    };

    var initModal = function (grayList, subId, types) {
        $("#" + options.modalSelector).on('show.bs.modal', function (event) {
                $.fn.modal.Constructor.prototype.enforceFocus = function () {
                };
                var b = $(event.relatedTarget);
                var modal = $(this);
                var operate = b.data("operate");
                modalHelp.modalInit(subId, types, $("#" + options.formSelector));
                if (dualOptions.modalSelectSelector != "") {
                    if ($("#hid_init_dual").val() == "") {
                        dualListHelp.dualInit(dualOptions);
                        $("#hid_init_dual").val(1);
                    } else {
                        $(dualOptions.modalSelectSelector).bootstrapDualListbox("refresh");
                        $(dualOptions.modalSelectSelector).next(".select2-container").hide();
                    }
                }
                if (operate == "add") {//新增操作
                    $("#" + options.saveSelector).show();
                    $("#" + options.updateSelector).hide();
                    $("#" + options.labelSelector).html("新增");

                    if (options.autoAssignWhenAdd) {
                        $("#" + options.searchSelector).find("input").each(function () {
                            var k = $(this).prop("id");
                            var value = $(this).val();
                            $("#" + options.modalSelector).find("[name='" + k + "']").eq(0).val(value);
                        });

                        $("#" + options.searchSelector).find("select").each(function () {
                            var k = $(this).prop("id");
                            var value = $(this).val();
                            $("#" + options.modalSelector).find("[name='" + k + "']").eq(0).val(value).select2();
                        });

                    }
                }
                else if (operate == "update") {
                    var id = b.data("id") + "";
                    var ids = id.split(",");
                    var idp = b.data("idp") + "";
                    var idps = idp.split(",");
                    var data = {};
                    for (var i = 0; i < ids.length; i++) {
                        data[idps[i]] = ids[i];
                    }
                    detail(data);
                    if (grayList != null && grayList.length > 0) {
                        for (var i = 0; i < grayList.length; i++) {
                            modal.find("[name='" + grayList[i] + "']").prop("readOnly", true);
                        }
                    }
                    if (stringUtils.isNotBlank(options.controlNumSelector)) {
                        var num = modal.find("[name='" + options.controlNumSelector + "']").val();
                        var hideSelectors = options.autoHideSelectors.split(",");
                        for (var i = 0; i < hideSelectors.length; i++) {
                            modal.find("input[name='" + hideSelectors[i] + "']").parent().parent().hide();
                        }
                        for (var j = 0; j < hideSelectors.length; j++) {
                            for (var k = 1; k <= Number(num); k++) {
                                if (hideSelectors[j].indexOf(k) != -1) {
                                    modal.find("input[name='" + hideSelectors[j] + "']").parent().parent().show();
                                }
                            }
                        }
                    }
                    $("#" + options.saveSelector).hide();
                    $("#" + options.updateSelector).show();
                    $("#" + options.labelSelector).html("修改")
                } else if (operate == "detail") {
                    var id = b.data("id") + "";
                    var ids = id.split(",");
                    var idp = b.data("idp") + "";
                    var idps = idp.split(",");
                    var data = {};
                    for (var i = 0; i < ids.length; i++) {
                        data[idps[i]] = ids[i];
                    }
                    detail(data);
                    modalHelp.forbidModal($("#" + options.formSelector));
                    $("#" + options.labelSelector).html("查看");
                    $("#" + options.saveSelector).hide();
                    $("#" + options.updateSelector).hide();
                }
            }
        );
    };

    //保存
    var save = function (d) {
        var data = ""
        if (d) {
            data = d;
        } else {
            data = form.formInit($("#" + options.formSelector));
            $("#" + options.formSelector).find("[data-othername]").each(function () {
                data[$(this).data("othername")] = data[$(this).attr("name")];
            });
        }
        ajax.ajaxInit(options.saveUrl, data, true);
        ajax.post(saveCal, "", options.showInfoSelector);
    };
    var saveCal = function () {
        search();
        $("#" + options.modalSelector).modal('hide');
    };

    var saveB = function (modal, url, data) {
        //  var data=form.formInit($("#"+options.formSelector));
        ajax.ajaxInit(url, data, true);
        options.modalSelector = modal;
        ajax.post(saveBCal, "", options.showInfoSelector);
    };

    var saveBCal = function () {
        search();
        $("#" + options.modalSelector).modal('hide');
    }
    //修改
    var update = function (d) {
        var data = "";
        if (d) {
            d.operate = "update";
            data = d;
        } else {
            data = form.formInit($("#" + options.formSelector));
            $("#" + options.formSelector).find("[data-othername]").each(function () {
                data[$(this).data("othername")] = data[$(this).attr("name")];
            });
            data.operate = "update";
        }
        ajax.ajaxInit(options.updateUrl, data, true);
        ajax.post(updateCal, "", options.showInfoSelector);
    };
        var updateCal = function () {
            search();
            $("#" + options.modalSelector).modal('hide');
        };
        //详情
        var detail = function (condition) {
            var data = condition;
            ajax.ajaxInit(options.detailUrl, data, false);
            ajax.post(detailCal, "", options.showInfoSelector);
        };
        var detailCal = function (d) {
            form.setForm($("#" + options.formSelector), d);
        };
        //查询
        var search = function () {
            var data = {};
            $("#" + options.searchSelector).find("input").each(function () {
                var k = $(this).prop("id");
                var value = $(this).val();
                data[k] = value;
            });
            $("#" + options.searchSelector).find("select").each(function () {
                var k = $(this).prop("id");
                var value = $(this).val();
                data[k] = value;
            });
            ajax.ajaxInit(options.searchUrl, data, true);
            ajax.post(searchCal, "", options.showInfoSelector);
        };
        var searchCal = function (en) {
            var data = {};
            data.list = en;
            var html = template('list', data);
            $("#" + options.tableSelector + " tbody").html(html);
        };
        //删除
        var del = function (condition, thi) {
            if (confirm("确定要删除吗？")) {
                var data = condition;
                ajax.ajaxInit(options.delUrl, data, true);
                ajax.post(delCal, thi, options.showInfoSelector);
            }
        };
        var delCal = function (d) {
            $(d[0]).parent().parent().remove();
        };

        //通用AJAX与后台交互
        var commonM = function (d) {
            if (confirm("确定要这样操作吗？")) {
                var data = d;
                ajax.ajaxInit(options.action, data, true);
                ajax.post(commonMCal, "", options.showInfoSelector);
            }
        };

        var commonMCal = function () {
            search();
            $("#" + options.modalSelector).modal('hide');
        };

        return {
            "save": save,
            "saveB": saveB,
            "update": update,
            "detail": detail,
            "search": search,
            "del": del,
            "commonM": commonM,
            "init": init,
            "initAction": initAction,
            "initModal": initModal
        }
    }
    );