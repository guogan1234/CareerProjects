/**
 * Created by fangxinyuan on 2015/12/3.
 * 时间处理
 */
var timeUtils=(function(){
     var timePattern="yyyy/MM/dd hh:mm:ss";
    //得到当前时间
    var getCurrentTime=function(){
        var date=new Date();

        var year=date.getFullYear();
        var month=date.getMonth()+1;
        var day=date.getDate();
        var hour=date.getHours();
        var minute=date.getMinutes();
        var second=date.getSeconds();

        var a1=new Array();
        var a2=new Array();

        a1.push(year);
        a1.push(month);
        a1.push(day);
        var a1Str=a1.join("-");
        a2.push(hour);
        a2.push(minute);
        a2.push(second);
        var a2Str=a2.join(":");
        return a1Str+" "+a2Str;
    };
    //得到当前时间的毫秒数
    var getCurrentMilliSeconds=function(){
         var date=new Date();
         return date.getMilliseconds();
    };
    //时间格式化
    var timeFormat=function(date,pattern){
        if(stringUtils.isBlank(date)){
            return null;
        }

        if(stringUtils.isBlank(pattern)){
            pattern=timePattern;
        }
        var date=strTimeToDate(date);
        return date.format(pattern);
    }
    
    var dateFormat=function(date,pattern){
    	if(stringUtils.isBlank(pattern)){
            pattern=timePattern;
        }
    	return date.format(pattern);
    }
    
    
    //string to date
    var strTimeToDate=function(date){
        if(stringUtils.isBlank(date)){
            return null;
        }
        var date=new Date(date.replace(/-/g, "/"));
        return date;
    }

    //时间转毫秒
    var timeToMilliSeconds=function(date){
        var m=0;
        if(typeof date =="undefined"){
            return m;
        }
        if(typeof date =="string"){//传入的是字符串类型
            if(stringUtils.isNotBlank(date)){
                var d=timeFormat(date,timePattern);
                if(stringUtils.isBlank(d)){
                    return m;
                }
                return strTimeToDate(d).getTime();
            }
        }else if(typeof date =="object"){
             return date.getTime();
        }
    }
    //毫秒转时间
    var milliSecondsToTime=function(milliSeconds,pattern){
        return timeFormat(milliSeconds,pattern);
    }
    //date 扩展format
    Date.prototype.format =function(format)
    {
        var o = {
            "M+" : this.getMonth()+1, //month
            "d+" : this.getDate(),    //day
            "h+" : this.getHours(),   //hour
            "m+" : this.getMinutes(), //minute
            "s+" : this.getSeconds(), //second
            "q+" : Math.floor((this.getMonth()+3)/3),  //quarter
            "S" : this.getMilliseconds() //millisecond
        }
        if(/(y+)/.test(format)) format=format.replace(RegExp.$1,
            (this.getFullYear()+"").substr(4- RegExp.$1.length));
        for(var k in o)if(new RegExp("("+ k +")").test(format))
            format = format.replace(RegExp.$1,
                RegExp.$1.length==1? o[k] :
                    ("00"+ o[k]).substr((""+ o[k]).length));
        return format;
    };

    return{
        getCurrentTime:getCurrentTime,
        getCurrentMilliSeconds:getCurrentMilliSeconds,
        timeFormat:timeFormat,
        strTimeToDate:strTimeToDate,
        timeToMilliSeconds:timeToMilliSeconds,
        milliSecondsToTime:milliSecondsToTime,
        dateFormat:dateFormat
    };




})();