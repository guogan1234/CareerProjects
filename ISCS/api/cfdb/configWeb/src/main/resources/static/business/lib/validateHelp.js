define(['jquery','jquery.validate','messages_zh'],function ($) {
    var options={
        "formSelector":"",//form选择器
        "rules":""
    };
    var validateInit=function (option) {
        options.formSelector=option.formSelector;
        console.log(options.formSelector);
        options.rules=option.rules;

        init();
    };
    var init=function () {
        $("#"+options.formSelector).validate({
            errorClass:"error",
            ignore: "",
            focusInvalid : true,
            errorPlacement: function(error, element) {
                if (element.is("select")){
                    error.appendTo(element.parent());
                    $(".select2-selection").css("background","#FFF7FA");
                    $(".select2-selection").css("border","1px solid #CE7979");
                }else {
                    error.insertAfter(element);
                }
            },
            rules:options.rules
        });
        $("#"+options.formSelector).on('change',function(event){
            var ev= event || window.event;
            var target = ev.target || ev.srcElement;
            if(target.nodeName.toLowerCase()=="select"){
                $(target).parent().find(".select2-selection").prop("style","");
                $(target).parent().find("label[class='error']").remove();
            }
        });
    };
    var valid=function () {
       return $("#"+options.formSelector).valid();
    };

    //校验子网掩码
    $.validator.addMethod("checkMask",function(value,element,params){
        if (value=="" || value==null){
            return true;
        }
        var exp=/^(254|252|248|240|224|192|128|0)\.0\.0\.0|255\.(254|252|248|240|224|192|128|0)\.0\.0|255\.255\.(254|252|248|240|224|192|128|0)\.0|255\.255\.255\.(254|252|248|240|224|192|128|0)$/;
        var reg = value.match(exp);
        if(reg==null) {
            return false; //"非法"
        }
         return true; //"合法"
    },"格式错误,请改正");
    //校验网卡地址
    $.validator.addMethod("checkMac",function(value,element,params){
        if (value=="" || value==null){
            return true;
        }
        var reg1 = /^[A-Fa-f0-9]{1,2}\-[A-Fa-f0-9]{1,2}\-[A-Fa-f0-9]{1,2}\-[A-Fa-f0-9]{1,2}\-[A-Fa-f0-9]{1,2}\-[A-Fa-f0-9]{1,2}$/;
        var reg2 = /^[A-Fa-f0-9]{1,2}\:[A-Fa-f0-9]{1,2}\:[A-Fa-f0-9]{1,2}\:[A-Fa-f0-9]{1,2}\:[A-Fa-f0-9]{1,2}\:[A-Fa-f0-9]{1,2}$/;
        if (reg1.test(value)) {
            return true;
        } else if (reg2.test(value)) {
            return true;
        }
        return false;
    },"格式错误,请改正");

    return {
        "validateInit":validateInit,
        "valid":valid
    }
});