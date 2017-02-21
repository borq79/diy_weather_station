function showWeatherData() {
  $.ajax({
    url: "/api/i",
    success: function(weatherData) {
      $("#deviceUptime").text('Device Uptime: ' + weatherData.u + ' ms');
      $("#temp").html('Temp: ' + weatherData.t + "&deg;");
      $("#humidity").text('Humidity: ' + weatherData.h + "%");
      $("#pressure").text('Pressure: ' + weatherData.p + " in");
      $("#brightness").text('Brightness: ' + weatherData.b);
      $("#weatherDataPage").toggle(true);
      $("#configDataPage").toggle(false);
      $("#weatherDataPageLoading").toggle(false);
    }
  });
}

function showConfigData() {
  $.ajax({
    url: "/api/c",
    success: function(config) {
      $.each(config, function(name, val){
        var $el = $('[name="'+name+'"]'),
            type = $el.attr('type');

        switch(type){
            case 'checkbox':
                $el.attr('checked', 'checked');
                break;
            case
             'radio':
                $el.filter('[value="'+val+'"]').attr('checked', 'checked');
                break;
            default:
                $el.val(val);
        }
      });

      $("#weatherDataPage").toggle(false);
      $("#configDataPage").toggle(true);
      $("#weatherDataPageLoading").toggle(false);
    },
    error: function(jqXHR, textStatus, errorThrown) {
      alert("Failed to load the config page: " + textStatus);
    }
  });
}


function configUpdate(form) {
  var newConfig = {};
  $(form).serializeArray().map(function(x){newConfig[x.name] = x.value;});
  console.log( newConfig );

  $.ajax({
    url: "/api/u",
    type: "POST",
    async: false,
    data: JSON.stringify(newConfig),
    contentType: "application/json; charset=utf-8",
    dataType: "json",
    success: function(config) {
      $("#saveConfigSuccess").show();
      $("#saveConfigFailure").hide();
    },

    error: function() {
      $("#saveConfigSuccess").hide();
      $("#saveConfigFailure").show();
    }
  });
}
