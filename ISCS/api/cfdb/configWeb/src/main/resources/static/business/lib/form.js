/**
 * form的操作，配合ajax使用
 */
define(['jquery', 'stringUtils', "select2"], function ($, stringUtils, select2) {
    /**
     * 得到表单的值
     * @param selector
     * @returns {{}}
     */
    var formInit = function (selector) {
        var serializeObj = {};
        $(selector.serializeArray()).each(function () {
            if (stringUtils.isNotBlank(this.value)) {
                // this.value=dealBool(this.value);
                serializeObj[this.name] = this.value;
            }
        });
        return serializeObj;
    };
    /**
     * 给form赋值
     * @param selector
     */
    var setForm = function (selector, list) {
        for (var item in list) {
            var t = selector.find("[name='" + item + "']");
            if (t.length == 0) {
                t = selector.find("[data-othername='" + item + "']");
            }
            if (t.length > 0) {
                if (t[0].nodeName == "SELECT") {
                    var selectV = list[item]+"";
                    if (selectV.indexOf(",") != -1) {
                        var selectArr = selectV.split(",");
                        t.val(selectArr).trigger('change');
                    } else {
                        t.val(selectV).select2();
                    }
                } else if (t[0].nodeName == "SPAN") {
                    t.html(list[item]);
                } else {
                    t.val(list[item]);
                }
            }
        }
    };
    /**
     * 处理bool
     * @param value
     * @returns {*}
     */
    var dealBool = function (value) {
        var b = value;
        if (value == "true") {
            b = true;
        } else if (value == "false") {
            b = false;
        }
        return b;
    }
    return {
        "formInit": formInit,
        "setForm": setForm
    }
});
