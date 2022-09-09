define(['jquery','jquery.validate','messages_zh'],function ($) {
    var initVal=function(formSelector){
        formSelector.validate({
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
            }
        });

        formSelector.on('change',function(event){
            var ev= event || window.event;
            var target = ev.target || ev.srcElement;
            if(target.nodeName.toLowerCase()=="select"){
                $(target).parent().find(".select2-selection").prop("style","");
                $(target).parent().find("label[class='error']").remove();
            }
        });
    };

    var valid=function (formSelector) {
        return formSelector.valid();
    };
    return {
        "initVal":initVal,
        "valid":valid
    }
});