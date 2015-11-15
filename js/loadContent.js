function spacyDelimiter(){
    return '#';
}

function clearNavigationLinks(){
    $('.current').each( function() {
        $(this).removeClass('current');
    });
}

function clearExampleLinks(){
    $('.current-example').each( function() {
        $(this).removeClass('current-example');
    });
}


function adjustBackgroundHeight(){
    var contentHeight = $('.main-content').height();
    var headerOffset = 150;
    $('.background-image-container').height(contentHeight + headerOffset);
}


function loadAndAdjustBackground(newPageName){
    $('.main-content').load('content/' + newPageName + '.html' , function(){
        if( newPageName === 'details'){
            MathJax.Hub.Queue( adjustBackgroundHeight );
            MathJax.Hub.Queue( drawChart );
            MathJax.Hub.Queue( adjustBackgroundHeight );
            MathJax.Hub.Queue(["Typeset",MathJax.Hub]);
            MathJax.Hub.Queue( adjustBackgroundHeight );
        }
        else{
            adjustBackgroundHeight();

            if(newPageName === 'contact')
            {
                $('.send-mail').attr('href',mailTo());
            }
        }
    });
}

function loadExample(newPageName){
    $('.example-container').addClass('hidden');
    $('.example-container').load('content/' + newPageName + '.html' , function(){
        MathJax.Hub.Queue(["Typeset",MathJax.Hub], adjustBackgroundHeight);
        MathJax.Hub.Queue( function(){
            clearExampleLinks();
            $('[href="' + spacyDelimiter() + newPageName + '"]').parent().addClass('current-example');
            $('.example-container').removeClass('hidden');
        });
    });
}

function loadExampleAndAdjustBackground(newPageName){
    if( $('.main-content').find('.example-list').length > 0){
        loadExample(newPageName);
    }
    else{
        $('.main-content').load('content/examples.html', function(){
            loadExample(newPageName);
        });
    }
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
    $('.example-list li a').click( function(){
        location.hash=$(this).attr('href').match(/(^.*)\./)[1]
        return false;
    });
    
    function hashChange(){
            var page=location.hash.slice(spacyDelimiter().length);
            if (page!=""){
                $('.background-image-container').removeClass('hidden-element');
                $('.start-page').addClass('hidden-element');
                
                if(/^examples/.test(page) && page.length > 8){
                    loadExampleAndAdjustBackground(page);
                }
                else{
                    loadAndAdjustBackground(page);
                }
                
                clearNavigationLinks();

                var fullPageName = spacyDelimiter() + page;
                if(/^examples/.test(page) && page.length > 8){
                    $('[href=#examples]').parent().addClass('current');
                }
                else{
                    $('[href="' + fullPageName + '"]').parent().addClass('current');
                }
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

