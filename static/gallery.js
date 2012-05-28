$(function () {
    $.ajax({
        url: '/images',
        dataType: 'json',

        success: function (json) {
            console.log(json)
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
