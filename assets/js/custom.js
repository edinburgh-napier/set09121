console.log("custom code going on up in here");

$(window).ready(function(){
    console.log("custom code setting off");
    $(".tip").wrap(function() {return "<a href='"+$(this).attr("tip")+"'></a>";});
});