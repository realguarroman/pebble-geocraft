#include <pebble.h>
#include "action_menu.h"

#define LEVEL_1_ITEMS 4
//#define LEVEL_2_ITEMS 3

	
	
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


typedef struct {
  int type;
} Context;

typedef enum {
  VibrationTypeShort,
  VibrationTypeLong,
  VibrationTypeDouble,
  VibrationTypeCustomShort,
  VibrationTypeCustomMedium,
  VibrationTypeCustomLong
} VibrationType;

static Window *s_main_window;
static TextLayer *s_label_layer;
static ActionBarLayer *s_action_bar;

static GBitmap *s_ellipsis_bitmap;
static int s_current_type;

static ActionMenu *s_action_menu;
static ActionMenuLevel *s_root_level, *s_custom_level;

/********************************* ActionMenu *********************************/

static void action_performed_callback(ActionMenu *action_menu, const ActionMenuItem *action, void *context) {
  // An action was selected, determine which one
  s_current_type = *(int*)action_menu_item_get_action_data(action);

  // Play this vibration
  static uint32_t segments[5];
  switch(s_current_type) {
    case VibrationTypeShort:  vibes_short_pulse();  break;
    case VibrationTypeLong:   vibes_long_pulse();   break;
    case VibrationTypeDouble: vibes_double_pulse(); break;
    default:
      // Create the patten
      for(int i = 0; i < 5; i++) {
        switch(s_current_type) {
          case VibrationTypeCustomShort:  segments[i] = i * 100; break;
          case VibrationTypeCustomMedium: segments[i] = i * 200; break;
          case VibrationTypeCustomLong:   segments[i] = i * 300; break;
          default: break;
        }
      }

      // Play the custom pattern
      VibePattern pat = {
        .durations = segments,
        .num_segments = ARRAY_LENGTH(segments),
      };
      vibes_enqueue_custom_pattern(pat);
      break;
  }
}

static void init_action_menu() {
  // Create the root level and secondary custom patterns level
//  s_root_level = action_menu_level_create(LEVEL_1_ITEMS);
//  s_custom_level = action_menu_level_create(LEVEL_2_ITEMS);

  // Set up the actions for this level, using action context to pass types
//  action_menu_level_add_action(s_root_level, "Short", action_performed_callback, 
//                               &(Context){.type=VibrationTypeShort});
//  action_menu_level_add_action(s_root_level, "Long", action_performed_callback, 
//                               &(Context){.type=VibrationTypeLong});
//  action_menu_level_add_action(s_root_level, "Double", action_performed_callback, 
//                               &(Context){.type=VibrationTypeDouble});
  
  // Set up the secondary level's actions
//  action_menu_level_add_child(s_root_level, s_custom_level, "Custom Pattern");
//  action_menu_level_add_action(s_custom_level, "Custom Fast", action_performed_callback, 
 //                              &(Context){.type=VibrationTypeCustomShort});
//  action_menu_level_add_action(s_custom_level, "Custom Medium", action_performed_callback, 
//                               &(Context){.type=VibrationTypeCustomMedium});
//  action_menu_level_add_action(s_custom_level, "Custom Slow", action_performed_callback, 
//                               &(Context){.type=VibrationTypeCustomLong});
}




static void request_venues(void) {
 // DictionaryIterator *iter;
 // app_message_outbox_begin(&iter);

 // if (!iter) {
    // Error creating outbound message
 //   return;
 // }

  //int value = 1;
  //dict_write_int(iter, 1, &value, sizeof(int), true);
 // dict_write_end(iter);

	
 // app_message_outbox_send();
	
	
	
		vibes_short_pulse();
		DictionaryIterator *iter;
		app_message_outbox_begin(&iter);
		// Add a key-value pair
		dict_write_uint8(iter, FETCH_TYPE, 0); // 0 es el tipo "venue"
		// Send the message!
		app_message_outbox_send();
	//	APP_LOG(APP_LOG_LEVEL_DEBUG, "Temperatura solicitada"); 
}


/*********************************** Clicks ***********************************/

static void select_click_handler(ClickRecognizerRef recognizer, void *context) {
	request_venues();
	
  // Configure the ActionMenu Window about to be shown
//  ActionMenuConfig config = (ActionMenuConfig) {
 //   .root_level = s_root_level,
  //  .colors = {
//#ifdef PBL_COLOR
//      .background = GColorChromeYellow,
//      .foreground = GColorBlack,
//#else
//      .background = GColorWhite,
//      .foreground = GColorBlack,
//#endif
      
 //   },
 //   .align = ActionMenuAlignCenter
 // };

  // Show the ActionMenu
 // s_action_menu = action_menu_open(&config);
}

static void click_config_provider(void *context) {
  window_single_click_subscribe(BUTTON_ID_SELECT, select_click_handler);
}








/******************************** Main Window *********************************/

static void window_load(Window *window) {
  Layer *window_layer = window_get_root_layer(window);
  GRect bounds = layer_get_bounds(window_layer);

  s_ellipsis_bitmap = gbitmap_create_with_resource(RESOURCE_ID_ELLIPSIS);

  s_action_bar = action_bar_layer_create();
  action_bar_layer_set_click_config_provider(s_action_bar, click_config_provider);
  action_bar_layer_set_icon(s_action_bar, BUTTON_ID_SELECT, s_ellipsis_bitmap);
  action_bar_layer_add_to_window(s_action_bar, window);

  s_label_layer = text_layer_create(GRect(0, 0, bounds.size.w - ACTION_BAR_WIDTH, bounds.size.h));
  text_layer_set_text(s_label_layer, "Bienvenido a Geocraft");
  text_layer_set_font(s_label_layer, fonts_get_system_font(FONT_KEY_GOTHIC_24_BOLD));
  text_layer_set_text_color(s_label_layer, GColorBlack);
  text_layer_set_background_color(s_label_layer, GColorClear);
  text_layer_set_text_alignment(s_label_layer, GTextAlignmentCenter);
  layer_add_child(window_layer, text_layer_get_layer(s_label_layer));
	
	
	
	
}




static void in_received_handler(DictionaryIterator *iter, void *context) 
{
    
	
		s_root_level = action_menu_level_create(LEVEL_1_ITEMS);
  //	s_custom_level = action_menu_level_create(LEVEL_2_ITEMS);

  // Set up the actions for this level, using action context to pass types
  
	
	
		(void) context;
    //Get data
    Tuple *t = dict_read_first(iter);
    while(t != NULL)
    {		
			
		//	process_tuple(t);      
      //Get next
			
			int key = t->key;
			switch(key) {
				case ITEM_1_NAME:
					{
						action_menu_level_add_action(s_root_level, t->value->cstring, action_performed_callback, 
                               &(Context){.type=VibrationTypeShort});
					}
				break;
				case ITEM_2_NAME:
					{
						action_menu_level_add_action(s_root_level, t->value->cstring, action_performed_callback, 
                               &(Context){.type=VibrationTypeLong});
					}
				break;
				case ITEM_3_NAME:
					{
						action_menu_level_add_action(s_root_level, t->value->cstring, action_performed_callback, 
                               &(Context){.type=VibrationTypeDouble});
					}
				break;
				case ITEM_4_NAME:
					{
						action_menu_level_add_action(s_root_level, t->value->cstring, action_performed_callback, 
                               &(Context){.type=VibrationTypeCustomLong});
					}
				break;
				
			}
				
			
			
			
      t = dict_read_next(iter);
			
			
    }
	  // Configure the ActionMenu Window about to be shown
  ActionMenuConfig config = (ActionMenuConfig) {
    .root_level = s_root_level,
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
	s_action_menu = action_menu_open(&config);
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


static void window_unload(Window *window) {
  text_layer_destroy(s_label_layer);
  action_bar_layer_destroy(s_action_bar);
  gbitmap_destroy(s_ellipsis_bitmap);

  action_menu_hierarchy_destroy(s_root_level, NULL, NULL);
}

/************************************ App *************************************/

static void init() {
  s_main_window = window_create();
#ifdef PBL_COLOR
  window_set_background_color(s_main_window, GColorChromeYellow);
#else
  window_set_background_color(s_main_window, GColorWhite);
#endif
  window_set_window_handlers(s_main_window, (WindowHandlers) {
    .load = window_load,
    .unload = window_unload,
  });
  window_stack_push(s_main_window, true);

  init_action_menu();
	
	
	
	
	
	app_message_register_inbox_received((AppMessageInboxReceived) in_received_handler);
	
	// Register callbacks
	app_message_register_inbox_dropped(inbox_dropped_callback);
	app_message_register_outbox_failed(outbox_failed_callback);
	app_message_register_outbox_sent(outbox_sent_callback);

	app_message_open(app_message_inbox_size_maximum(), app_message_outbox_size_maximum());
//	app_message_open(64, 64);
}

static void deinit() {
  window_destroy(s_main_window);
}

int main() {
  init();
  app_event_loop();
  deinit();
}
