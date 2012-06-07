(function () {

ImagesPage = Class(Page, {
    init: function () {
        var self = this

        this.base()

        this._element.html(
            '<div class="ui-images-list">'
          + '</div>'
        )
        .on('click', '.ui-images-thumb', function () {
            self._onThumbClicked($(this))
        })
    },

    title: function () { return 'Images' },

    show: function () {
        if (this._images == null)
            this.load()
    },

    load: function () {
        this.loading(true)

        $.ajax({
            url: '/images',
            dataType: 'json',

            success: this._dataLoaded.bind(this),

            complete: (function () { this.loading(false) }).bind(this)
        })
    },

    _dataLoaded: function (json) {
        this._images = json

        this._generateThumbs()
    },


    _generateThumbs: function () {
        var list = this._element.children('.ui-images-list')

        for (var i = 0; i < this._images.length; i++) {
            $('<div>', { 'class': 'ui-images-thumb' })
                .prop('image', this._images[i])
                .append(
                    $('<img>', { 'class': 'ui-images-thumb-img' })
                        .attr('src', '/thumb/' + this._images[i].id)
                )
                .append(
                    $('<div>', { 'class': 'ui-images-thumb-title' })
                        .text(this._images[i].title)
                )
                .appendTo(list)
        }
    },

    _onThumbClicked: function (thumb) {
        var image = thumb.prop('image')

        var imgw = image.width
        var imgh = image.height

        var maxw = window.innerWidth * 0.8
        var maxh = window.innerHeight * 0.8

        if (image.width / image.height > maxw / maxh)
        {
            imgw = maxw
            imgh = imgw / image.width * image.height
        }
        else
        {
            imgh = maxh
            imgw = imgh / image.height * image.width
        }

        $('<div>', { 'class': 'ui-image-dialog' })
            .append(
                $('<img>', { 'class': 'ui-image-dialog-img' })
                    .attr('src', '/content/' + image.id)
                    .css({
                        'width': imgw,
                        'height': imgh
                    })
            )
            .mui_dialog()
    }
})

})();
