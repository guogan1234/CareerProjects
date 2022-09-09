var statusArr = getLevel();
var statuId;

var timeSpanArr = getTimeSpans();    //时间周期
var timeSpanId = timeSpanArr[0].id;

var startTimeDate;
var endTime;
var stationMarkerList = new Array();

var map = null;

map = new BMap.Map("baiduMap");
map.centerAndZoom("南京", 14);
map.enableScrollWheelZoom();

$(function () {
    $("#timeScope").datepicker("setDate", '-1y');
    initSearchTime();

});

function initSearchTime() {

    $("#status").select2({
        data: statusArr,
        placeholder: '全部',
        allowClear: true
    });

    $("#timeSpan").select2({
        data: timeSpanArr,
        placeholder: '请选择时间周期',
        allowClear: true
    });
    $("#timeSpan").val(timeSpanId).trigger('change');
}

function search() {

    for (var k = 0; k < stationMarkerList.length; k++) {
        stationMarkerList[k].remove();
    }

    startTimeDate = $("#timeScope").val();
    timeSpanId = $("#timeSpan").val();
    statuId = $("#status").val();

    var param = new Object();
    endTime = new Date(new Date().setMonth(new Date(startTimeDate).getMonth() + parseInt(timeSpanId))).pattern("yyyy/MM");  //年月相加
    param.endTime = endTime + "/01";
    param.startTime = startTimeDate + "/01";
    param.level = statuId;

    AJAX_GET('/mapDetail', param, function callback(data) {
        if (data.code > 0) {
            var dataList = data.result.statusList;
            for (var v = 0, length = dataList.length; v < length; v++) {
                if (dataList[v].longitude != null && dataList[v].latitude != null) {
                    initMap(dataList[v]);
                }
            }

        }
    });

    function initMap(obj) {

        var totalNum = obj.totalNum;     //故障数量
        var point = new BMap.Point(obj.longitude, obj.latitude);     //根据经纬度找车站

        var img = "";
        if (obj.status == 1) {
            img = "/cdai/marker-green.png";            //故障高于60000     故障
        } else if (obj.status == 0) {
            img = "/cdai/images/map3.png";             //故障低于60000     正常
        } else if (obj.status == 2) {
            img = "/cdai/images/marker-red.gif";            //故障最高     报警
        }

        //添加标注图片        路径，尺寸，图片中心
        var icon = new BMap.Icon(img, new BMap.Size(50, 50), {
            anchor: new BMap.Size(25, 25)
        });

        // 设置标注的经纬度
        var marker = new BMap.Marker(point, {icon: icon});
        map.addOverlay(marker);

        var opts = {
            width: 30,     // 信息窗口宽度
            height: 10,     // 信息窗口高度
            title: obj.stationName, // 信息窗口标题
            enableMessage: true,//设置允许信息窗发送短息
            message: ""
        };
        var message = '故障数量：' + totalNum + '个';
        var infoWindow = new BMap.InfoWindow(message, opts);  // 创建信息窗口对象
        marker.addEventListener("click", function () {
            map.openInfoWindow(infoWindow, point); //开启信息窗口
        });

        stationMarkerList.push(marker);
    }
}


