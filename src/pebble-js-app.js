
function fetchVenues(latitude, longitude) {
  var req = new XMLHttpRequest();
  req.open('GET', 'http://baasbox-soukron.rhcloud.com:8000/plugin/venues.fs.search?ll=' + latitude + ',' + longitude + '&X-BB-SESSION=1b24b1a8-a72f-4e15-a382-36c4282a9c2c', true);
  req.onload = function () {
    if (req.readyState === 4) {
      if (req.status === 200) {
        console.log(req.responseText);
        var response = JSON.parse(req.responseText);
				
        var venue0 = response.data.response.venues[0].name;
				var venue1 = response.data.response.venues[1].name;
				var venue2 = response.data.response.venues[2].name;
    
        console.log(venue0);
				console.log(venue1);
				console.log(venue2);
  
       Pebble.sendAppMessage({
					
        });
      } else {
        console.log('Error');
      }
    }
  };
  req.send(null);
}


function fetchVenuestest(latitude, longitude) {
Pebble.sendAppMessage({
	'DATA_TYPE': 'venues',
	'DATA_LENGTH': '4',
	'ITEM_1_ID': '4b7339f8f964a52006a32de3',
	'ITEM_1_NAME': 'Paiporta',
	'ITEM_2_ID': '4c38c73893db0f4788392292',
	'ITEM_2_NAME': 'Auditori Paiporta',
	'ITEM_3_ID': '4e2dea85b0fbdc2b64f23f3b',
	'ITEM_3_NAME': 'PLAZA MAYOR',
	'ITEM_4_ID': '4f40f809e4b028f640a5a1c1',
	'ITEM_4_NAME': 'AMANECER'
});
}

function locationSuccess(pos) {
  var coordinates = pos.coords;
  fetchVenuestest(coordinates.latitude, coordinates.longitude);
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
  if (e.payload.FETCH_TYPE == 1) window.navigator.geolocation.getCurrentPosition(locationSuccess, locationError, locationOptions);
	console.log(JSON.stringify(e.payload));
  console.log(e.payload.FETCH_TYPE);
  console.log('message!');
	
	
	
});

Pebble.addEventListener('webviewclosed', function (e) {
  console.log('webview closed');
  console.log(e.type);
  console.log(e.response);
});