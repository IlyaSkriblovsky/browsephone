(function () {

ContactsPage = Class(Page, {
    init: function () {
        this.base()
        this._element.html(
            '<div class="ui-contacts-filter">'
          + '</div>'
          + '<ul class="ui-contacts-list">'
          + '</ul>'
        )

        this._contacts = null

        var self = this

        this._element.find('.ui-contacts-filter')
            .on('click', '.ui-contacts-letter', function () {
                self._onLetterClicked($(this))
            })

        this._element.find('.ui-contacts-list')
            .on('click', 'li', function () {
                self._onContactClicked($(this))
            })
    },

    title: function () { return 'Contacts' },

    show: function () {
        if (this._contacts == null)
            this.load()
    },

    load: function () {
        this.loading(true)

        $.ajax({
            url: '/contacts',
            dataType: 'json',

            success: this._dataLoaded.bind(this),

            complete: (function () {
                this.loading(false)
            }).bind(this)
        })
    },

    _dataLoaded: function (json) {
        this._contacts = json

        this._contacts.sort(function (a, b) {
            if (a.label.toUpperCase() < b.label.toUpperCase()) return -1
            if (a.label.toUpperCase() > b.label.toUpperCase()) return +1
            return 0
        })

        this._generateFilter()
        this._generateContent()
    },

    _generateFilter: function (json) {
        var filter = this._element.find('.ui-contacts-filter')
            .html(
                '<div class="ui-contacts-letter ui-contacts-letter-all active">All</div>'
            )

        var last = ''
        for (var i = 0; i < this._contacts.length; i++)
        {
            var name = this._contacts[i].label.toUpperCase()
            if (name[0] != last) {
                var letter = $('<div>', { 'class': 'ui-contacts-letter' })
                    .text(name[0])
                    .appendTo(filter)
            }

            last = name[0]
        }

        filter.show()

        this.onResize();
    },

    _generateContent: function () {
        var list = this._element.children('.ui-contacts-list')

        for (var i = 0; i < this._contacts.length; i++) {
            var contact = this._contacts[i]
            var ul = $('<li>')
                .attr('ci', i)
                .attr('cid', contact.id)
                .text(contact.label)
                .appendTo(list)
        }
    },


    onResize: function () {
        var filter = this._element.children('.ui-contacts-filter')
        var list = this._element.children('.ui-contacts-list')

        list.css('top', filter.height())
    },

    _onLetterClicked: function (letter) {
        letter.addClass('active').siblings().removeClass('active')

        if (letter.hasClass('ui-contacts-letter-all'))
        {
            this._element.find('.ui-contacts-list').children().show()
        }
        else
        {
            var letter = letter.text()[0]

            this._element.find('.ui-contacts-list').children().each((function (i, li) {
                li = $(li)
                li.toggle(this._contacts[li.attr('ci')].label.toUpperCase()[0] == letter)
            }).bind(this))
        }
    },



    _detailDiv: function (options) {

        var div = $('<div>', { 'class': 'ui-contact-detail' })
            .append($('<div>', { 'class': 'ui-contact-detail-name' }).text(options.name))

        if (options.context)
            div.append(
                $('<div>', { 'class': 'ui-contact-detail-context' })
                    .text(options.context)
            )

        var contdiv = $('<div>', { 'class': 'ui-contact-detail-content' })

        if (options.isHtml)
            contdiv.html(options.content)
        else
            contdiv.text(options.content)
        div.append(contdiv)


        return div
    },

    _onContactClicked: function (li) {
        var contact = this._contacts[li.attr('ci')]

        var dialog = $('<div>', { 'class': 'ui-contact-dialog' })
            .append($('<h2>', { 'class': 'mui_dialog_title' }).text(contact.label))

        for (var i = 0; i < contact.details.length; i++) {
            var d = contact.details[i]
            switch (d.def) {
                case 'Name': {
                    dialog.append(this._detailDiv({
                        name: 'Name:',
                        content: (d.Prefix ? d.Prefix : '') + ' '
                                  + (d.FirstName ? d.FirstName : '') + ' '
                                  + (d.MiddleName ? d.MiddleName : '') + ' '
                                  + (d.LastName ? d.LastName : '') + ' '
                                  + (d.Suffix ? d.Suffix : '')
                    }))
                    break
                }

                case 'Organization': {
                    dialog.append(this._detailDiv({
                        name: 'Organization:',
                        content: d.Name + (d.Title ? ', ' + d.Title : '')
                    }))
                    break
                }

                case 'PhoneNumber': {
                    dialog.append(this._detailDiv({
                        name: 'Phone number:',
                        content: d.PhoneNumber,
                        context: d.Context ? d.Context[0] : ''
                    }))
                    break
                }

                case 'Address': {
                    var parts = []
                    if (d.Street) parts.push(d.Street)
                    if (d.Postcode) parts.push(d.Postcode)
                    if (d.PostOfficeBox) parts.push(d.PostOfficeBox)
                    if (d.Locality) parts.push(d.Locality)
                    if (d.Region) parts.push(d.Region)
                    if (d.Country) parts.push(d.Country)
                    for (var j = 0; j < parts.length; j++)
                        parts[j] = parts[j].replace(/\n/g, ', ').replace(/&/g, '&amp;').replace(/</g, '&lt;').replace(/>/g, '&gt;')

                    dialog.append(this._detailDiv({
                        name: 'Address:',
                        content: parts.join('<br/>'),
                        isHtml: true,
                        context: d.Context ? d.Context[0] : ''
                    }))
                    break
                }

                case 'EmailAddress': {
                    dialog.append(this._detailDiv({
                        name: 'E-Mail:',
                        content: d.EmailAddress,
                        context: d.Context ? d.Context[0] : ''
                    }))
                    break
                }

                case 'Nickname': {
                    dialog.append(this._detailDiv({
                        name: 'Nickname:',
                        content: d.Nicknameddress
                    }))
                    break
                }
            }
        }

        dialog.mui_dialog()
    }

})

})();
