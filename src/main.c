#include <pebble.h>

#include "src/configuration.h"

#define KEY_TYPE 1
  
static Window *_window;
static TextLayer *_timetext;
static TextLayer *_datetext;
static BitmapLayer *_bitmaplayer;
static GBitmap *_bitmap;

static enum ChenType _type;

static void update_time() {
  static char buffer_time[] = "00:00";
  clock_copy_time_string(buffer_time, sizeof(buffer_time));
  text_layer_set_text(_timetext, buffer_time);
}

static void update_date() {
  static char buffer_date[] = "Mon 1995-03-30";
  time_t temp = time(NULL);
  struct tm *t = localtime(&temp);
  strftime(buffer_date, sizeof(buffer_date), "%a %F", t);
  text_layer_set_text(_datetext, buffer_date);
}

static void image_load()
{
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
  
  gbitmap_destroy(_bitmap);
  _bitmap = gbitmap_create_with_resource(resource);
  bitmap_layer_set_bitmap(_bitmaplayer, _bitmap);
}

static void window_load(Window *window)
{
  _timetext = text_layer_create(GRect(0,10,144,60));
  text_layer_set_background_color(_timetext, GColorClear);
  text_layer_set_text_color(_timetext, GColorBlack);
  text_layer_set_font(_timetext, fonts_get_system_font(FONT_KEY_GOTHIC_28_BOLD));
  text_layer_set_text_alignment(_timetext, GTextAlignmentCenter);
  
  _datetext = text_layer_create(GRect(0,45,144,60));
  text_layer_set_background_color(_datetext, GColorClear);
  text_layer_set_text_color(_datetext, GColorBlack);
  text_layer_set_font(_datetext, fonts_get_system_font(FONT_KEY_GOTHIC_14));
  text_layer_set_text_alignment(_datetext, GTextAlignmentCenter);
  
  _bitmaplayer = bitmap_layer_create(GRect(0,0,144,168));
  
  image_load();
  
  layer_add_child(window_get_root_layer(_window), bitmap_layer_get_layer(_bitmaplayer));
  layer_add_child(window_get_root_layer(_window), text_layer_get_layer(_timetext));
  layer_add_child(window_get_root_layer(_window), text_layer_get_layer(_datetext));
}

static void window_unload(Window *window)
{
  gbitmap_destroy(_bitmap);
  bitmap_layer_destroy(_bitmaplayer);
  text_layer_destroy(_timetext);
  text_layer_destroy(_datetext);
}

static void tick_handler(struct tm *tick_time, TimeUnits units_changed)
{
  switch(units_changed)
  {
    case MINUTE_UNIT:
      update_time();
      break;
    case DAY_UNIT:
      update_date();
      break;
    default:
      break;
  }
}

static void inbox_received_callback(DictionaryIterator *iterator, void *context) {
  Tuple *t = dict_read_first(iterator);

  while(t) {
    switch (t->key) {
      case KEY_TYPE:
        _type = t->value->int32;
        image_load();
        configuration_save(t->value->int32);
        break;
    }
    
    t = dict_read_next(iterator);
  }
}

static void init()
{
  app_message_register_inbox_received(inbox_received_callback);
  app_message_open(app_message_inbox_size_maximum(), app_message_outbox_size_maximum());
  
  _window = window_create();
  window_set_window_handlers(_window, (WindowHandlers){
    .load = window_load,
    .unload = window_unload
  });
  
  _type = configuration_load();
  
  window_stack_push(_window, true);
  update_time();
  update_date();
  
  tick_timer_service_subscribe(MINUTE_UNIT, tick_handler);
  tick_timer_service_subscribe(DAY_UNIT, tick_handler);
}

static void deinit()
{
  tick_timer_service_unsubscribe();
  configuration_save(_type);
  window_destroy(_window);
}

int main(void)
{
  init();
  app_event_loop();
  deinit();
}