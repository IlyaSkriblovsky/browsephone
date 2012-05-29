$(function () {
    $.ajax({
        url: '/images',
        dataType: 'json',

        success: function (json) {
            $.each(json, function () {
                var img = $('<img>')
                    .attr({
                        src: '/thumb/' + this.id
                    })
                    .appendTo('body')
            })
        },

        error: function() {
            console.log('error')
            console.log(arguments)
            a1 = arguments[0]
            a2 = arguments[1]
            a3 = arguments[2]
        }
    })
})
