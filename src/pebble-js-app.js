
function fetchVenues(latitude, longitude) {
  var req = new XMLHttpRequest();
  // Este comentario es nuevo
  req.open('GET', 'http://baasbox-soukron.rhcloud.com:8000/plugin/venues.fs.search?ll=' + latitude + ',' + longitude + '&X-BB-SESSION=1b24b1a8-a72f-4e15-a382-36c4282a9c2c', true);
//	req.open('GET', 'http://api.openweathermap.org/data/2.5/weather?' + 'lat=' + latitude + '&lon=' + longitude + '&cnt=1', true);
  req.onload = function () {
    if (req.readyState === 4) {
      if (req.status === 200) {
        console.log(req.responseText);
        var response = JSON.parse(req.responseText);
				
        var venue0 = response.data.response.venues[0].name;
				var venue1 = response.data.response.venues[1].name;
				var venue2 = response.data.response.venues[2].name;
      //  var icon = iconFromWeatherId(response.weather[0].id);
      //  var city = response.name;
        console.log(venue0);
				console.log(venue1);
				console.log(venue2);
     //   console.log(icon);
     //   console.log(city);
       Pebble.sendAppMessage({
				'VENUE_NAME_0': venue0,
				'VENUE_NAME_1': venue1,
				'VENUE_NAME_2': venue2
        });
      } else {
        console.log('Error');
			//	console.log(req.responseText);
      }
    }
  };
  req.send(null);
}

function locationSuccess(pos) {
  var coordinates = pos.coords;
  fetchVenues(coordinates.latitude, coordinates.longitude);
}

function locationError(err) {
  console.warn('location error (' + err.code + '): ' + err.message);
  Pebble.sendAppMessage({
    'WEATHER_CITY_KEY': 'Loc Unavailable',
    'WEATHER_TEMPERATURE_KEY': 'N/A'
  });
}

var locationOptions = {
  'timeout': 15000,
  'maximumAge': 60000
};

Pebble.addEventListener('ready', function (e) {
  console.log('connect!' + e.ready);
 // window.navigator.geolocation.getCurrentPosition(locationSuccess, locationError,
 //   locationOptions);
  console.log(e.type);
	console.log('Pebble Watch Token: ' + Pebble.getWatchToken());
	console.log('Pebble Account Token: ' + Pebble.getAccountToken());
});

Pebble.addEventListener('appmessage', function (e) {
  window.navigator.geolocation.getCurrentPosition(locationSuccess, locationError,
    locationOptions);
  console.log(e.type);
  console.log(e.payload.temperature);
  console.log('message!');
});

Pebble.addEventListener('webviewclosed', function (e) {
  console.log('webview closed');
  console.log(e.type);
  console.log(e.response);
});