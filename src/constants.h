// Written by Shawn Xiang

#pragma once
#include <pebble.h>

#define CONFIG_LONG_REST 0
#define CONFIG_LONG_REST_DELAY 1
#define CONFIG_WORK 2
#define CONFIG_REST 3

#define DATA_COUNTER 4

#define MODE_RUNNING_WORK 5
#define MODE_RUNNING_PAUSE 6
#define MODE_PAUSED 7
#define SAVED_MODE 8
#define SAVED_TIME 9

#define SETTINGS_KEY 10

#define COLOR_REST 11
#define COLOR_WORK 12
#define COLOR_PAUSE 13


// A structure containing our settings
typedef struct ClaySettings {
  int workTime; 
  int restTime; 
  int longRestTime;
  int longRestFrequency; 
  GColor workColor; 
  GColor restColor; 
  GColor pauseColor; 
} __attribute__((__packed__)) ClaySettings;

