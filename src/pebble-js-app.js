
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


function fetchVenuesTest(latitude, longitude) {
Pebble.sendAppMessage({
	'DATA_TYPE': 0, //venues
	'DATA_LENGTH': 5,
	'ITEM_1_ID': '4b7339f8f964a52006a32de3',
	'ITEM_1_NAME': 'Paiporta',
	'ITEM_2_ID': '4c38c73893db0f4788392292',
	'ITEM_2_NAME': 'Auditori Paiporta',
	'ITEM_3_ID': '4e2dea85b0fbdc2b64f23f3b',
	'ITEM_3_NAME': 'PLAZA MAYOR',
	'ITEM_4_ID': '4f40f809e4b028f640a5a1c1',
	'ITEM_4_NAME': 'AMANECER',
	'ITEM_5_ID': '4f40f809e4b0287640a5a1c1',
	'ITEM_5_NAME': 'JARL'
});
console.log("Venues enviadas al Pebble");
}


function fetchItemsTest(venue_id) {
Pebble.sendAppMessage({	
	'DATA_TYPE': 1, //items
	'DATA_LENGTH': 2,
	'ITEM_1_ID': '4b7339f8f964a52006a32',
	'ITEM_1_NAME': 'Llave de oro',
	'ITEM_2_ID': '473893db0f4788392292',
	'ITEM_2_NAME': '10 Monedas',
});
console.log("Ítems enviados al Pebble");
}




function locationSuccess(pos) {
  var coordinates = pos.coords;
  fetchVenuesTest(coordinates.latitude, coordinates.longitude);
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





// Called when JS is ready
Pebble.addEventListener("ready",
												function(e) {
													console.log("JS is ready");

													var http = new XMLHttpRequest();
													var params = '{ "username": "' + Pebble.getAccountToken() + '", "password": "' + Pebble.getWatchToken() + '", "appcode": "1234567890" }';
													http.open("POST", "http://baasbox-soukron.rhcloud.com:8000/login", true);

													//Send the proper header information along with the request
													http.setRequestHeader("Content-type", "application/json");
													http.setRequestHeader("Connection", "close");

													//Call a function when the state changes
													http.onload = function() {
														if(http.readyState == 4 && http.status == 200) {
															//	console.log(http.responseText);
															var response = JSON.parse(http.responseText);
															console.log('X-BB-SESSION -> ' + response.data["X-BB-SESSION"]);

														}
														else {
															console.log("Ha habido algun error");
															console.log(http.responseText);
														}
													};

													http.send(params);
												});



Pebble.addEventListener('appmessage', function (e) {
  //Fetch venues
	if (e.payload.FETCH_TYPE == 0) window.navigator.geolocation.getCurrentPosition(locationSuccess, locationError, locationOptions);
	//Fetch items from venues
	if (e.payload.FETCH_TYPE == 1)  fetchItemsTest(999);
	
	
//	console.log(JSON.stringify(e.payload));
  console.log(e.payload.FETCH_TYPE);
//  console.log('message!');
	
});

Pebble.addEventListener('webviewclosed', function (e) {
  console.log('webview closed');
  console.log(e.type);
  console.log(e.response);
});