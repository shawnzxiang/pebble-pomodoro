// Written by Shawn Xiang

#include <pebble.h>
#include "windows/mainwindow.h"
#include "constants.h"
    
ClaySettings settings;
// Initialize the default settings
static void prv_default_settings() {
  settings.workTime = 25;
  settings.restTime = 5;
  settings.longRestTime = 15;
  settings.longRestFrequency = 4;
}

// Read settings from persistent storage
static void prv_load_settings() {
  // Load the default settings
  prv_default_settings();
  // Read settings from persistent storage, if they exist
  persist_read_data(SETTINGS_KEY, &settings, sizeof(settings));
}

// Save the settings to persistent storage
static void prv_save_settings() {
  persist_write_data(SETTINGS_KEY, &settings, sizeof(settings));
 
}

// Handle the response from AppMessage
static void prv_inbox_received_handler(DictionaryIterator *iter, void *context) {
  APP_LOG(APP_LOG_LEVEL_DEBUG, "received");
  
  Tuple *tempwork = dict_find(iter, MESSAGE_KEY_WorkTime);
  if (tempwork) {
    
    settings.workTime = tempwork->value->int32; 
    APP_LOG(APP_LOG_LEVEL_DEBUG, "work time assigned %d", settings.workTime);
  
    persist_write_int(CONFIG_WORK, settings.workTime); 
  }

 Tuple *temprest = dict_find(iter, MESSAGE_KEY_RestTime);
  if (temprest) {
    settings.restTime = temprest->value->int32; 
    persist_write_int(CONFIG_REST,settings.restTime) ; 
  }
  
  Tuple *templongrest = dict_find(iter, MESSAGE_KEY_LongRestTime);
  if (templongrest) {
    settings.longRestTime = templongrest->value->int32; 
    persist_write_int(CONFIG_LONG_REST, settings.longRestTime); 
  }
  
   Tuple *templongrestfreq = dict_find(iter, MESSAGE_KEY_LongRestFrequency);
  
  if (templongrestfreq) {
    settings.longRestFrequency = templongrestfreq->value->int32; 
    persist_write_int(CONFIG_LONG_REST_DELAY, settings.longRestFrequency); 
  }
  

  // Save the new settings to persistent storage
  prv_save_settings();
}

static void init(void) {
	//if(!persist_exists(CONFIG_WORK)){
  
 // if (!persist_exists(SAVED_MODE)) {
    if (!persist_exists(CONFIG_WORK)) {
		persist_write_int(CONFIG_WORK,25);
		persist_write_int(CONFIG_REST,5);
    persist_write_int(CONFIG_LONG_REST, 15); 
    persist_write_int(CONFIG_LONG_REST_DELAY, 4); 
    
	}
  
  if (!persist_exists(SAVED_MODE)){
    persist_write_int(SAVED_MODE, MODE_PAUSED); 
  }
  
  if (!persist_exists(SAVED_TIME)){
    persist_write_int(SAVED_TIME, 1500); 
  } 
  
   if (!persist_exists(DATA_COUNTER)){
    persist_write_int(DATA_COUNTER, 0); 
  } 
  
  
  prv_load_settings();

  // Listen for AppMessages
  app_message_register_inbox_received(prv_inbox_received_handler);
  app_message_open(128, 128);
  
	show_mainwindow();
}

static void deinit(void) {
  
  hide_mainwindow();
}

int main(void) {
  init();
  app_event_loop();
  deinit();
}
