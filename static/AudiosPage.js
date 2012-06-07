(function () {

AudiosPage = Class(Page, {
    init: function () {
        this.base()
    },

    title: function () { return 'Music' },

    show: function () {
        if (this._audios == null)
            this.load()
    },

    load: function () {
        this.loading(true)

        $.ajax({
            url: '/audios',
            dataType: 'json',

            success: this._dataLoaded.bind(this),

            complete: (function () { this.loading(false) }).bind(this)
        })
    },

    _dataLoaded: function (json) {
        this._audios = json

        this._generatePlayers()
    },

    _generatePlayers: function () {
        var list = this._element

        for (var i = 0; i < this._audios.length; i++) {
            var player =$('<div>')
                .prop('audio', this._audios[i])
                .append(
                    $('<a>', {
                        'class': 'jouele',
                        'href': '/content/' + this._audios[i].id
                    })
                    .text(this._audios[i].title)
                )
                .appendTo(list)
            window.joueleCreatePlayer.apply(player.children('a'))
        }
    }
})

})();
