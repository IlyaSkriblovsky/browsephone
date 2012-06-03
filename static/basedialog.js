$.widget('mui.basedialog', {
    options: {
        outsideclick: function() { return false }
    },

    dimmer: null,
    wrapper: null,

    _create: function() {
        var self = this, options = this.options

        this.dimmer =
            $('<div class="mui_basedialog_dimmer">')
                .click(function() {
                    if (self._trigger('outsideclick', null, []) == false)
                        self.close()
                    return false
                })

        this.wrapper = $('<div>')
            .addClass('mui_basedialog')
            .append('<div class="mui_basedialog_close">')
            .append(this.element)

        this.wrapper.find('.mui_basedialog_close')
            .click(function() { self.close() })

        this.dimmer.appendTo('body')
        this.wrapper.appendTo('body')
    },

    destroy: function() {
        this.element.siblings('.mui_basedialog_close').remove()
        this.element.unwrap()
        this.dimmer.add(this.wrapper).remove()
        this.element.hide()

        delete this.dimmer
        delete this.wrapper

        $.Widget.prototype.destroy.call(this)
    },

    _init: function() {
        this.wrapper.add(this.dimmer)
            .show()
            .addClass('mui_basedialog_visible')

        this.element.show()

        this.wrapper
            .position({
                my: 'center',
                at: 'center',
                of: this.dimmer
            })
    },

    close: function() {
        if (this.options.canClose && this.options.canClose.apply(this.element) == false)
            return

        this.wrapper.removeClass('mui_basedialog_visible').addClass('mui_basedialog_postshow')
        this.dimmer.removeClass('mui_basedialog_visible')

        var self = this
        setTimeout(function() {
            self.wrapper.removeClass('mui_basedialog_postshow')

            self._trigger('close')
        }, 250)
    }
});
