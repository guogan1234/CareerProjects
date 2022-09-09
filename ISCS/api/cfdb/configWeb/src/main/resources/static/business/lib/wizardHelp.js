define(['wizard'],function (wizard) {

    var wizardInit=function (selector) {
        $('#'+selector).bootstrapWizard({
            'tabClass': 'form-wizard',
            'onNext': function (tab, navigation, index) {
                $('#'+selector).find('.form-wizard').children('li').eq(index-1).addClass('complete');
                $('#'+selector).find('.form-wizard').children('li').eq(index-1).find('.step').html('<i class="fa fa-check"></i>');
            },
            'onPrevious':function (tab,navigation,index) {
                var tabs=tab.prevAll("li");
                var key=0;
                var bl=0;
                tabs.each(function () {
                    if ($(this).css("display")=="none"){
                        bl++;
                    }
                });
                for(var i=0;i<tabs.length;i++){
                    var ds=tabs.eq(i).css("display");
                    if(ds =="none"){
                        continue;
                    }else{
                        key =tabs.length-bl;
                        break;
                    }
                }
                $('#'+selector).bootstrapWizard('show',key);
            }
        });
    };
    var wizardHide=function (selector,index) {
        $('#'+selector).bootstrapWizard('hide',index);
    };
    
    var wizardDisplay=function (selector,index) {
        $('#'+selector).bootstrapWizard('display',index);

    };

    return {
        "wizardInit":wizardInit,
        "wizardHide":wizardHide,
        "wizardDisplay":wizardDisplay
    }
});