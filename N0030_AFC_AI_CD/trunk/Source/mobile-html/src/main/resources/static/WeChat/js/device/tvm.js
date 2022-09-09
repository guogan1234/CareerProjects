var devId = null;
var doc = null;

$(function () {
    initTVM();
});

function initTVM() {

    devId = getQueryString("deviceid");  //获取报警推送url中的参数
    if (devId == null) {
        devId = localStorage.getItem("devId");
    }
    var stationId = getQueryString("stationid");  //获取报警推送url中的参数
    if (stationId != null) {
        localStorage.setItem("devs-stationId", stationId);
    }

    AJAX_GET('/findByEquipmentId/' + devId, '', function callback(data) {
        if (data.code > 0) {
            var data = data.results;

            setTimeout(function () {
                initTVMSVG(data[0]);
            }, 1000);
        }
    });
}

function initTVMSVG(data) {
    doc = document.getElementById('tvmsvg').getSVGDocument();

    doc.getElementById('onetvmspan').innerHTML = '编号：' + data.equipmentName;

    var oneDevSVGMap = {};
    oneDevSVGMap['devid'] = data.equipmentId;
    var tagsArr = CONSTANTS.DEVTAGS.TVM;
    for (var v = 0, length = tagsArr.length; v < length; v++) {
        oneDevSVGMap[tagsArr[v]] = 'onetvm' + (v + 1) + '-1';
    }

    var oneDevSvgJson = JSON.stringify(oneDevSVGMap);
    localStorage.setItem("tagSVGMap", oneDevSvgJson);

    var sendOnedev = [];
    var sendParam = new Object();
    sendParam.type = 4; //tag标签type：4
    sendParam.ids = [data.equipmentId];
    sendOnedev.push(sendParam);
    sendMessage(sendOnedev);
}

function setTagStatus(id, status) {
    if (status == 0) {
        doc.getElementById(id).style.fill = "#00ff00";  //正常
    } else if (status == 1) {
        doc.getElementById(id).style.fill = "#ffff00";  //警告
    } else if (status == 3) {
        doc.getElementById(id).style.fill = "#ff0000";  //报警
    }
}
