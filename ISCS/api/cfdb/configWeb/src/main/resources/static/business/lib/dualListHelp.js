define(['jquery','duallistbox'],function ($) {
    var options={
         "modalSelectSelector":"",
         "selectedLabel":"",
         "nonSelectedLabel":""
    };
    var dualInit=function (option) {
        options.modalSelectSelector=option.modalSelectSelector;
        options.selectedLabel=option.selectedLabel;
        options.nonSelectedLabel=option.nonSelectedLabel;

        init();
    };
    var init=function (){
        var a=$(options.modalSelectSelector).bootstrapDualListbox({
            nonSelectedListLabel: options.nonSelectedLabel,
            selectedListLabel: options.selectedLabel,
            preserveSelectionOnMove: 'moved',
            moveOnSelect: false,
            nonSelectedFilter: ''
        });
        $(options.modalSelectSelector).next(".select2-container").hide();
    };
    return {
        "dualInit":dualInit
    }
});