$("#addUser").click(function(){
    alert(111);
    $.ajax({
            type:"post",
            url:"/addDemo",
            success:function(data){

            }
        });

});