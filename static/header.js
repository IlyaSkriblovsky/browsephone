(function () {

UI.Header = Class({
    init: function () {
        this._element = $(
            '<div class="ui-header">'
          + '</div>'
        )
    },

    element: function () { return this._element }
})

})();
