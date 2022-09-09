var stationid = null;
var gatdoc = null;
var tvmdoc = null;
var bomdoc = null;
var svgGatSum = CONSTANTS.SVG.SVGGATNUM[0];
var svgTvmSum = CONSTANTS.SVG.SVGTVMNUM[0];
var svgBomSum = CONSTANTS.SVG.SVGBOMNUM[0];

$(function () {
    initDevices();
});

function initDevices() {

    stationid = getQueryString("stationid");  //获取报警推送url中的参数
    if (stationid == null) {
        stationid = localStorage.getItem("devs-stationId");
    }

    AJAX_GET('/stationById/' + stationid, '', function callback(data) {
        if (data.code > 0) {
            var data = data.result.equipmentMap;

            setTimeout(function () {
                initDevicesSVG(data);
            }, 1000);
        }
    });
}

function initDevicesSVG(data) {
    var sendDevices = [];

    var devIdArr = [];

    var devSVGMap = {};
    if (data.GAT != undefined) {
        gatdoc = document.getElementById('alldevices_gatsvg').getSVGDocument();

        var length = data.GAT.length;
        if (length >= svgGatSum) {
            length = svgGatSum;
        } else {
            setDevicesHide(gatdoc, 'gat', length);
        }

        var gatMap = new Map();
        gatMap = data.GAT;
        for (var v = 0; v < length; v++) {
            devIdArr.push(gatMap[v].equipmentId);
            devSVGMap[gatMap[v].equipmentId] = 'gat' + (v + 1) + '-3';  //将闸机id和svg图中id绑定

            gatdoc.getElementById('tspan' + (v + 1) + '-1').innerHTML = gatMap[v].equipmentName;

            if (gatMap[v].subTypeId == 2) {   //进站闸机
                gatdoc.getElementById('gat' + (v + 1) + '-1').style.fill = "#00ff00";   //正常
                gatdoc.getElementById('gat' + (v + 1) + '-2').style.fill = "#777793";   //停止服务

            } else if (gatMap[v].subTypeId == 19) {   //出站闸机
                gatdoc.getElementById('gat' + (v + 1) + '-1').style.fill = "#777793";   //停止服务
                gatdoc.getElementById('gat' + (v + 1) + '-2').style.fill = "#00ff00";   //正常

            } else if (gatMap[v].subTypeId == 20) {   //双向闸机
                gatdoc.getElementById('gat' + (v + 1) + '-1').style.fill = "#00ff00";   //正常
                gatdoc.getElementById('gat' + (v + 1) + '-2').style.fill = "#00ff00";   //正常

            }
            addTouch(gatdoc, 'gat', v + 1, gatMap[v].equipmentId);
        }
    }
    if (data.TVM != undefined) {
        tvmdoc = document.getElementById('alldevices_tvmsvg').getSVGDocument();

        var length = data.TVM.length;
        if (length >= svgTvmSum) {
            length = svgTvmSum;
        } else {
            setDevicesHide(tvmdoc, 'tvm', length);
        }
        var tvmMap = new Map();
        tvmMap = data.TVM;
        for (var v = 0; v < length; v++) {
            devIdArr.push(tvmMap[v].equipmentId);
            devSVGMap[tvmMap[v].equipmentId] = 'tvm' + (v + 1) + '-1';  //将自动售票机id和svg图中id绑定

            tvmdoc.getElementById('tvmspan' + (v + 1) + '-1').innerHTML = tvmMap[v].equipmentName;

            addTouch(tvmdoc, 'tvm', v + 1, tvmMap[v].equipmentId);
        }
    }
    if (data.BOM != undefined) {
        bomdoc = document.getElementById('alldevices_bomsvg').getSVGDocument();

        var length = data.GAT.length;
        if (length >= svgBomSum) {
            length = svgBomSum;
        } else {
            setDevicesHide(bomdoc, 'bom', length);
        }
        var bomMap = new Map();
        bomMap = data.BOM;
        for (var v = 0; v < length; v++) {
            devIdArr.push(bomMap[v].equipmentId);
            devSVGMap[bomMap[v].equipmentId] = 'bom' + (v + 1) + '-1';

            bomdoc.getElementById('bomspan' + (v + 1) + '-1').innerHTML = bomMap[v].equipmentName;

            addTouch(bomdoc, 'bom', v + 1, bomMap[v].equipmentId);
        }
    }

    var json1 = JSON.stringify(devSVGMap);
    localStorage.setItem("devSVGMap", json1);

    var obj = new Object();
    obj.type = 3;
    obj.ids = devIdArr;
    sendDevices.push(obj);
    sendMessage(sendDevices);
}

function addTouch(doc, obj, v, devId) {
    var id = doc.getElementById(obj + v);
    if (id !== null) {
        id.addEventListener("touchend", function () {
            localStorage.setItem("devId", devId);

            disconnect();
            window.location.href = '../../skip?path=device/' + obj;
        }, false);
    }
}

function setDevicesHide(doc, obj, sum) {
    var svgid = doc.getElementById(obj + 's');
    var svgheight = svgid.getAttribute('height');
    if (obj === 'gat') {
        if (sum < svgGatSum) {
            var row = CONSTANTS.SVG.SVGGATNUM[1];   //行
            var col = CONSTANTS.SVG.SVGGATNUM[2];   //列

            var num = parseInt((svgGatSum - sum) / col); //要隐藏的行数
            var ys = (svgGatSum - sum) % col;   //要隐藏的不足一行的个数
            if (row > num > 0) {
                for (var v = row; v > 0; v--) {
                    if (Number(v + num) == row) {
                        break;
                    } else {
                        doc.getElementById(obj + '-' + v).style.display = "none";
                    }
                    svgGatSum -= col;
                }
                svgheight = Number(svgheight) - CONSTANTS.SVG.SVGGATNUM[3] * num;   //根据隐藏行数决定显示高度
                svgid.setAttribute('height', svgheight);

                document.getElementsByClassName('iframe-svg2')[0].style.marginTop = -CONSTANTS.SVG.SVGGATNUM[3] * num + 'px';
            }
            if (ys > 0) {  //隐藏余数个
                for (var v = 0; v < ys; v++) {
                    doc.getElementById(obj + svgGatSum).style.display = "none";
                    doc.getElementById('tspan' + svgGatSum).style.display = "none";

                    svgGatSum--;
                }
            }
        }
    } else if (obj === 'tvm') {
        if (sum < svgTvmSum) {
            var row = CONSTANTS.SVG.SVGTVMNUM[1];   //行
            var col = CONSTANTS.SVG.SVGTVMNUM[2];   //列

            var num = parseInt((svgTvmSum - sum) / col); //要隐藏的行数
            var ys = (svgTvmSum - sum) % col;   //要隐藏的不足一行的个数
            if (row > num > 0) {
                for (var v = row; v > num; v--) {
                    if (Number(v + num) == row) {
                        break;
                    } else {
                        doc.getElementById(obj + '-' + v).style.display = "none";
                    }
                    svgTvmSum -= col;
                }
                svgheight = Number(svgheight) - CONSTANTS.SVG.SVGTVMNUM[3] * num;   //根据隐藏行数决定显示高度
                svgid.setAttribute('height', svgheight);
                document.getElementsByClassName('iframe-svg3')[0].style.marginTop = -CONSTANTS.SVG.SVGTVMNUM[3] * num + 'px';
            }
            if (ys > 0) {  //隐藏余数个
                for (var v = 0; v < ys; v++) {
                    doc.getElementById(obj + svgTvmSum).style.display = "none";
                    doc.getElementById(obj + 'span' + svgTvmSum).style.display = "none";

                    svgTvmSum--;
                }
            }
        }
    } else if (obj === 'bom') {
        if (sum < svgBomSum) {
            var num = svgBomSum - sum;
            if (num < svgBomSum) {
                for (var v = svgBomSum; v > sum; v--) {
                    doc.getElementById(obj + v).style.display = "none";
                    doc.getElementById(obj + 'span' + v).style.display = "none";
                }
            }
        }
    }
}

function setDevStatus(id, status) {
    var doc = null;
    if (id.indexOf('gat') >= 0) {
        doc = gatdoc;
    } else if (id.indexOf('tvm') >= 0) {
        doc = tvmdoc;
    } else if (id.indexOf('bom') >= 0) {
        doc = bomdoc;
    }

    if (status == 0) {
        doc.getElementById(id).style.fill = "#00ff00";  //正常
    } else if (status == 1) {
        doc.getElementById(id).style.fill = "#ffff00";  //警告
    } else if (status == 3) {
        doc.getElementById(id).style.fill = "#ff0000";  //报警
    } else if (status == 4) {
        doc.getElementById(id).style.fill = "#ff0000";  //报警
    } else if (status == 5) {
        doc.getElementById(id).style.fill = "#ffffff";  //离线
    }
}


