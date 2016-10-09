console.log("js working")
$.ajax({
        url: 'http://freegeoip.net/json/',
        type: 'POST',
        dataType: 'jsonp',
        success: function(location) {
            alert(location.ip);
        }
    });
