#pragma once

#define CHEN_TYPES 3
  
enum ChenType
{
  Chibi = 0,
  Normal = 1,
  Ex = 2
};

enum ChenType configuration_load(void);
void configuration_save(enum ChenType newType);
