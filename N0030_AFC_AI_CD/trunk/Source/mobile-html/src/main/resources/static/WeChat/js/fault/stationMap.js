var map = null;

$(function () {
    //创建Map实例
    map = new BMap.Map("baiduMap");
    map.centerAndZoom("南京", 14);
    //添加鼠标滚动缩放
    map.enableScrollWheelZoom();

    initStations();
});

function initStations() {
    AJAX_GET('/mapDetail', '', function callback(data) {
        if (data.code > 0) {
            var dataList = data.result.statusList;
            for (var v = 0, length = dataList.length; v < length; v++) {
                if (dataList[v].longitude != null && dataList[v].latitude != null) {
                    initMap(dataList[v]);
                }
            }

        }
    });
}

function initMap(obj) {

    var totalNum = obj.totalNum;
    var point = new BMap.Point(obj.longitude, obj.latitude);

    var img = "";
    if (obj.status == 0) {
        img = "/WeChat/image/marker-green.png";
    } else if (obj.status == 1) {
        img = "/WeChat/image/marker-yellow.gif";
    } else if (obj.status == 2) {
        img = "/WeChat/image/marker-red.gif";
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
}

