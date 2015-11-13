/**
 * Example implementation of the dialog message UI pattern.
 */

#include "dialog.h"

static Window *s_main_window;
static TextLayer *s_label_layer;
static BitmapLayer *s_icon_layer;
static Layer *s_background_layer;

AppTimer *timer;

static GBitmap *s_icon_bitmap;
static char message[80]; 

static void background_update_proc(Layer *layer, GContext *ctx) {
 
	
	#ifdef PBL_COLOR
  	 graphics_context_set_fill_color(ctx, GColorYellow);
	#endif
	
	
  graphics_fill_rect(ctx, layer_get_bounds(layer), 0, 0);
}

static void window_load(Window *window) {
  Layer *window_layer = window_get_root_layer(window);
  GRect bounds = layer_get_bounds(window_layer);

  s_background_layer = layer_create(GRect(0, 168, bounds.size.w, bounds.size.h));
	layer_set_update_proc(s_background_layer, background_update_proc);
  layer_add_child(window_layer, s_background_layer);

  s_icon_bitmap = gbitmap_create_with_resource(RESOURCE_ID_WARNING);
  GRect bitmap_bounds = gbitmap_get_bounds(s_icon_bitmap);

  s_icon_layer = bitmap_layer_create(GRect(10,  10, bitmap_bounds.size.w, bitmap_bounds.size.h));
  bitmap_layer_set_bitmap(s_icon_layer, s_icon_bitmap);
  bitmap_layer_set_compositing_mode(s_icon_layer, GCompOpSet);
  layer_add_child(window_layer, bitmap_layer_get_layer(s_icon_layer));

  s_label_layer = text_layer_create(GRect(10,  40, 124, 150));
  text_layer_set_text(s_label_layer, message);
  text_layer_set_background_color(s_label_layer, GColorClear);
  text_layer_set_font(s_label_layer, fonts_get_system_font(FONT_KEY_GOTHIC_24_BOLD));
  layer_add_child(window_layer, text_layer_get_layer(s_label_layer));
}

static void window_unload(Window *window) {
  layer_destroy(s_background_layer);

  text_layer_destroy(s_label_layer);

  bitmap_layer_destroy(s_icon_layer);
  gbitmap_destroy(s_icon_bitmap);

  window_destroy(window);
  s_main_window = NULL;
}

void timer_callback(void *data) {
	 window_stack_remove(s_main_window, true);;
}

void dialog_message_window_push(char cadena[]) {
	strcpy(message, cadena);
  if(!s_main_window) {
    s_main_window = window_create();
    window_set_window_handlers(s_main_window, (WindowHandlers) {
        .load = window_load,
        .unload = window_unload,
    });
  }
  window_stack_push(s_main_window, true);
	timer = app_timer_register(3000, (AppTimerCallback) timer_callback, NULL);
}