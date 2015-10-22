(function( $, undefined ) {
$.widget('silexlabs.pageable', {
  version: '1.0.1',
  options: {
    currentPage:'page-home',
    useDeeplink:true,
    pageClass: 'paged-element',
    onPageChanged: null,
    window: window // useful if you are in an iframe and want to set window = window.parent
  },
  // _setOption is called for each individual option that is changing
  _setOption: function( key, value ) {
    this.options[key] = value;
    switch (key) {
      case 'useDeeplink':
        this._destroy();
        this._create();
        break;
      case 'currentPage':
      case 'pageClass':
        this.updatePage();
        break;
    }
  },
  _create: function() {
    // mark the body
    $(document.body).addClass('pageable-plugin-created');
    // listen for page change
    var that=this;
    if(this.options.useDeeplink){
      $(this.options.window).bind( 'hashchange', this.cbk = function(){that.updatePage()});
    }
    else{
      // FIXME: this is wrong? it will prevent all links, whereas it should check indexOf('#!')
      this.element.find('a').each(function(){
        $(this).bind('click', function(event){
          event.preventDefault();
          if( $(this).attr('href').length ){
            that.options.currentPage = $(this).attr('href');
          }
          else {
            that.options.currentPage = 'page-home';
          }
          
          that.updatePage();
        });
      });
    }
    // mark all paged elements as hidden
    $('.' + this.options.pageClass).each(function() {
      if (!$(this).hasClass('paged-element-hidden'))
        $(this).addClass('paged-element-hidden');
    });
 
    this.updatePage();
  },
  _destroy: function() {
    if(this.options.useDeeplink){
      $(this.options.window).unbind( 'hashchange', this.cbk);
    }
    else{
      this.element.find('a').each(function(){
        $(this).unbind('click');
      });
    }
  },
  updatePage: function (){
    if(this.options.useDeeplink){
      if (this.options.window.location.hash && this.options.window.location.hash.indexOf('#!') >= 0)
        this.options.currentPage = this.options.window.location.hash;
    }
    if (this.options.currentPage && this.options.currentPage.indexOf('#!') >= 0){
        this.options.currentPage = this.options.currentPage.substr(this.options.currentPage.indexOf('#!') + 2);
    }
    else{
        this.options.currentPage = 'page-home';
    }

    // show elements which belong to this page
    $('#current-page-style').remove();
    $('head').append('<style id="current-page-style">.'+this.options.currentPage+'{display:inherit; }</style>');
    // mark these elements as visible
    $('.paged-element-visible').each(function() {
      $(this).removeClass('paged-element-visible');
      $(this).addClass('paged-element-hidden');
    });
    if( $('.'+this.options.currentPage).length )
    {
        $('.'+this.options.currentPage).each(function() {
        $(this).addClass('paged-element-visible');
        $(this).removeClass('paged-element-hidden');
        $(".background-image-container").css("height",150+$(this).height());
        $(".page-body-container").css("height",150+$(this).height());      
        });
    }

    // mark all links as not active
    $('.page-link-active').removeClass('page-link-active');
    $('.current').each( function() {
        $(this).removeClass('current');
    });
    // mark links to the current page as active
    if( $('[href="#!'+this.options.currentPage+'"]' ).length ){
        $('[href="#!'+this.options.currentPage+'"]' ).parent().addClass('current');        
    }
    else{
          $('[href="#!page-home"]').parent().addClass('current');  
    }
    
    // dispatch a change event
    $(this.element).trigger('pageChanged');
    if (this.options.onPageChanged) this.options.onPageChanged(this.options.currentPage);
  }
});
})(jQuery);
