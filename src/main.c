#include <pebble.h>
#include "action_menu.h"
	

		
#define DATA_TYPE			100
#define DATA_LENGTH		101
#define FETCH_TYPE		200
#define	ITEM_1_ID			1
#define	ITEM_1_NAME		11
#define	ITEM_2_ID			2
#define	ITEM_2_NAME		12
#define	ITEM_3_ID			3
#define	ITEM_3_NAME		13
#define	ITEM_4_ID			4
#define	ITEM_4_NAME		14
#define	ITEM_5_ID			5
#define	ITEM_5_NAME		15
#define	ITEM_6_ID			6
#define	ITEM_6_NAME		16
#define	ITEM_7_ID			7
#define	ITEM_7_NAME		17
#define	ITEM_8_ID			8
#define	ITEM_8_NAME		18
#define	ITEM_9_ID			9
#define	ITEM_9_NAME		19
#define	ITEM_10_ID		10
#define	ITEM_10_NAME	20
	

	
// Types of actions
typedef enum {
  ActionTypeEmpty,
  ActionTypeEnterVenue,
  ActionTypeEnterItem,
  ActionTypeGetItem,
  ActionTypeDropItem,
  ActionTypeLookItem
} ActionType;

// Struct with data about a menu action
typedef struct {
  ActionType type;
  char* id;
  char* name;
} Context;

// Main window variables
static Window *s_main_window;
static TextLayer *s_main_label_layer;
static ActionBarLayer *s_main_action_bar;

static ActionMenu *s_main_action_menu;
static ActionMenuLevel *s_main_root_level, *s_main_inventory_level, *s_main_venues_level;
static Context *s_main_action_data;

// Venue window variables
static Window *s_venue_window;
static TextLayer *s_venue_label_layer;
static ActionBarLayer *s_venue_action_bar;

static ActionMenu *s_venue_action_menu;
static ActionMenuLevel *s_venue_root_level, *s_venue_inventory_level, *s_venue_items_level;
static Context *s_venue_action_data;

// Item window variables
static Window *s_item_window;
static TextLayer *s_item_label_layer;
static ActionBarLayer *s_item_action_bar;

static ActionMenu *s_item_action_menu;
static ActionMenuLevel *s_item_root_level, *s_item_inventory_level, *s_item_items_level;
static Context *s_item_action_data;

// Other variables
static GBitmap *s_ellipsis_bitmap, *s_up_bitmap, *s_down_bitmap;

/****************************** Item  ActionMenu *****************************/

static void item_action_performed_callback(ActionMenu *action_menu, const ActionMenuItem *action, void *context) {
  // Get data from performed action
  s_item_action_data = action_menu_item_get_action_data(action);

//  APP_LOG(APP_LOG_LEVEL_INFO, "Type: %d, Id: %s, Name %s", s_item_action_data->type, s_item_action_data->id, s_item_action_data->name);
}

static void init_item_action_menu() {
  // Create the root level
  s_item_root_level = action_menu_level_create(2);
      
  action_menu_level_add_action(s_item_root_level, "Mirar", item_action_performed_callback, NULL);
  action_menu_level_add_action(s_item_root_level, "Dejar", item_action_performed_callback, NULL);
}

/******************************** Item  Clicks ********************************/

static void item_select_click_handler(ClickRecognizerRef recognizer, void *context) {
  // Configure the ActionMenu Window about to be shown
  ActionMenuConfig item_action_menu_config = (ActionMenuConfig) {
    .root_level = s_item_root_level,
    .colors = {
      #ifdef PBL_COLOR
      .background = GColorChromeYellow,
      .foreground = GColorBlack,
			#else
      .background = GColorWhite,
      .foreground = GColorBlack,
			#endif
    },
    .align = ActionMenuAlignCenter
  };

  // Show the ActionMenu
  s_item_action_menu = action_menu_open(&item_action_menu_config);
}

static void item_click_config_provider(void *context) {
  window_single_click_subscribe(BUTTON_ID_SELECT, item_select_click_handler);
}

/******************************* Item  Window ********************************/

static void item_window_load(Window *window) {
  // Let's use s_main_action_data as basic information (venue id, venue name)
  // More information to get from remote API like address and/or objects
  
  Layer *item_window_layer = window_get_root_layer(window);
  GRect bounds = layer_get_bounds(item_window_layer);

  s_item_action_bar = action_bar_layer_create();
  action_bar_layer_set_click_config_provider(s_item_action_bar, item_click_config_provider);
  action_bar_layer_set_icon(s_item_action_bar, BUTTON_ID_SELECT, s_ellipsis_bitmap);
  action_bar_layer_add_to_window(s_item_action_bar, window);

  s_item_label_layer = text_layer_create(GRect(0, 0, bounds.size.w - ACTION_BAR_WIDTH, bounds.size.h));
  text_layer_set_text(s_item_label_layer, s_main_action_data->name);
  text_layer_set_font(s_item_label_layer, fonts_get_system_font(FONT_KEY_GOTHIC_24_BOLD));
  text_layer_set_text_color(s_item_label_layer, GColorBlack);
  text_layer_set_background_color(s_item_label_layer, GColorClear);
  text_layer_set_text_alignment(s_item_label_layer, GTextAlignmentCenter);
  layer_add_child(item_window_layer, text_layer_get_layer(s_item_label_layer));
}

static void item_window_unload(Window *window) {
  text_layer_destroy(s_item_label_layer);
  action_bar_layer_destroy(s_item_action_bar);
  action_menu_unfreeze(s_main_action_menu);
}

/****************************** Venue  ActionMenu *****************************/

static void venue_action_performed_callback(ActionMenu *action_menu, const ActionMenuItem *action, void *context) {
  // Get data from performed action
  s_venue_action_data = action_menu_item_get_action_data(action);

  APP_LOG(APP_LOG_LEVEL_INFO, "Type: %d, Id: %s, Name %s", s_venue_action_data->type, s_venue_action_data->id, s_venue_action_data->name);
  switch(s_venue_action_data->type) {
    case ActionTypeGetItem:
    break;
  case ActionTypeDropItem:
    break;
  default:
    break;
  }

}

static void init_venue_action_menu() {
  // Create the root level
  s_venue_root_level = action_menu_level_create(2);
  
  // Get Items from remote API
  
  // Create both child levels for inventory and venues
  s_venue_inventory_level = action_menu_level_create(2);
  s_venue_items_level = action_menu_level_create(2);
  
  // Add items entries to be get
  action_menu_level_add_child(s_venue_root_level, s_venue_items_level, "Coger objeto");
  action_menu_level_add_action(s_venue_items_level, "10 Monedas", venue_action_performed_callback, 
                               &(Context){.type=ActionTypeGetItem,.id="0001",.name="10 Monedas"});
  action_menu_level_add_action(s_venue_items_level, "Cofre", venue_action_performed_callback, 
                               &(Context){.type=ActionTypeGetItem,.id="0003",.name="Cofre"});

  // Add inventory entries to be dropped
  action_menu_level_add_child(s_venue_root_level, s_venue_inventory_level, "Dejar objeto");
  action_menu_level_add_action(s_venue_inventory_level, "Dinero", venue_action_performed_callback, 
                               &(Context){.type=ActionTypeDropItem,.id="0001",.name="Dinero"});
  action_menu_level_add_action(s_venue_inventory_level, "Llave", venue_action_performed_callback, 
                               &(Context){.type=ActionTypeDropItem,.id="0002",.name="Llave"});

}

/******************************** Venue  Clicks ********************************/

static void venue_select_click_handler(ClickRecognizerRef recognizer, void *context) {
  // Configure the ActionMenu Window about to be shown
  ActionMenuConfig venue_action_menu_config = (ActionMenuConfig) {
    .root_level = s_venue_root_level,
    .colors = {
      #ifdef PBL_COLOR
      .background = GColorChromeYellow,
      .foreground = GColorBlack,
			#else
      .background = GColorWhite,
      .foreground = GColorBlack,
			#endif
    },
    .align = ActionMenuAlignCenter
  };

  // Show the ActionMenu
  s_venue_action_menu = action_menu_open(&venue_action_menu_config);
}

static void venue_click_config_provider(void *context) {
  window_single_click_subscribe(BUTTON_ID_SELECT, venue_select_click_handler);
}

/******************************* Venue  Window ********************************/

static void venue_window_load(Window *window) {
  // Let's use s_main_action_data as basic information (venue id, venue name)
  // More information to get from remote API like address and/or objects
  
  Layer *venue_window_layer = window_get_root_layer(window);
  GRect bounds = layer_get_bounds(venue_window_layer);

  s_venue_action_bar = action_bar_layer_create();
  action_bar_layer_set_click_config_provider(s_venue_action_bar, venue_click_config_provider);
  action_bar_layer_set_icon(s_venue_action_bar, BUTTON_ID_SELECT, s_ellipsis_bitmap);
  action_bar_layer_add_to_window(s_venue_action_bar, window);

  s_venue_label_layer = text_layer_create(GRect(0, 0, bounds.size.w - ACTION_BAR_WIDTH, bounds.size.h));
  text_layer_set_text(s_venue_label_layer, s_main_action_data->name);
  text_layer_set_font(s_venue_label_layer, fonts_get_system_font(FONT_KEY_GOTHIC_24_BOLD));
  text_layer_set_text_color(s_venue_label_layer, GColorBlack);
  text_layer_set_background_color(s_venue_label_layer, GColorClear);
  text_layer_set_text_alignment(s_venue_label_layer, GTextAlignmentCenter);
  layer_add_child(venue_window_layer, text_layer_get_layer(s_venue_label_layer));
}

static void venue_window_unload(Window *window) {
  text_layer_destroy(s_venue_label_layer);
  action_bar_layer_destroy(s_venue_action_bar);
  action_menu_unfreeze(s_main_action_menu);
}


/****************************** Request Data to JS ****************************************/

static void request_venues(void) {
		//vibes_short_pulse();
		DictionaryIterator *iter;
		app_message_outbox_begin(&iter);
		// Add a key-value pair
		dict_write_uint8(iter, FETCH_TYPE, 0); // 0 es el tipo "venue"
		// Send the message!
		app_message_outbox_send();
		APP_LOG(APP_LOG_LEVEL_INFO, "Venues solicitadas"); 
}




/******************************* Main ActionMenu ******************************/

static void main_action_performed_callback(ActionMenu *action_menu, const ActionMenuItem *action, void *context) {
  // Get data from performed action
  s_main_action_data = action_menu_item_get_action_data(action);
  APP_LOG(APP_LOG_LEVEL_INFO, "Type: %d, Id: %s, Name %s", s_main_action_data->type, s_main_action_data->id, s_main_action_data->name);

  switch (s_main_action_data->type) {
    case ActionTypeEnterVenue: 
      APP_LOG(APP_LOG_LEVEL_INFO, "Accediendo a la venue %s (id => %s)", s_main_action_data->name, s_main_action_data->id);
      action_menu_freeze(s_main_action_menu);
      window_stack_push(s_venue_window, true);

      break;
    case ActionTypeEnterItem: 
      APP_LOG(APP_LOG_LEVEL_INFO, "Accediendo al objeto %s (id => %s)", s_main_action_data->name, s_main_action_data->id);
      action_menu_freeze(s_main_action_menu);
      window_stack_push(s_item_window, true);
      break;
    default:
      break;
  }
}




static void init_main_action_menu() {
  // Create the root level
  s_main_root_level = action_menu_level_create(2);
  
  // Get Inventory/Venues from remote API
  
	
  // Create both child levels for inventory and venues
  s_main_inventory_level = action_menu_level_create(1);
  s_main_venues_level = action_menu_level_create(10);
  
  // Add venues entries
  action_menu_level_add_child(s_main_root_level, s_main_venues_level, "Búsqueda");
//  action_menu_level_add_action(s_main_venues_level, "StarBucks Coffee", main_action_performed_callback, 
//                               &(Context){.type=ActionTypeEnterVenue,.id="0001",.name="StarBucks Coffee"});
//  action_menu_level_add_action(s_main_venues_level, "Bar Tolo", main_action_performed_callback, 
//                               &(Context){.type=ActionTypeEnterVenue,.id="0002",.name="Bar Tolo"});

  // Add inventory entries
  action_menu_level_add_child(s_main_root_level, s_main_inventory_level, "Inventario");
  action_menu_level_add_action(s_main_inventory_level, "Dinero", main_action_performed_callback, 
                               &(Context){.type=ActionTypeEnterItem,.id="0001",.name="Dinero"});
  action_menu_level_add_action(s_main_inventory_level, "Llave", main_action_performed_callback, 
                               &(Context){.type=ActionTypeEnterItem,.id="0002",.name="Llave"});
	//action_menu_freeze(s_main_action_menu);
}

/********************************* Main Clicks *********************************/

static void main_select_click_handler(ClickRecognizerRef recognizer, void *context) {
	request_venues();
	
  // Configure the ActionMenu Window about to be shown
  ActionMenuConfig main_action_menu_config = (ActionMenuConfig) {
    .root_level = s_main_root_level,
    .colors = {
      #ifdef PBL_COLOR
      .background = GColorChromeYellow,
      .foreground = GColorBlack,
			#else
      .background = GColorWhite,
      .foreground = GColorBlack,
			#endif
    },
    .align = ActionMenuAlignCenter
  };

  // Show the ActionMenu
	s_main_action_menu = action_menu_open(&main_action_menu_config);
	action_menu_freeze(s_main_action_menu);
}

static void main_click_config_provider(void *context) {
  window_single_click_subscribe(BUTTON_ID_SELECT, main_select_click_handler);
}

/******************************** Main Window *********************************/

static void main_window_load(Window *window) {
  Layer *main_window_layer = window_get_root_layer(window);
  GRect bounds = layer_get_bounds(main_window_layer);

  s_ellipsis_bitmap = gbitmap_create_with_resource(RESOURCE_ID_ELLIPSIS);
  //s_up_bitmap = gbitmap_create_with_resource(RESOURCE_ID_UP);
  //s_down_bitmap = gbitmap_create_with_resource(RESOURCE_ID_DOWN);

  s_main_action_bar = action_bar_layer_create();
  action_bar_layer_set_click_config_provider(s_main_action_bar, main_click_config_provider);
  action_bar_layer_set_icon(s_main_action_bar, BUTTON_ID_SELECT, s_ellipsis_bitmap);
  action_bar_layer_add_to_window(s_main_action_bar, window);

  s_main_label_layer = text_layer_create(GRect(0, 0, bounds.size.w - ACTION_BAR_WIDTH, bounds.size.h));
  text_layer_set_text(s_main_label_layer, "Bienvenido a GeoCraft");
  text_layer_set_font(s_main_label_layer, fonts_get_system_font(FONT_KEY_GOTHIC_24_BOLD));
  text_layer_set_text_color(s_main_label_layer, GColorBlack);
  text_layer_set_background_color(s_main_label_layer, GColorClear);
  text_layer_set_text_alignment(s_main_label_layer, GTextAlignmentCenter);
  layer_add_child(main_window_layer, text_layer_get_layer(s_main_label_layer));
}

static void main_window_unload(Window *window) {
  text_layer_destroy(s_main_label_layer);
  action_bar_layer_destroy(s_main_action_bar);
  gbitmap_destroy(s_ellipsis_bitmap);

  action_menu_hierarchy_destroy(s_main_root_level, NULL, NULL);
}


/********************************* Messages and received handlers ***********************************/

static void in_received_handler(DictionaryIterator *iter, void *context) 
{
	vibes_short_pulse();
	APP_LOG(APP_LOG_LEVEL_INFO, "Received handler"); 
//	(void) context;
   //Get data
	Tuple *length_tuple = dict_find(iter,DATA_LENGTH);
	//APP_LOG(APP_LOG_LEVEL_ERROR, length_tuple->value->int32);
	int length = length_tuple->value->int32;
//	s_root_level = action_menu_level_create(length);
	
	for (int i = 1; i <= (length); i ++) {  //rellenamos los nombres de las venues
		Tuple *venue_tuple = dict_find(iter,i + 10);		
		Tuple *id_tuple = dict_find(iter,i);	
		action_menu_level_add_action(s_main_venues_level, venue_tuple->value->cstring, main_action_performed_callback, 
                               &(Context){.type=ActionTypeEnterVenue,.id=id_tuple->value->cstring,.name=venue_tuple->value->cstring});

		
		
	}
	action_menu_unfreeze(s_main_action_menu);
	
}


static void inbox_dropped_callback(AppMessageResult reason, void *context) {
  APP_LOG(APP_LOG_LEVEL_ERROR, "Message dropped!");
}

static void outbox_failed_callback(DictionaryIterator *iterator, AppMessageResult reason, void *context) {
  APP_LOG(APP_LOG_LEVEL_ERROR, "Outbox send failed!");
}

static void outbox_sent_callback(DictionaryIterator *iterator, void *context) {
  APP_LOG(APP_LOG_LEVEL_INFO, "Outbox send success!");
}

/************************************ App *************************************/

static void init() {
  // Main window creation
  s_main_window = window_create();
  #ifdef PBL_COLOR
  	window_set_background_color(s_main_window, GColorChromeYellow);
	#else
  	window_set_background_color(s_main_window, GColorWhite);
	#endif
  window_set_window_handlers(s_main_window, (WindowHandlers) {
    .load = main_window_load,
    .unload = main_window_unload,
  });
  
  // Venue window creation
  s_venue_window = window_create();
  #ifdef PBL_COLOR
  	window_set_background_color(s_main_window, GColorChromeYellow);
	#else
  	window_set_background_color(s_main_window, GColorWhite);
	#endif
  window_set_window_handlers(s_venue_window, (WindowHandlers) {
    .load = venue_window_load,
    .unload = venue_window_unload,
  });
  
  // Item window creation
  s_item_window = window_create();
  #ifdef PBL_COLOR
  	window_set_background_color(s_main_window, GColorChromeYellow);
	#else
  	window_set_background_color(s_main_window, GColorWhite);
	#endif
  window_set_window_handlers(s_item_window, (WindowHandlers) {
    .load = item_window_load,
    .unload = item_window_unload,
  });
  
  // Show main window
  window_stack_push(s_main_window, true);
	
	// Register callbacks
	app_message_register_inbox_received((AppMessageInboxReceived) in_received_handler);
	app_message_register_inbox_dropped(inbox_dropped_callback);
	app_message_register_outbox_failed(outbox_failed_callback);
	app_message_register_outbox_sent(outbox_sent_callback);
	app_message_open(app_message_inbox_size_maximum(), app_message_outbox_size_maximum());
    
  init_main_action_menu();
  init_venue_action_menu();
  init_item_action_menu();
}

static void deinit() {
  window_destroy(s_main_window);
}

int main() {
  init();
  app_event_loop();
  deinit();
}
