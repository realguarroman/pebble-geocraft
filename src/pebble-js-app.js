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
					dictionary["ITEM_" + i + "_ICON"] = response.data.objects[i-1].object_type.index;
					dictionary["ITEM_" + i + "_CAT"] = response.data.objects[i-1].object_category.index;
				}			
				for (i = length + 1; i <= slots; i++) { 
					dictionary["ITEM_" + i + "_ID"] = 0;
					dictionary["ITEM_" + i + "_NAME"] = "empty";
					dictionary["ITEM_" + i + "_ICON"] = 0;
					dictionary["ITEM_" + i + "_CAT"] = -1;
				}				
				Pebble.sendAppMessage(dictionary);
      } else {
        console.log('Error');
      }
    }
  };
  req.send(null);
}



function pickItem(object_id, object_name, location_id, location_name) {
  console.log(object_id);
  console.log(object_name);
  console.log(location_id);
  console.log(location_name);
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
					dictionary["ITEM_" + i + "_ICON"] = response.data.objects[i-1].object_type.index;
					dictionary["ITEM_" + i + "_CAT"] = response.data.objects[i-1].object_category.index;
				}			
				for (i = length + 1; i <= slots; i++) { 
					dictionary["ITEM_" + i + "_ID"] = 0;
					dictionary["ITEM_" + i + "_NAME"] = "empty";
					dictionary["ITEM_" + i + "_ICON"] = 0;
					dictionary["ITEM_" + i + "_CAT"] = -1;
				}				
      
        // Create the pin
        var date = new Date();
        date.setHours(date.getHours());
        var pin = {
          "id": "geocraft-pick-item-" + (date.getTime() / 1000 | 0),
          "time": date.toISOString(),
          "layout": {
            "type": "genericPin",
            "title": "You picked up " + object_name,
            "tinyIcon": "system://images/GENERIC_CONFIRMATION"
          }
        };
        console.log('Inserting pin in the future: ' + JSON.stringify(pin));
        // Push the pin
        insertUserPin(pin, function(responseText) { 
          console.log('Result: ' + responseText);
        });
      
        
        Pebble.sendAppMessage(dictionary);
				
				
				
				
				
      } else {
        console.log('Ha habido algun error al coger el objeto');
				console.log(req.responseText);
      }
    }
  };
  req.send(null);
}







function inventory() {
  var req = new XMLHttpRequest();
	console.log(url + 'plugin/geocraft.view.user?X-BB-SESSION=' + session);
  req.open('GET', url + 'plugin/geocraft.view.user?X-BB-SESSION=' + session, true);
	req.onload = function () {
    if (req.readyState === 4) {
			if (req.status === 200) {
        console.log(req.responseText);
        var response = JSON.parse(req.responseText);
				var length =  response.data.visibleByTheUser.objects.length;			
				
		//		if (length > 10) length = 10; limitamos el inventario?
				// Assemble dictionary using our keys
				if (length > 10) length = 10;
				var dictionary = {
					'DATA_TYPE': 4, //inventory
					'DATA_LENGTH': length,
				};			
				var i;
				for (i = 1; i <= length; i++) { 
					dictionary["ITEM_" + i + "_ID"] = response.data.visibleByTheUser.objects[i-1].id;
					dictionary["ITEM_" + i + "_NAME"] = response.data.visibleByTheUser.objects[i-1].object_type.name;
					dictionary["ITEM_" + i + "_ICON"] = response.data.visibleByTheUser.objects[i-1].object_type.index;
					dictionary["ITEM_" + i + "_CAT"] = response.data.visibleByTheUser.objects[i-1].object_category.index;

				}			
				Pebble.sendAppMessage(dictionary);
      } else {
        console.log('Error');
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
	if (e.payload.FETCH_TYPE == 2) pickItem(e.payload.ID_ITEM,e.payload.NAME_ITEM,e.payload.ID_LOCATION,e.payload.NAME_LOCATION);
	//inventory
	if (e.payload.FETCH_TYPE == 3) inventory();
	
//	console.log(JSON.stringify(e.payload));
  console.log(e.payload.FETCH_TYPE);
//  console.log('message!');
	
});

Pebble.addEventListener('webviewclosed', function (e) {
  console.log('webview closed');
  console.log(e.type);
  console.log(e.response);
});

/******************************* timeline lib *********************************/

// The timeline public URL root
var API_URL_ROOT = 'https://timeline-api.getpebble.com/';

/**
 * Send a request to the Pebble public web timeline API.
 * @param pin The JSON pin to insert. Must contain 'id' field.
 * @param type The type of request, either PUT or DELETE.
 * @param topics Array of topics if a shared pin, 'null' otherwise.
 * @param apiKey Timeline API key for this app, available from dev-portal.getpebble.com
 * @param callback The callback to receive the responseText after the request has completed.
 */
function timelineRequest(pin, type, topics, apiKey, callback) {
  // User or shared?
  var url = API_URL_ROOT + 'v1/' + ((topics != null) ? 'shared/' : 'user/') + 'pins/' + pin.id;

  // Create XHR
  var xhr = new XMLHttpRequest();
  xhr.onload = function () {
    console.log('timeline: response received: ' + this.responseText);
    callback(this.responseText);
  };
  xhr.open(type, url);

  // Set headers
  xhr.setRequestHeader('Content-Type', 'application/json');
  if(topics != null) {
    xhr.setRequestHeader('X-Pin-Topics', '' + topics.join(','));
    xhr.setRequestHeader('X-API-Key', '' + apiKey);
  }

  // Get token
  Pebble.getTimelineToken(function(token) {
    // Add headers
    xhr.setRequestHeader('X-User-Token', '' + token);

    // Send
    xhr.send(JSON.stringify(pin));
    console.log('timeline: request sent.');
  }, function(error) { console.log('timeline: error getting timeline token: ' + error); });
}

/**
 * Insert a pin into the timeline for this user.
 * @param pin The JSON pin to insert.
 * @param callback The callback to receive the responseText after the request has completed.
 */
function insertUserPin(pin, callback) {
  timelineRequest(pin, 'PUT', null, null, callback);
}

/**
 * Delete a pin from the timeline for this user.
 * @param pin The JSON pin to delete.
 * @param callback The callback to receive the responseText after the request has completed.
 */
function deleteUserPin(pin, callback) {
  timelineRequest(pin, 'DELETE', null, null, callback);
}

/**
 * Insert a pin into the timeline for these topics.
 * @param pin The JSON pin to insert.
 * @param topics Array of topics to insert pin to.
 * @param apiKey Timeline API key for this app, available from dev-portal.getpebble.com
 * @param callback The callback to receive the responseText after the request has completed.
 */
function insertSharedPin(pin, topics, apiKey, callback) {
  timelineRequest(pin, 'PUT', topics, apiKey, callback);
}

/**
 * Delete a pin from the timeline for these topics.
 * @param pin The JSON pin to delete.
 * @param topics Array of topics to delete pin from.
 * @param apiKey Timeline API key for this app, available from dev-portal.getpebble.com
 * @param callback The callback to receive the responseText after the request has completed.
 */
function deleteSharedPin(pin, topics, apiKey, callback) {
  timelineRequest(pin, 'DELETE', topics, apiKey, callback);
}

/****************************** end timeline lib ******************************/