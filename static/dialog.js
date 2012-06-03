$.widget('mui.mui_dialog', $.mui.basedialog, {
    options: {
        buttons: []
    },

    _create: function() {
        $.mui.basedialog.prototype._create.call(this)

        var self = this, options = this.options

        this.element.closest('.mui_basedialog')
            .toggleClass('mui_dialog_error', this.options.error == true)

        this.element.addClass('mui_dialog_content')

        if (this.options.buttons.length > 0)
        {
            this.wrapper
                .append('<div class="mui_dialog_buttonbox">')
                .children('.mui_dialog_buttonbox')
                    .buttonbox({
                        buttons: $.map(this.options.buttons, function(btn) {
                            return {
                                text: btn.text,
                                click: btn.click.bind(self.element)
                            }
                        })
                    })
        }
    },

    destroy: function() {
        this.element.removeClass('mui_dialog_content')
        this.element.siblings('.mui_dialog_buttonbox').remove()

        $.mui.basedialog.prototype.destroy.call(this)
    }
});


$.mui.mui_dialog.messagebox = function (options) {
    $('<div>')
        .append($('<h2 class="mui_dialog_title"></h2>').text(options.title))
        .append(options.message)
        .mui_dialog($.extend({}, options, {
            close: function() {
                $(this).dialog('destroy')
                $(this).remove()
            }
        }))
};

$.mui.mui_dialog.errorbox = function (options) {
    return $.mui.mui_dialog.messagebox($.extend(options, {error:true}))
};
