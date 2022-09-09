var doc = null;
var lineId = CONSTANTS.LINEID[0];
var svgStationSum = CONSTANTS.SVG.SVGSTATIONNUM[0];

$(function () {
    initStations();
});

function initStations() {

    AJAX_GET('/stationsByLineId/' + lineId, '', function callback(data) {
        if (data.code > 0) {
            var data = data.results;

            setTimeout(function () {    //定时刷新，防止svg图未加载完
                initStationsSVG(data);
            }, 1000);
        }
    });
}

function initStationsSVG(data) {
    doc = document.getElementById('stationssvg').getSVGDocument();

    var length = data.length;
    if (Number(length) >= svgStationSum) {  //最多展示svg图中总数
        length = svgStationSum;
    } else {
        setStationsHide(length);
    }

    var sendStations = [];
    var sendParam1 = new Object();
    sendParam1.type = 1;    //线路type：1
    sendParam1.ids = [lineId];
    sendStations.push(sendParam1);
    var sendParam2 = new Object();
    sendParam2.type = 2;    //车站type：2

    var stationIdArr = [];

    var stationSVGMap = {};
    for (var v = 0; v < length; v++) {
        stationSVGMap[data[v].stationId] = 'station' + (v + 1) + '-1';  //将车站id和svg图中id绑定

        stationIdArr.push(data[v].stationId);

        doc.getElementById('stationname' + (v + 1)).innerHTML = data[v].stationName;

        addTouch('station' + (v + 1), data[v].lineId, data[v].stationId);
        addTouch('text' + (v + 1), data[v].lineId, data[v].stationId);
    }

    var stationSVGJson = JSON.stringify(stationSVGMap);
    localStorage.setItem("stationSVGMap", stationSVGJson);

    sendParam2.ids = stationIdArr;
    sendStations.push(sendParam2);
    sendMessage(sendStations);
}

function setStationsHide(sum) {
    var row = CONSTANTS.SVG.SVGSTATIONNUM[1]; //行
    var col = CONSTANTS.SVG.SVGSTATIONNUM[2]; //列

    var num = parseInt((svgStationSum - sum) / col);    //需要隐藏整数行
    var ys = (svgStationSum - sum) % col;   //需要隐藏的余数列
    if (num > 0) {
        var svgid = doc.getElementById('stations');
        var svgheight = svgid.getAttribute('height');

        for (var v = row; v > 0; v--) {
            if (Number(v + num) == row) {
                break;
            } else {
                doc.getElementById('stationdiv' + v).style.display = "none";
            }
            svgStationSum -= col;
        }
        svgheight = Number(svgheight) - CONSTANTS.SVG.SVGSTATIONNUM[3] * num;   //根据隐藏行数决定显示高度
        svgid.setAttribute('height', svgheight);
    }
    if (ys > 0) {
        for (var v = 0; v < ys; v++) {  //隐藏余数个
            doc.getElementById('station' + svgStationSum).style.display = "none";
            doc.getElementById('text' + svgStationSum).style.display = "none";

            svgStationSum--;
        }
    }
}

function addTouch(obj, lineId, stationId) {
    var id = doc.getElementById(obj);
    if (id !== null) {
        id.addEventListener("touchend", function () {
            localStorage.setItem("devs-stationId", stationId);

            disconnect();
            window.location.href = '../../skip?path=device/alldevices';
        }, false);
    }
}

function setStationState(id, status) {
    doc.getElementById('text' + id.substring(7, id.length - 2)).style.fill = "black";   //设置字体颜色

    if (status == 0) {
        doc.getElementById(id).style.fill = "#00ff00";  //正常
        // } else if(status == 1){
        //     doc.getElementById(id).style.fill = "#777793";   //停止服务
        //
        //     doc.getElementById('text'+id.substring(7,id.length-2)).style.fill = "white"; //设置字体颜色
    } else if (status == 1) {
        doc.getElementById(id).style.fill = "#ffff00";  //警告
    } else if (status == 3) {
        doc.getElementById(id).style.fill = "#ff0000";  //报警
        // } else if(status == 4){
        //     doc.getElementById(id).style.fill = "#ffffff";   //离线
    }
}

