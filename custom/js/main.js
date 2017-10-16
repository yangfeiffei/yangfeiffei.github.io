$(function(){
    var padding=[10,20,30,40,50];
    $('.post').find('h1,h2,h3,h4,h5').each(function(){
        // $('<a name="c'+index+'"></a>').insertBefore($(this));
        var headerText=$(this).text();
        var myTagName=$(this)[0].tagName.toLowerCase();
        var tagIndex=parseInt(myTagName.charAt(1))-1;
        //设置不同等级header的排列及缩进样式
        $("#menu-content").append('<a href="#'+$(this)[0].id+'" style="padding-left:'+padding[tagIndex]+'px;display:block;">'+headerText+'</a>');
    });
});