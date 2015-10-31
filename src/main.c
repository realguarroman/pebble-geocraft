#include <pebble.h>
#include "action_menu.h"
	

		
#define DATA_TYPE			100
#define DATA_LENGTH		101
#define FETCH_TYPE		200
#define ID_VENUE  		201
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
	

#define ANIM_DURATION 250
	


#define OFFSET_Y 168


	




#define	MAX_STRINGS 10
#define	MAX_STRING_SIZE	50
	

#ifdef PBL_PLATFORM_CHALK

	#define LABEL_NAME_X 35
	#define LABEL_NAME_Y 15
	#define LABEL_NAME_WIDTH 103
	#define LABEL_NAME_HEIGHT 75

  #define LABEL_ICON_X 35
	#define LABEL_ICON_Y 93
	#define LABEL_ICON_WIDTH 56
	#define LABEL_ICON_HEIGHT 65

	#define LABEL_PAGINATION_X 93
	#define LABEL_PAGINATION_Y 130
	#define LABEL_PAGINATION_WIDTH 50
	#define LABEL_PAGINATION_HEIGHT 25

#else

	#define LABEL_NAME_X 5
	#define LABEL_NAME_Y 0
	#define LABEL_NAME_WIDTH 103
	#define LABEL_NAME_HEIGHT 75

	#define LABEL_ICON_X 5
	#define LABEL_ICON_Y 93
	#define LABEL_ICON_WIDTH 56
	#define LABEL_ICON_HEIGHT 65

	#define LABEL_PAGINATION_X 63
	#define LABEL_PAGINATION_Y 130
	#define LABEL_PAGINATION_WIDTH 50
	#define LABEL_PAGINATION_HEIGHT 25

#endif


// Types of actions
typedef enum {
  ActionTypeEmpty,
	ActionTypeFetchVenues,
	ActionTypeFetchItems,
	ActionTypeShowInventory,
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


char venues_names[MAX_STRINGS][MAX_STRING_SIZE]; 
char venues_ids[MAX_STRINGS][MAX_STRING_SIZE]; 

char items_names[MAX_STRINGS][MAX_STRING_SIZE]; 
char items_ids[MAX_STRINGS][MAX_STRING_SIZE]; 

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
static BitmapLayer *s_venue_icon_layer;
static TextLayer *s_venue_pagination_layer;




static ActionMenu *s_venue_action_menu;
static ActionMenuLevel *s_venue_root_level, *s_venue_inventory_level, *s_venue_items_level;
static Context *s_venue_action_data;

// Item window variables
static Window *s_item_window;
static TextLayer *s_item_label_layer;
static ActionBarLayer *s_item_action_bar;
static BitmapLayer *s_item_icon_layer;
static TextLayer *s_item_pagination_layer;

static ActionMenu *s_item_action_menu;
static ActionMenuLevel *s_item_root_level, *s_item_inventory_level, *s_item_items_level;
static Context *s_item_action_data;

// Other variables
static GBitmap *s_ellipsis_bitmap, *s_up_bitmap, *s_down_bitmap, *s_logoFSQ_bitmap, *s_icon_bitmap;
static BitmapLayer *s_logoFSQ_layer;
static bool LOGGED_IN = false;
static int s_active_venue;
static int s_active_item;
static int s_venue_length;
static int s_item_length;

static bool on_animation;

//temporizadores
AppTimer *timer;
AppTimer *timer2;
AppTimer *timer3;

/****************************** Animation *************************************************/

void on_animation_stopped(Animation *anim, bool finished, void *context)
{
    //Free the memoery used by the Animation
    property_animation_destroy((PropertyAnimation*) anim);
	  
	
}
 
void animate_layer(Layer *layer, GRect *start, GRect *finish, int duration, int delay)
{
		
    //Declare animation
    PropertyAnimation *anim = property_animation_create_layer_frame(layer, start, finish);
 
    //Set characteristics
    animation_set_duration((Animation*) anim, duration);
    animation_set_delay((Animation*) anim, delay);
 
    //Set stopped handler to free memory
    AnimationHandlers handlers = {
        //The reference to the stopped handler is the only one in the array
        .stopped = (AnimationStoppedHandler) on_animation_stopped
    };
    animation_set_handlers((Animation*) anim, handlers, NULL);
 
    //Start animation!
    animation_schedule((Animation*) anim);
}




/****************************** Request Data to JS ****************************************/

static void request_venues(void) {
		//vibes_short_pulse();
		DictionaryIterator *iter;
		app_message_outbox_begin(&iter);
		// Add a key-value pair
		dict_write_uint8(iter, FETCH_TYPE, 0); // 0 es buscar Venues
		// Send the message!
		app_message_outbox_send();
		APP_LOG(APP_LOG_LEVEL_INFO, "Venues solicitadas"); 
}

static void request_items(char* id_venue) {
		//vibes_short_pulse();
		DictionaryIterator *iter;
		app_message_outbox_begin(&iter);
		// Add a key-value pair
		dict_write_uint8(iter, FETCH_TYPE, 1); // 1 es buscar ítems
  	dict_write_cstring(iter, ID_VENUE, id_venue); 
		// Send the message!
		app_message_outbox_send();
		APP_LOG(APP_LOG_LEVEL_INFO, "Ítems solicitados"); 
}




/********************* ANimations *******************/

void animate_label_up_first(Layer *l) {
	GRect a = layer_get_frame(l);
	GRect b = GRect(a.origin.x,a.origin.y + OFFSET_Y + 1,a.size.w,a.size.h);
  animate_layer(l, &a, &b, ANIM_DURATION, 0);

}

void animate_label_up_last(Layer *l) {
	GRect a = layer_get_frame(l);
	GRect b = GRect(a.origin.x,a.origin.x - OFFSET_Y - OFFSET_Y,a.size.w,a.size.h);
	GRect c = GRect(a.origin.x,a.origin.y - OFFSET_Y,a.size.w,a.size.h);
  animate_layer(l, &b, &c, ANIM_DURATION, 0);
}
	
void animate_label_down_first(Layer *l) {
	GRect a = layer_get_frame(l);
	GRect b = GRect(a.origin.x,a.origin.y - OFFSET_Y - 1,a.size.w,a.size.h);
  animate_layer(l, &a, &b, ANIM_DURATION, 0);
}

void animate_label_down_last(Layer *l) {
	GRect a = layer_get_frame(l);
	GRect b = GRect(a.origin.x,a.origin.y + OFFSET_Y + OFFSET_Y,a.size.w,a.size.h);
	GRect c = GRect(a.origin.x,a.origin.y + OFFSET_Y,a.size.w,a.size.h);
  animate_layer(l, &b, &c, ANIM_DURATION, 0);
}


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


/********************************* Update  Layers **********************************************/

static void update_venue_layers (int id) {
	static char pagination[] = "10/10";
	snprintf(pagination, sizeof(pagination), "%d/%d", s_active_venue+1, s_venue_length);
	text_layer_set_text(s_venue_label_layer, venues_names[id]);
	text_layer_set_text(s_venue_pagination_layer, pagination);
	
	gbitmap_destroy(s_icon_bitmap);
	
	int question=rand()%7;
	switch(question)
	{
		case 0:
		s_icon_bitmap = gbitmap_create_with_resource(RESOURCE_ID_VENUE0);
		break;
		case 1:
		s_icon_bitmap = gbitmap_create_with_resource(RESOURCE_ID_VENUE1);
		break;
		case 2:
		s_icon_bitmap = gbitmap_create_with_resource(RESOURCE_ID_VENUE2);
		break;
		case 3:
		s_icon_bitmap = gbitmap_create_with_resource(RESOURCE_ID_VENUE3);
		break;
		case 4:
		s_icon_bitmap = gbitmap_create_with_resource(RESOURCE_ID_VENUE4);
		break;
		case 5:
		s_icon_bitmap = gbitmap_create_with_resource(RESOURCE_ID_VENUE5);
		break;
		case 6:
		s_icon_bitmap = gbitmap_create_with_resource(RESOURCE_ID_VENUE6);
		break;
	
	}
	bitmap_layer_set_bitmap(s_venue_icon_layer, s_icon_bitmap);
}

static void update_item_layers (int id) {
	static char pagination[] = "10/10";
	snprintf(pagination, sizeof(pagination), "%d/%d", s_active_item+1, s_item_length);
	text_layer_set_text(s_item_label_layer, items_names[id]);
	text_layer_set_text(s_item_pagination_layer, pagination);
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






void timer2_callback_up(void *data) {
	update_item_layers(s_active_item);
	animate_label_up_last(text_layer_get_layer(s_item_label_layer));
	animate_label_up_last(text_layer_get_layer(s_item_pagination_layer));
}
void timer2_callback_down(void *data) {
	update_item_layers(s_active_item);
	animate_label_down_last(text_layer_get_layer(s_item_label_layer));
	animate_label_down_last(text_layer_get_layer(s_item_pagination_layer));
}
void timer3_callback(void *data) {
	on_animation=false;
}
	
static void item_select_up_handler(ClickRecognizerRef recognizer, void *context) {
	if (on_animation == false) {
		if ((s_active_item>=1)) {
			on_animation = true;
			timer3 = app_timer_register(ANIM_DURATION * 2, (AppTimerCallback) timer3_callback, NULL);
			animate_label_up_first(text_layer_get_layer(s_item_label_layer));
			animate_label_up_first(text_layer_get_layer(s_item_pagination_layer));
			s_active_item=s_active_item-1;
			timer = app_timer_register(ANIM_DURATION, (AppTimerCallback) timer2_callback_up, NULL);
		}
	}
}

static void item_select_down_handler(ClickRecognizerRef recognizer, void *context) {
	if (on_animation == false) {
		if (s_active_item<(s_item_length-1)) {
			on_animation = true;
			timer3 = app_timer_register(ANIM_DURATION * 2, (AppTimerCallback) timer3_callback, NULL);
			animate_label_down_first(text_layer_get_layer(s_item_label_layer));
			animate_label_down_first(text_layer_get_layer(s_item_pagination_layer));
			s_active_item=s_active_item+1;
			timer2 = app_timer_register(ANIM_DURATION, (AppTimerCallback) timer2_callback_down, NULL);
		}
	}
}


static void item_click_config_provider(void *context) {
  window_single_click_subscribe(BUTTON_ID_SELECT, item_select_click_handler);
	window_single_click_subscribe(BUTTON_ID_UP, item_select_up_handler);
	window_single_click_subscribe(BUTTON_ID_DOWN, item_select_down_handler);
}





/******************************* Item  Window ********************************/

static void item_window_load(Window *window) {

  Layer *item_window_layer = window_get_root_layer(window);

  s_item_action_bar = action_bar_layer_create();
  action_bar_layer_set_click_config_provider(s_item_action_bar, item_click_config_provider);
  action_bar_layer_set_icon(s_item_action_bar, BUTTON_ID_SELECT, s_ellipsis_bitmap);
	action_bar_layer_set_icon(s_item_action_bar, BUTTON_ID_UP, s_up_bitmap);
	action_bar_layer_set_icon(s_item_action_bar, BUTTON_ID_DOWN, s_down_bitmap);
  action_bar_layer_add_to_window(s_item_action_bar, window);

 
  s_item_label_layer = text_layer_create(GRect(LABEL_NAME_X,LABEL_NAME_Y,LABEL_NAME_WIDTH,LABEL_NAME_HEIGHT));
  text_layer_set_text(s_item_label_layer, "");
  text_layer_set_font(s_item_label_layer, fonts_get_system_font(FONT_KEY_GOTHIC_28_BOLD));
  text_layer_set_text_color(s_item_label_layer, GColorBlack);
	
	

		
		
  text_layer_set_text_alignment(s_item_label_layer, GTextAlignmentLeft);
  layer_add_child(item_window_layer, text_layer_get_layer(s_item_label_layer));
	
	s_item_pagination_layer = text_layer_create(GRect(LABEL_PAGINATION_X,LABEL_PAGINATION_Y,LABEL_PAGINATION_WIDTH,LABEL_PAGINATION_HEIGHT));
  text_layer_set_text(s_item_pagination_layer, "10/10");
  text_layer_set_font(s_item_pagination_layer, fonts_get_system_font(FONT_KEY_GOTHIC_24_BOLD));
  text_layer_set_text_color(s_item_pagination_layer, GColorBlack);
  text_layer_set_text_alignment(s_item_pagination_layer, GTextAlignmentCenter);
  layer_add_child(item_window_layer, text_layer_get_layer(s_item_pagination_layer));
	
	#ifdef PBL_COLOR
		window_set_background_color(window, GColorCyan);
		text_layer_set_background_color(s_item_label_layer, GColorCyan);
		text_layer_set_background_color(s_item_pagination_layer, GColorCyan);
	#else
  #endif
	

}

static void item_window_unload(Window *window) {
  text_layer_destroy(s_item_label_layer);
  action_bar_layer_destroy(s_item_action_bar);
  action_menu_unfreeze(s_main_action_menu);
}



/******************************** Venue  Clicks ********************************/

static void venue_select_click_handler(ClickRecognizerRef recognizer, void *context) {
  request_items(venues_ids[s_active_venue]);
}



void timer_callback_up(void *data) {
	update_venue_layers(s_active_venue);
	animate_label_up_last(text_layer_get_layer(s_venue_label_layer));
	animate_label_up_last(text_layer_get_layer(s_venue_pagination_layer));
	animate_label_up_last(bitmap_layer_get_layer(s_venue_icon_layer));
}
void timer_callback_down(void *data) {
	update_venue_layers(s_active_venue);
	animate_label_down_last(text_layer_get_layer(s_venue_label_layer));
	animate_label_down_last(text_layer_get_layer(s_venue_pagination_layer));
	animate_label_down_last(bitmap_layer_get_layer(s_venue_icon_layer));
}
	
static void venue_select_up_handler(ClickRecognizerRef recognizer, void *context) {
	if (on_animation == false) {
		if (s_active_venue>=1) {
			on_animation = true;
			timer3 = app_timer_register(ANIM_DURATION * 2, (AppTimerCallback) timer3_callback, NULL);
			animate_label_up_first(text_layer_get_layer(s_venue_label_layer));
			animate_label_up_first(text_layer_get_layer(s_venue_pagination_layer));
			animate_label_up_first(bitmap_layer_get_layer(s_venue_icon_layer));
			s_active_venue=s_active_venue-1;
			timer = app_timer_register(ANIM_DURATION, (AppTimerCallback) timer_callback_up, NULL);	
		}
	}
 	
}

static void venue_select_down_handler(ClickRecognizerRef recognizer, void *context) {
	if (on_animation == false) {
		if (s_active_venue<(s_venue_length-1)) {
			on_animation = true;
			timer3 = app_timer_register(ANIM_DURATION * 2, (AppTimerCallback) timer3_callback, NULL);
			animate_label_down_first(text_layer_get_layer(s_venue_label_layer));
			animate_label_down_first(text_layer_get_layer(s_venue_pagination_layer));
			animate_label_down_first(bitmap_layer_get_layer(s_venue_icon_layer));
			s_active_venue=s_active_venue+1;
			timer = app_timer_register(ANIM_DURATION, (AppTimerCallback) timer_callback_down, NULL);
		}
	}
}

static void venue_click_config_provider(void *context) {
  window_single_click_subscribe(BUTTON_ID_SELECT, venue_select_click_handler);
	window_single_click_subscribe(BUTTON_ID_UP, venue_select_up_handler);
	window_single_click_subscribe(BUTTON_ID_DOWN, venue_select_down_handler);
}

/******************************* Venue  Window ********************************/

static void venue_window_load(Window *window) {
  Layer *venue_window_layer = window_get_root_layer(window);

  s_venue_action_bar = action_bar_layer_create();
  action_bar_layer_set_click_config_provider(s_venue_action_bar, venue_click_config_provider);
  action_bar_layer_set_icon(s_venue_action_bar, BUTTON_ID_SELECT, s_ellipsis_bitmap);
	action_bar_layer_set_icon(s_venue_action_bar, BUTTON_ID_UP, s_up_bitmap);
	action_bar_layer_set_icon(s_venue_action_bar, BUTTON_ID_DOWN, s_down_bitmap);
  action_bar_layer_add_to_window(s_venue_action_bar, window);

  s_venue_label_layer = text_layer_create(GRect(LABEL_NAME_X,LABEL_NAME_Y,LABEL_NAME_WIDTH,LABEL_NAME_HEIGHT));
  text_layer_set_text(s_venue_label_layer, "Universidad Politecnica de Valencia");
  text_layer_set_font(s_venue_label_layer, fonts_get_system_font(FONT_KEY_GOTHIC_24_BOLD));
  text_layer_set_text_color(s_venue_label_layer, GColorBlack);
  text_layer_set_text_alignment(s_venue_label_layer, GTextAlignmentLeft);
  layer_add_child(venue_window_layer, text_layer_get_layer(s_venue_label_layer));
	
	s_venue_pagination_layer = text_layer_create(GRect(LABEL_PAGINATION_X,LABEL_PAGINATION_Y,LABEL_PAGINATION_WIDTH,LABEL_PAGINATION_HEIGHT));
  text_layer_set_text(s_venue_pagination_layer, "10/10");
  text_layer_set_font(s_venue_pagination_layer, fonts_get_system_font(FONT_KEY_GOTHIC_24_BOLD));
  text_layer_set_text_color(s_venue_pagination_layer, GColorBlack);
  text_layer_set_text_alignment(s_venue_pagination_layer, GTextAlignmentCenter);
  layer_add_child(venue_window_layer, text_layer_get_layer(s_venue_pagination_layer));
	
	
	s_venue_icon_layer= bitmap_layer_create(GRect(LABEL_ICON_X, LABEL_ICON_Y, LABEL_ICON_WIDTH, LABEL_ICON_HEIGHT));
	s_icon_bitmap = gbitmap_create_with_resource(RESOURCE_ID_VENUE2);
	bitmap_layer_set_bitmap(s_venue_icon_layer, s_icon_bitmap);
	layer_add_child(venue_window_layer, bitmap_layer_get_layer(s_venue_icon_layer));
	
	#ifdef PBL_COLOR
		window_set_background_color(window, GColorYellow);
		text_layer_set_background_color(s_venue_label_layer, GColorYellow);
		text_layer_set_background_color(s_venue_pagination_layer, GColorYellow);
	#else
  #endif
}

static void venue_window_unload(Window *window) {
  text_layer_destroy(s_venue_label_layer);
  action_bar_layer_destroy(s_venue_action_bar);
  action_menu_unfreeze(s_main_action_menu);
}






/******************************* Main ActionMenu ******************************/

static void main_action_performed_callback(ActionMenu *action_menu, const ActionMenuItem *action, void *context) {
  // Get data from performed action
  s_main_action_data = action_menu_item_get_action_data(action);
  APP_LOG(APP_LOG_LEVEL_INFO, "Type: %d, Id: %s, Name %s", s_main_action_data->type, s_main_action_data->id, s_main_action_data->name);
  switch (s_main_action_data->type) {
		
		 case ActionTypeFetchVenues: 
      request_venues();
			action_menu_freeze(s_main_action_menu); 
      break;
		
		
		 case ActionTypeFetchItems: 
     // request_items(s_main_action_data->id);
		//	action_menu_freeze(s_venue_action_menu); 
      break;
		
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
  
  
	action_menu_level_add_action (s_main_root_level, "Búsqueda", main_action_performed_callback,
															  &(Context){.type=ActionTypeFetchVenues,.id="busqueda_id",.name="busqueda_name"});
	action_menu_level_add_action (s_main_root_level, "Inventario", main_action_performed_callback,
															  &(Context){.type=ActionTypeShowInventory,.id="inventario_id",.name="inventario_name"});
	
	
 
}

/********************************* Main Clicks *********************************/

static void main_select_click_handler(ClickRecognizerRef recognizer, void *context) {
	//request_venues();
	if (LOGGED_IN) {
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
	}
}

static void main_click_config_provider(void *context) {
  window_single_click_subscribe(BUTTON_ID_SELECT, main_select_click_handler);
}

/******************************** Main Window *********************************/

static void main_window_load(Window *window) {
  Layer *main_window_layer = window_get_root_layer(window);
  GRect bounds = layer_get_bounds(main_window_layer);

  s_ellipsis_bitmap = gbitmap_create_with_resource(RESOURCE_ID_ARROWS);	
  s_up_bitmap = gbitmap_create_with_resource(RESOURCE_ID_ICON_UP);
  s_down_bitmap = gbitmap_create_with_resource(RESOURCE_ID_ICON_DOWN);

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
	
	

	
	
//static BitmapLayer *s_icon_layer;
//static ActionBarLayer *s_item_bar;
	

	
	
	//logo FSQ
	s_logoFSQ_bitmap = gbitmap_create_with_resource(RESOURCE_ID_logoFSQ_BLACK);
	s_logoFSQ_layer= bitmap_layer_create(GRect(0, 145, 144, 27));
	bitmap_layer_set_bitmap(s_logoFSQ_layer, s_logoFSQ_bitmap);
	layer_add_child(main_window_layer, bitmap_layer_get_layer(s_logoFSQ_layer));
	
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
	
	
	APP_LOG(APP_LOG_LEVEL_INFO, "Received handler"); 
	
	

	
	
	Tuple *length_tuple = dict_find(iter,DATA_LENGTH);
	int length = length_tuple->value->int32;
	Tuple *type_tuple = dict_find(iter,DATA_TYPE);
	
	//APP_LOG(APP_LOG_LEVEL_INFO, "Tipo de datos recibidos: %i", type_tuple->value->int32); 

	switch (type_tuple->value->int32) {
		
		case 0:
			APP_LOG(APP_LOG_LEVEL_INFO, "Login exitoso"); 
			LOGGED_IN = true;
			gbitmap_destroy(s_ellipsis_bitmap);
			s_ellipsis_bitmap = gbitmap_create_with_resource(RESOURCE_ID_ELLIPSIS);
	  	action_bar_layer_set_icon(s_main_action_bar, BUTTON_ID_SELECT, s_ellipsis_bitmap);
		break;
		
		case 1:    		
			APP_LOG(APP_LOG_LEVEL_INFO, "Recibidas las venues en el Pebble"); 
			for (int i = 1; i <= (length); i ++) {  //rellenamos los nombres de las venues
				strcpy(venues_ids[i-1], dict_find(iter,i)->value->cstring);
				strcpy(venues_names[i-1], dict_find(iter,i+10)->value->cstring);				
			}
			action_menu_unfreeze(s_main_action_menu);
			#ifdef PBL_PLATFORM_APLITE
				window_set_fullscreen(s_venue_window, true);
			#endif
			window_stack_push(s_venue_window, true);
			s_active_venue=0;
		  s_venue_length=length;
			update_venue_layers(s_active_venue);
		break;
		
		case 2:    		
			APP_LOG(APP_LOG_LEVEL_INFO, "Recibidos los items en el Pebble"); 
			for (int i = 1; i <= (length); i ++) {  //rellenamos los nombres de los items
				strcpy(items_ids[i-1], dict_find(iter,i)->value->cstring);
				strcpy(items_names[i-1], dict_find(iter,i+10)->value->cstring);	
			}
			action_menu_unfreeze(s_main_action_menu);
			#ifdef PBL_PLATFORM_APLITE
				window_set_fullscreen(s_item_window, true);
			#endif
			window_stack_push(s_item_window, true);
			s_active_item=0;
  		s_item_length=length;
			update_item_layers(s_active_item);
		break;
		
		

	}
	
	
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
	
	#ifdef PBL_PLATFORM_APLITE
		window_set_fullscreen(s_main_window, true);
	#endif
  window_stack_push(s_main_window, true);
	
	// Register callbacks
	app_message_register_inbox_received((AppMessageInboxReceived) in_received_handler);
	app_message_register_inbox_dropped(inbox_dropped_callback);
	app_message_register_outbox_failed(outbox_failed_callback);
	app_message_register_outbox_sent(outbox_sent_callback);
	app_message_open(app_message_inbox_size_maximum(), app_message_outbox_size_maximum());
    
  init_main_action_menu();
 //	s_venue_root_level = action_menu_level_create(10);
 // init_venue_action_menu();
 // init_item_action_menu();
}

static void deinit() {
  window_destroy(s_main_window);
}

int main() {
  init();
  app_event_loop();
  deinit();
}
