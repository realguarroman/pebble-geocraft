var session;
var url = "http://geocraft-sgarcia.itos.redhat.com/";

//var url_old = "http://geocraft.oshift.net/";

function fetchVenues(latitude, longitude) {
  var req = new XMLHttpRequest();
  req.open('GET', url + 'plugin/geocraft.search.locations?ll=' + latitude + ',' + longitude + '&X-BB-SESSION=' + session, true);
	req.onload = function () {
    if (req.readyState === 4) {
			if (req.status === 200) {
        console.log(req.responseText);
        var response = JSON.parse(req.responseText);
				var length =  response.data.count;
				if (length > 10) length = 10;
				// Assemble dictionary using our keys
				var dictionary = {
					'DATA_TYPE': 1, //venues
					'DATA_LENGTH': length,
				};			
				var i;
				for (i = 1; i <= length; i++) { 
					dictionary["ITEM_" + i + "_ID"] = response.data.results[i-1].venue_id;
					dictionary["ITEM_" + i + "_NAME"] = response.data.results[i-1].name;
					dictionary["ITEM_" + i + "_ICON"] = response.data.results[i-1].index;
				}				
				Pebble.sendAppMessage(dictionary);
      } else {
        console.log('Error');
      }
    }
  };
  req.send(null);
}



function fetchItems(venue_id) {
  var req = new XMLHttpRequest();
	console.log(url + 'plugin/geocraft.view.location?venue_id=' + venue_id + '&X-BB-SESSION=' + session);
  req.open('GET',url + 'plugin/geocraft.view.location?venue_id=' + venue_id + '&X-BB-SESSION=' + session, true);
	req.onload = function () {
    if (req.readyState === 4) {
			if (req.status === 200) {
        console.log(req.responseText);
        var response = JSON.parse(req.responseText);
				var length =  response.data.objects.length;
				var slots =  response.data.location_category.slots;
				var location = response.data.id;
				var index = response.data.location_category.index;
			//	console.log('slots');
			//	console.log(slots);
				
				if (length > 10) length = 10;
				// Assemble dictionary using our keys
				var dictionary = {
					'DATA_TYPE': 2, //items
					'DATA_LENGTH': slots,
					'ID_LOCATION': location,
					'LOCATION_ICON': index,
				};			
				var i;
				for (i = 1; i <= length; i++) { 
					dictionary["ITEM_" + i + "_ID"] = response.data.objects[i-1].id;
					dictionary["ITEM_" + i + "_NAME"] = response.data.objects[i-1].object_type.name;
				}			
				for (i = length + 1; i <= slots; i++) { 
					dictionary["ITEM_" + i + "_ID"] = 0;
					dictionary["ITEM_" + i + "_NAME"] = "empty";
				}				
				Pebble.sendAppMessage(dictionary);
      } else {
        console.log('Error');
      }
    }
  };
  req.send(null);
}



function pickItem(object_id,location_id) {
  var req = new XMLHttpRequest();
	console.log(url + 'plugin/geocraft.pick.object?object_id=' + object_id + '&location_id=' + location_id + '&X-BB-SESSION=' + session);
  req.open('PUT', url + 'plugin/geocraft.pick.object?object_id='  + object_id + '&location_id=' + location_id + '&X-BB-SESSION=' + session, true);
	req.onload = function () {
    if (req.readyState === 4) {
			if (req.status === 200) {
        console.log(req.responseText);
      
				 
        var response = JSON.parse(req.responseText);
				var length =  response.data.objects.length;
				var slots =  response.data.location_category.slots;
				var location = response.data.id;
				
			//	console.log('slots');
			//	console.log(slots);
				
				if (length > 10) length = 10;
				// Assemble dictionary using our keys
				var dictionary = {
					'DATA_TYPE': 3, //pick
					'DATA_LENGTH': slots,
					'ID_LOCATION': location,
				};			
				var i;
				for (i = 1; i <= length; i++) { 
					dictionary["ITEM_" + i + "_ID"] = response.data.objects[i-1].id;
					dictionary["ITEM_" + i + "_NAME"] = response.data.objects[i-1].object_type.name;
				}			
				for (i = length + 1; i <= slots; i++) { 
					dictionary["ITEM_" + i + "_ID"] = 0;
					dictionary["ITEM_" + i + "_NAME"] = "empty";
				}				
				Pebble.sendAppMessage(dictionary);
				
				
				
				
				
      } else {
        console.log('Ha habido algun error al coger el objeto');
				console.log(req.responseText);
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
	enableHighAccuracy: true,
  'timeout': 15000,
  'maximumAge': 0 
};





// Called when JS is ready
Pebble.addEventListener("ready",
												function(e) {
													console.log("JS is ready");

													var http = new XMLHttpRequest();
													var params = '{ "username": "' + Pebble.getAccountToken() + '", "password": "' + Pebble.getWatchToken() + '", "appcode": "1234567890" }';
										//			var params = '{ "username": "' + 'api' + '", "password": "' + 'apipassword' + '", "appcode": "1234567890" }';
													
													
													var http2 = new XMLHttpRequest();
													var params2 = '{ "username": "' + Pebble.getAccountToken() + '", "password": "' + Pebble.getWatchToken() + '"}';
													
													http.open("POST", url + "login", true);

													//Send the proper header information along with the request
													http.setRequestHeader("Content-type", "application/json");
													http.setRequestHeader("Connection", "close");

													//Call a function when the state changes
													http.onload = function() {
														if (http.status == 401) {
														//registro
															
															
															http2.open("POST", url + "user", true);

															//Send the proper header information along with the request
															http2.setRequestHeader("Content-type", "application/json");
															http2.setRequestHeader("X-BAASBOX-APPCODE", "1234567890");
															http2.setRequestHeader("Connection", "close");
															http2.send(params2);
														}
														else { //login
															if(http.readyState == 4 && http.status == 200) {  
																//	console.log(http.responseText);
																var response = JSON.parse(http.responseText);
																console.log('X-BB-SESSION -> ' + response.data["X-BB-SESSION"]);
																session = response.data["X-BB-SESSION"];
																var dictionary = {
																	'DATA_TYPE': 0, //login exitoso
																	'DATA_LENGTH': 0,
																};			
																Pebble.sendAppMessage(dictionary);
															}
															else {
																console.log("Ha habido algun error en el login");
																console.log(http.responseText);
															}
														}
													};
													
													http2.onload = function() {
															if(http2.readyState == 4 && http2.status == 200) {  //depurar esto dando de baja al usuario, se va al else
																//	console.log(http.responseText);
																var response = JSON.parse(http2.responseText);
																console.log('X-BB-SESSION -> ' + response.data["X-BB-SESSION"]);
																session = response.data["X-BB-SESSION"];
																
															}
															else {
																console.log("Ha habido algun error en el registro");
																console.log(http2.responseText);
															}
														
													}

													http.send(params);
												});



Pebble.addEventListener('appmessage', function (e) {
  //Fetch venues
	if (e.payload.FETCH_TYPE == 0) window.navigator.geolocation.getCurrentPosition(locationSuccess, locationError, locationOptions);
	//Fetch items from venues
	if (e.payload.FETCH_TYPE == 1) fetchItems(e.payload.ID_VENUE);
	//pick item from location
	if (e.payload.FETCH_TYPE == 2) pickItem(e.payload.ID_ITEM,e.payload.ID_LOCATION);
	
//	console.log(JSON.stringify(e.payload));
  console.log(e.payload.FETCH_TYPE);
//  console.log('message!');
	
});

Pebble.addEventListener('webviewclosed', function (e) {
  console.log('webview closed');
  console.log(e.type);
  console.log(e.response);
});