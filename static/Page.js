(function () {

Page = Class({

    init: function () {
        this._element = $(
            '<div class="ui-page">'
          + '</div>'
        )

        $(window).resize(this.onResize.bind(this))
    },

    element: function () { return this._element },

    title: function () { return 'unknown' },

    show: function () {
    },

    onResize: function () {
    }

})
.implement(Class.Properties({

    loading: {
        value: false,
        set: function (value) {
            if (value == this._loading) return

            this._loading = value

            if (value)
            {
                this._element.append(
                    '<div class="ui-loading">'
                  + '</div>'
                )
            }
            else
            {
                this._element.children('.ui-loading').remove()
            }
        }
    }

}))

})();
