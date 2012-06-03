(function () {

Router = Class({
    init: function () {
        $(window).on('hashchange', this._onChange.bind(this))

        this._listeners = {}
    },

    _onChange: function () {
        this.notify('changed')

        console.log(window.location.hash)

        if (this._listeners.hasOwnProperty(window.location.hash)) {
            var cbs = this._listeners[window.location.hash]
            for (var i = 0; i < cbs.length; i++)
                cbs[i]()
        }
    },


    on: function (hash, callback) {
        if (this._listeners.hasOwnProperty(hash))
            this._listeners[hash].push(callback)
        else
            this._listeners[hash] = [callback]
    }
})
.implement(Class.EventSource)

})();
