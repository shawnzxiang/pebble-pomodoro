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

// A structure containing our settings
typedef struct ClaySettings {
  int workTime; 
  int restTime; 
  int longRestTime;
  int longRestFrequency; 
} __attribute__((__packed__)) ClaySettings;

