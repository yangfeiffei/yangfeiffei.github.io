$(function(){

    // 生成目录
    var padding=[0,10,20,30,40];
    $('.post').find('h1,h2,h3,h4,h5').each(function(){
        // $('<a name="c'+index+'"></a>').insertBefore($(this));
        var headerText=$(this).text();
        var myTagName=$(this)[0].tagName.toLowerCase();
        var tagIndex=parseInt(myTagName.charAt(1))-1;
        //设置不同等级header的排列及缩进样式
        $("#menu-content").append('<a href="#'+$(this)[0].id+'" style="padding-left:'+padding[tagIndex]+'px;display:block;">'+headerText+'</a>');
    });

    // 为引用类型增加标记：note：
    $('.post blockquote').each(function () {
        $(this).prepend(
            '<div style="font-size: 20px;color: #194fa0;"><span class="glyphicon glyphicon-pencil"></span>&nbsp;<strong>Note:</strong></strong></div>'
        );
    });

    // 为H2增加标记：
    $('.post h2').each(function () {
        $(this).prepend(
            '<span class="glyphicon glyphicon-menu-right">&nbsp;</span>'
        );
    });
});