function loadAndAdjustBackground(newPageName){
    $('.main-content').load('content/' + newPageName + '.html' , function(){
        var contentHeight = $('.main-content').height();
        var headerOffset = 105;
        $('.background-image-container').height(contentHeight + headerOffset);
        
        if(newPageName === 'contact')
        {
            $('.send-mail').attr('href',mailTo());
        }
//        $('.send-mail').text(mailAddress());
    });
}

function spacyDelimiter(){
    return '#';
}

function getPageName(fullPageName){
    var newPageName = fullPageName;
    
    var delim = spacyDelimiter();
    if (newPageName.indexOf(delim) >= 0){
        newPageName = newPageName.substr(newPageName.indexOf(delim) + delim.length);
    }
    else{
        newPageName = 'home';
    }
    return newPageName;
}

function clearNavigationLinks(){
    $('.current').each( function() {
        $(this).removeClass('current');
    });
}

$(document).ready( function(){    
    $('.send-mail').click( function(){
        emailMe();
        return true;
    } );
    $('.start-page a').click( function() {
        location.hash=$(this).attr('href').match(/(^.*)\./)[1]
        return false;
    } );
    $('.tablist li a').click( function(){
        location.hash=$(this).attr('href').match(/(^.*)\./)[1]
        return false;
    });
    
    function hashChange(){
            var page=location.hash.slice(spacyDelimiter().length);
            if (page!=""){
                $('.background-image-container').removeClass('hidden-element');
                $('.start-page').addClass('hidden-element');
                
                loadAndAdjustBackground(page);
                
                clearNavigationLinks();
                
                var fullPageName = spacyDelimiter() + page;
                $('[href="' + fullPageName + '"]').parent().addClass('current');
            }
            else{
                $('.background-image-container').addClass('hidden-element');
                $('.start-page').removeClass('hidden-element');                            
            }
    }
    if ("onhashchange" in window){ // cool browser
            $(window).on('hashchange',hashChange).trigger('hashchange')
    }else{ // lame browser
            var lastHash=''
            setInterval(function(){
                    if (lastHash!=location.hash)
                            hashChange()
                    lastHash=location.hash
            },100)
    }
});

