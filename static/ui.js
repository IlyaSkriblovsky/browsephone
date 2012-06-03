(function () {

UI = Class({

    init: function () {
        this._router = new Router

        this._pages = {
            'contacts': new ContactsPage,
            'images': new ImagesPage
        }

        $('body')
            .append(new UI.Header().element())
            .append(
                '<ul class="ui-pagelinks"><ul>'
            )
            .append(
                '<div class="ui-page-container"></div>'
            )

        var linksUl = $('.ui-pagelinks')
        for (var id in this._pages) {
            var link = $('<a href="#' + id + '">')
                .text(this._pages[id].title())

            linksUl.append($('<li>', { 'class': 'ui-pagelink' }).append(link))

            var page = this._pages[id]
        }

        this._router.bind('changed', this._onUrlChange.bind(this))
    },

    _onUrlChange: function () {
        for (var id in this._pages)
        {
            if (window.location.hash == '#' + id)
            {
                $('.ui-page-container').children().each(function () { $(this).detach() })
                this._pages[id].element().appendTo('.ui-page-container')
                this._pages[id].show()
                break
            }
        }

        $('.ui-pagelink')
            .removeClass('active')

        console.log($('.ui-pagelink a[href="' + window.location.hash + '"]'))
        $('.ui-pagelink a[href="' + window.location.hash + '"]')
            .parent().addClass('active')
    }

})

})();
