/**
 * 对ajax的封装,只正对当前项目业务
 */
define(['jquery','stringUtils','popUp','jsonStrUtils'],function ($,stringUtils,popUp,jsonStrUtils) {
    var callback=$.Callbacks();//定义回调函数列表
    var url="";
    var json="";
    var async=true;
    var ajaxInit=function (url,json,async){
      this.url=url;
      this.json=json;
      this.async=async;
    };
    var post=function (c,thi,selector,type,inherit) {
        $.ajax({
            type:"post",
            url:this.url,
            data:this.json,
            async:this.async,
            success:function (data) {
                popUp.close();
                if(data.retFlag){
                    popUp.showPopUp(selector,"success","业务操作:",data.msg);
                        callback.add(c);
                    if (thi=="") {
                        var en = jsonStrUtils.jsonStrToObj(data.obj);
                        callback.fire(en,type,inherit);
                    }else {
                        callback.fire(thi);
                    }
                        callback.empty();
                }else{
                    popUp.showPopUp(selector,"fail","业务操作:",data.msg);
                }
            },
            error:function (d) {
                popUp.close();
                popUp.showPopUp(selector,"fail","","请求或程序执行错误");
            },
            complete:function() {
                //setTimeout(popUp.close(),5000);
            }
        });
    };
    return{
        "ajaxInit":ajaxInit,
        "post":post
    }

});