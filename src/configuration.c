#include <pebble.h>
#include "configuration.h"

#define KEY_CHENTYPE 1
  
enum ChenType configuration_load()
{
  if(!persist_exists(KEY_CHENTYPE))
  {
    configuration_save(Chibi);
  }
  
  return persist_read_int(KEY_CHENTYPE);
}

void configuration_save(enum ChenType type)
{
  persist_write_int(KEY_CHENTYPE, type);
}
