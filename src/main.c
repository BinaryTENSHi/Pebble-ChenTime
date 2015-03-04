#include <pebble.h>

#include "src/configuration.h"

static Window *_window;
static TextLayer *_timetext;
static BitmapLayer *_bitmaplayer;
static GBitmap *_bitmap;

static enum ChenType _type;

static void update_time() {
  time_t temp = time(NULL); 
  struct tm *tick_time = localtime(&temp);
  static char buffer[] = "00:00";
  
  strftime(buffer, sizeof("00:00"), "%H:%M", tick_time);
  text_layer_set_text(_timetext, buffer);
}

static void window_load(Window *window)
{
  _timetext = text_layer_create(GRect(0,20,144,60));
  text_layer_set_background_color(_timetext, GColorClear);
  text_layer_set_text_color(_timetext, GColorBlack);
  text_layer_set_font(_timetext, fonts_get_system_font(FONT_KEY_BITHAM_42_BOLD));
  text_layer_set_text_alignment(_timetext, GTextAlignmentCenter);
  
  int resource = RESOURCE_ID_CHEN_CHIBI;
  
  switch (_type)
  {
    case Chibi:
      resource = RESOURCE_ID_CHEN_CHIBI;
      break;
    case Normal:
      resource = RESOURCE_ID_CHEN_NORMAL;
      break;
    case Ex:
      resource = RESOURCE_ID_CHEN_EX;
      break;
  }
  
  _bitmap = gbitmap_create_with_resource(resource);
  _bitmaplayer = bitmap_layer_create(GRect(0,0,144,168));
  bitmap_layer_set_bitmap(_bitmaplayer, _bitmap);
  
  layer_add_child(window_get_root_layer(_window), bitmap_layer_get_layer(_bitmaplayer));
  layer_add_child(window_get_root_layer(_window), text_layer_get_layer(_timetext));
}

static void window_unload(Window *window)
{
  gbitmap_destroy(_bitmap);
  bitmap_layer_destroy(_bitmaplayer);
  text_layer_destroy(_timetext);
}

static void tick_handler(struct tm *tick_time, TimeUnits units_changed)
{
  update_time();
}

static void init()
{
  _window = window_create();
  window_set_window_handlers(_window, (WindowHandlers){
    .load = window_load,
    .unload = window_unload
  });
  
  _type = configuration_load();
  
  window_stack_push(_window, true);
  update_time();
  
  tick_timer_service_subscribe(MINUTE_UNIT, tick_handler);
}

static void deinit()
{
  configuration_save(_type);
  window_destroy(_window);
}

int main(void)
{
  init();
  app_event_loop();
  deinit();
}