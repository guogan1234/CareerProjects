/**
 *  帮助常量
 */
var CONSTANTS = {
    LINEID: [4, 8],    //[十号线, X号线]  十号线:仅供设备监控车站集合页面使用;    X号线:仅供线路客流使用
    DIRECT: [0, 1], //[出站, 进站]
    BASE_URL:"http://localhost:4060",
    SVG: {
        SVGSTATIONNUM: [30, 6, 5, 31.51],  //X号线车站SVG图  [车站总数，行数，列数, 行高]
        SVGGATNUM: [48, 4, 12, 99.57],   //闸机SVG图   [闸机总数，行数，列数，行高]
        SVGTVMNUM: [32, 4, 8, 80],   //自动售票机SVG图    [自动售票机总数，行数，列数，行高]
        SVGBOMNUM: [4]   //客服中心SVG图 [客服中心总数]
    },
    DEVTAGS: {
        GAT: ['SER', 'CS2', 'ECU', 'PSU', 'EOD', 'CPU', 'PLC', 'MEM', 'CTN', 'ETH', 'CS1', 'DSK'],
        TVM: ['SER', 'CHS', 'ECU', 'PSU', 'EOD', 'BCG', 'TIM', 'CPU', 'CS1', 'MEM', 'CS2', 'ETH', 'BNA', 'DSK'],
        AVM: ['SER', 'CHS', 'ECU', 'PSU', 'EOD', 'BCG', 'AIM', 'CPU', 'CS1', 'MEM', 'CS2', 'ETH', 'BNA', 'DSK'],
        BOM: ['CPU', 'ETH', 'MEM', 'DSK']
    },
    PAGE: [1, 10, 7, 4] //[第一页，故障页面10条，能耗页面一周7条,故障因子页面4条]


    // AJAX: {
    //     TO_KEN: sessionStorage.getItem('XSRF-TOKEN'),
    //     TOKEN_TYPE: null,
    //     // ENABLED: true,
    //     JSAPI_TICKET: null,
    //     ACCESS_TOKEN: null
    // },
    // URL: {
    //     ACCESS_TOKEN: 'https://qyapi.weixin.qq.com/cgi-bin/gettoken'
    // },
    // CONF: {
    //     //本地
    //     // APP_SECRET: 'fLg_YT6auVuQHJoDfrAw0f672KEq5ooltHbTcPAnuOw',
    //     // CORP_ID: 'ww958c5f9d94ed052c',
    //     //线上
    //     CORP_ID: 'ww459483b92c637742',
    //     APP_SECRET: 'c7RXcFkHovcdEJO_Noy67Emj3PfwNA3gVOvH_OaxnRY',
    //     AGENT_ID: '1000071',
    //     LOGIN_URL: 'http://www.ac-mems.com:9999/sso/oauth/token', /* 登录URL */
    //     RESOURCE_API: 'http://192.168.1.110:4065' /* API  */
    // },
    // BASE: {
    //     OPEN_ID: "1231",
    //     PROJECT_ID: 6,
    //     LOGIN_NAME: 'admin',
    //     LOGIN_PASSWORD: 'avpAdmin',
    //     USER_INFO: JSON.parse(sessionStorage.getItem('USER_INFO')),
    //     USER: JSON.parse(sessionStorage.getItem('USER')),
    //     USERPW: '123456'
    // },
    // MASSAGE: {
    //     LOGIN: {
    //         LOGIN_ERROR: '登录失败',
    //         LOGIN_ERROR_BTN: '朕已阅'
    //     },
    //     RESULTM: {
    //         RESULT_NOTN: '暂无数据',
    //         RESULT_NOTN_BTN: '确定'
    //     },
    //     WORK: {
    //         REPAIRS_SUCCESS: '报修成功'
    //     }
    // }
};