/**
 * bootStrap modal 帮助类
 */
define(['jquery', 'devSelect', "jquery.validate"], function ($, devSelect) {
    var forbidAndClearModal = function (selector, modalSelector, index, b) {
        if (!b) {
            selector.find("input[type='text']").each(function () {
                var sdv = $(this).data('sdv');
                $(this).val(sdv);
                //console.log($(this).attr("name") + ":" + $(this).val());
                $(this).prop("readOnly", true);
            });
            selector.find("select").each(function () {
                var sdv = $(this).data('sdv');
                $(this).val(sdv + "").select2();
                $(this).prop("disabled", true);
            });
        }
    };
    var clearModal = function (modalSelector, wizard) {
        modalSelector.find("input[type='number']").each(function () {
            var sdv = $(this).data('sdv');
            $(this).val(sdv);
            $(this).prop("disabled", false);
        });
        modalSelector.find("input[type='text']").each(function () {
            var sdv = $(this).data('sdv');
            $(this).val(sdv);
            $(this).prop("readOnly", false);
        });
        modalSelector.find("select").each(function () {
            var sdv = $(this).data('sdv');
            var selectName = $(this).prop("name");
            if (typeof(selectName) != "undefined" && selectName.indexOf("_helper") != -1) {

            } else {
                $(this).val(sdv + "").select2();
            }
            $(this).prop("disabled", false);
        });
        modalSelector.find("div").each(function () {
            if ($(this).css("display") == "none") {
                $(this).css("display", "");
            }
        });
        modalSelector.find("label[class='error']").each(function () {
            $(this).remove();
        });

        modalSelector.find(".error").each(function () {
            $(this).removeClass("error");
        });
        if (wizard != null && wizard != "" && wizard != undefined) {
            //初始化wizard
            $('#' + wizard).bootstrapWizard('show', 0);
            $('#' + wizard).find('.form-wizard').children('li').each(function () {
                $(this).removeClass('complete');
                $(this).removeClass('active');
                $(this).find(".fa-check").remove();
            });
            $('#' + wizard).find('.form-wizard').children('li').eq(0).addClass('active');
        }
    };

    var forbidModal = function (modalSelector) {
        modalSelector.find("input[type='text']").each(function () {
            $(this).prop("readOnly", true);
        });
        modalSelector.find("select").each(function () {
            $(this).prop("disabled", true);
        });
        modalSelector.find("input[type='number']").each(function () {
            $(this).prop("disabled", true);
        });
    };

    var modalInit = function (subId, types, modalselector, wizard) {
        clearModal(modalselector, wizard);
        for (var i = 0; i < types.length; i++) {
            var type = types[i];
            if (type == "ZY") {//专业
                devSelect.selectAssignment(null, "ZY", modalselector.find("[name='proSystemId']"));
            } else if (type == "DT") {//设备类型
                if (stringUtils.isNotBlank(subId)) {
                    devSelect.selectAssignment(subId, "DT", modalselector.find("[name='devTypeLabel']"));
                }
            } else if (type == "DTL") {
                if (stringUtils.isNotBlank(subId)) {
                    devSelect.selectAssignment(subId, "DTL", modalselector.find("[name='diTextLabel']"));
                }
            } else if (type == "AL") {  //报警等级
                devSelect.selectAssignment(null, "AL", modalselector.find("[name='alarmLevel']"));
            } else if (type == "AT") {  //报警类型
                devSelect.selectAssignment(null, "AT", modalselector.find("[name='alarmType']"));
            } else if (type == "AC") {   //报警动作
                devSelect.selectAssignment(null, "AC", modalselector.find("[name='alarmAct']"));
            } else if (type == "FM") {
                devSelect.selectAssignment(null, "FM", modalselector.find("[name='formulaTpLabel']"));
            } else if (type == "DG") {
                if (stringUtils.isNotBlank(subId)) {
                    devSelect.selectAssignment(subId, "DG", modalselector.find("[name='ctrlGrpLabel']"));
                }
            } else if (type == "DM") {//域
                devSelect.selectAssignment(null, "DM", modalselector.find("[name='domainId']"));
                devSelect.selectAssignment(null, "DM", modalselector.find("[name='sourceDomainId']"));
            } else if (type == "RG") {//责任区
                // if (stringUtils.isNotBlank(subId)) {
                devSelect.selectAssignment(subId, "RG", modalselector.find("[name='regionId']"));
                // }
            } else if (type == "ST") {//  车站
                devSelect.selectAssignment(subId, "ST", modalselector.find("[name='stationId']"));

            } else if (type == "DW") {//单位
                devSelect.selectAssignment(null, "DW", modalselector.find("[name='unitId']"));
            } else if (type == "PT") {//权限类型
                devSelect.selectAssignment(null, "PT", modalselector.find("[name='permTypeId']"));
            } else if (type == "DEP") {//部门
                devSelect.selectAssignment(null, "DEP", modalselector.find("[name='deptId']"));
            } else if (type == "USER") {//用户
                devSelect.selectAssignment(null, "USER", modalselector.find("[name='userId']"));
            } else if (type == "GROUP") {//用户组
                devSelect.selectAssignment(null, "GROUP", modalselector.find("[name='groupId']"));
            } else if (type=="CF"){//普通权限
                devSelect.selectAssignment(null, "CF", modalselector.find("[name='permId']"));
            }else if (type=="SF"){//特殊权限
                devSelect.selectAssignment(null, "SF", modalselector.find("[name='permId']"));
            }
        }
    };
    return {
        "modalInit": modalInit,
        "forbidModal": forbidModal,
        "clearModal": clearModal,
        "forbidAndClearModal": forbidAndClearModal
    }
});

