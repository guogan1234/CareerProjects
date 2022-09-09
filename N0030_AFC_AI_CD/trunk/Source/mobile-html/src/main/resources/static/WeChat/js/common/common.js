// function getYesterday() {
//     var day = new Date();
//     day.setDate(new Date().getDate() - 1);
//     var y = day.getFullYear();
//     var m = day.getMonth() + 1;
//     var d = day.getDate();
//
//     return y + '-' + m + '-' + d;
// }

// function getTomorrow() {
//     var day = new Date();
//     day.setDate(new Date().getDate() + 1);
//     var y = day.getFullYear();
//     var m = day.getMonth() + 1;
//     var d = day.getDate();
//     return y + '-' + m + '-' + d;
// }

function getNextweek() {
    var day = new Date();
    day.setDate(new Date().getDate() + 7);
    var y = day.getFullYear();
    var m = day.getMonth() + 1;
    var m = day.getMonth() + 1;
    var d = day.getDate();
    return y + '-' + m + '-' + d;
}

function getQueryString(name) { //获取url中的参数
    var reg = new RegExp("(^|&)" + name + "=([^&]*)(&|$)");
    var r = window.location.search.substr(1).match(reg);
    if (r != null) {
        return unescape(r[2]);
    }
    return null;
}

function getLines() {
    var linesArr = [];
    AJAX_GET('/lines', '', function callback(data) {
        if (data.code > 0) {
            var ls = data.results;
            for (var v = 0, length = ls.length; v < length; v++) {
                linesArr.push({value: ls[v].lineId, title: ls[v].lineName});
            }
        }
    });
    return linesArr;
}

function getStations() {
    var stationsArr = [];
    AJAX_GET('/stations', '', function callback(data) {
        if (data.code > 0) {
            var ls = data.results;
            for (var v = 0, length = ls.length; v < length; v++) {
                stationsArr.push({value: ls[v].stationId, title: ls[v].stationName});
            }
        }
    });
    return stationsArr;
}

function getStationsByLineId(lineId) {
    var stationsArr = [];
    AJAX_GET('/stationsByLineId/'+lineId, '', function callback(data) {
        if (data.code > 0) {
            var ls = data.results;
            for (var v = 0, length = ls.length; v < length; v++) {
                stationsArr.push({value: ls[v].stationId, title: ls[v].stationName});
            }
        }
    });
    return stationsArr;
}

function getEquipmentByStationId(stationId) {
    var equipmentArr = [];
    AJAX_GET('/findByStationId/'+stationId, '', function callback(data) {
        if (data.code > 0) {
            var ls = data.results;
            for (var v = 0, length = ls.length; v < length; v++) {
                equipmentArr.push({value: ls[v].equipmentId, title: ls[v].equipmentName});
            }
        }
    });
    return equipmentArr;
}
function getModules() {
    var modulesArr = [];
    AJAX_GET('/RefModules', '', function callback(data) {
        if (data.code > 0) {
            var ls = data.results;
            for (var v = 0, length = ls.length; v < length; v++) {
                modulesArr.push({value: ls[v].moduleId, title: ls[v].moduleName});
            }
        }
    });
    return modulesArr;
}

function getTimeSpans() {
    var timeSpanArr = [];
    timeSpanArr.push({value: 1, title: "1个月"});
    timeSpanArr.push({value: 3, title: "3个月"});
    timeSpanArr.push({value: 6, title: "6个月"});
    timeSpanArr.push({value: 12, title: "12个月"});
    return timeSpanArr;
}


Date.prototype.pattern = function (fmt) {
    var o = {
        "M+": this.getMonth() + 1, //月份
        "d+": this.getDate(), //日
        "h+": this.getHours() % 12 == 0 ? 12 : this.getHours() % 12, //小时
        "H+": this.getHours(), //小时
        "m+": this.getMinutes(), //分
        "s+": this.getSeconds(), //秒
        "q+": Math.floor((this.getMonth() + 3) / 3), //季度
        "S": this.getMilliseconds() //毫秒
    };
    var week = {
        "0": "/u65e5",
        "1": "/u4e00",
        "2": "/u4e8c",
        "3": "/u4e09",
        "4": "/u56db",
        "5": "/u4e94",
        "6": "/u516d"
    };
    if (/(y+)/.test(fmt)) {
        fmt = fmt.replace(RegExp.$1, (this.getFullYear() + "").substr(4 - RegExp.$1.length));
    }
    if (/(E+)/.test(fmt)) {
        fmt = fmt.replace(RegExp.$1, ((RegExp.$1.length > 1) ? (RegExp.$1.length > 2 ? "/u661f/u671f" : "/u5468") : "") + week[this.getDay() + ""]);
    }
    for (var k in o) {
        if (new RegExp("(" + k + ")").test(fmt)) {
            fmt = fmt.replace(RegExp.$1, (RegExp.$1.length == 1) ? (o[k]) : (("00" + o[k]).substr(("" + o[k]).length)));
        }
    }
    return fmt;
}

/**
 * 'YYYY-MM-DD HH:mm:ss'
 * @param time 时间
 * @param key   时间格式
 */
function dateToStr(time, key) {
    return moment(time).format(key);
}


// Cookies
function setCookie(sName, sValue, oExpires, sPath, sDomain, bSecure) {
    var sCookie = sName + "=" + encodeURIComponent(sValue);
    if (oExpires) {
        sCookie += "; expires=" + oExpires.toGMTString();
    }
    if (sPath) {
        sCookie += "; path=" + sPath;
    }
    if (sDomain) {
        sCookie += "; domain=" + sDomain;
    }
    if (bSecure) {
        sCookie += "; secure";
    }
    document.cookie = sCookie;
}

function getCookie(sName) {
    var sRE = "(?:; )?" + sName + "=([^;]*);?";
    var oRE = new RegExp(sRE);
    if (oRE.test(document.cookie)) {
        return decodeURIComponent(RegExp["$1"]);
    } else {
        return null;
    }
}

function deleteCookie(sName, sPath, sDomain) {
    var sCookie = sName + "=; expires=" + (new Date(0)).toGMTString();
    if (sPath) {
        sCookie += "; path=" + sPath;
    }
    if (sDomain) {
        sCookie += "; domain=" + sDomain;
    }
    document.cookie = sCookie;
}

function isNotEmpty(v) {
    if (null != v && undefined != v && "" != v.toString() && "null" != v.toString()) {
        return true;
    }
    return false;
}

/**
 * 自定义 MAP 性能不咋地。勿吐槽
 * put();                添加数据到MAP 中
 * get();                    使用Key 取值
 * keySet();                获取Key集合  Array 类型
 * size()                    获取MAP 的长度
 * remove()                    移除指定Key 对象
 */
/****************************************************** start MAP  **********************************************************/
function Map() {
    this.container = new Object();
}

Map.prototype.put = function (key, value) {
    this.container[key] = value;
}

Map.prototype.get = function (key) {
    return this.container[key];
}

Map.prototype.keySet = function () {
    var keyset = new Array();
    var count = 0;
    for (var key in this.container) {
// 跳过object的extend函数
        if (key == 'extend') {
            continue;
        }
        keyset[count] = key;
        count++;
    }
    return keyset;
}

Map.prototype.size = function () {
    var count = 0;
    for (var key in this.container) {
// 跳过object的extend函数
        if (key == 'extend') {
            continue;
        }
        count++;
    }
    return count;
}

Map.prototype.remove = function (key) {
    delete this.container[key];
}
/****************************************************** end MAP  **********************************************************/

