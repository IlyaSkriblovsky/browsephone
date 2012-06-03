(function () {

ImagesPage = Class(Page, {
    init: function () {
        this.base()
        this._element.text('images')
    },

    title: function () { return 'Images' }
})

})();
