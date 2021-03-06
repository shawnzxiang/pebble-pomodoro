// Written by Shawn Xiang

#include <pebble.h>
#include "mainwindow.h"
#include "../constants.h"
  
int m = 0;  // minute 
int s = 0;  // second
int worktime = 25;
int resttime = 5;
int longRestTime = 20; 
int longRestDelay = 4; 
int counter = 0;
int mode = MODE_PAUSED;
int beforePause = MODE_RUNNING_WORK; 

static char timeText[24] = "";
static char counterText[24] = "";
   
static Window *s_window;
static GFont s_res_bitham_42_medium_numbers;
static GFont s_res_roboto_condensed_21;
static GFont s_res_gothic_bold; 
static GBitmap *s_res_play_btn;
static GBitmap *s_res_x_btn;
static GBitmap *s_res_trash_btn;
static GBitmap *s_res_config_btn;
static TextLayer *timer_layer;
static TextLayer *counter_layer;
static TextLayer *clock_layer; 
static ActionBarLayer *s_actionbarlayer_1;

GColor workScreenColor; 
GColor restScreenColor; 
GColor pauseScreenColor; 

static void initialise_ui(void) {
  s_window = window_create();
  window_set_background_color(s_window, GColorClear);
  #ifndef PBL_SDK_3
    window_set_fullscreen(s_window, true);
  #endif
  
  s_res_bitham_42_medium_numbers = fonts_get_system_font(FONT_KEY_BITHAM_42_MEDIUM_NUMBERS);
  s_res_roboto_condensed_21 = fonts_get_system_font(FONT_KEY_ROBOTO_CONDENSED_21);
   s_res_gothic_bold = fonts_get_system_font(FONT_KEY_GOTHIC_18_BOLD);
  
  // hook up all resources buttons
  s_res_play_btn = gbitmap_create_with_resource(RESOURCE_ID_PLAY_BTN);
  s_res_x_btn = gbitmap_create_with_resource(RESOURCE_ID_X_BTN);
  s_res_trash_btn = gbitmap_create_with_resource(RESOURCE_ID_TRASH_BTN);
  s_res_config_btn = gbitmap_create_with_resource(RESOURCE_ID_CONFIG_BTN);

  // initialize the timer layer
  #if defined(PBL_ROUND)
    timer_layer = text_layer_create(GRect(10, 48, 127, 61));
  #else 
    timer_layer = text_layer_create(GRect(-3, 45, 117, 61));
  #endif
  
  text_layer_set_background_color(timer_layer, GColorClear);
  text_layer_set_text_alignment(timer_layer, GTextAlignmentCenter);
  text_layer_set_text_color(timer_layer, GColorWhite);
  text_layer_set_font(timer_layer, s_res_bitham_42_medium_numbers);
  layer_add_child(window_get_root_layer(s_window), (Layer *)timer_layer);

  // initialize the counter layer
  #if defined(PBL_ROUND)
    counter_layer = text_layer_create(GRect(10, 100, 124, 25));
  #else 
    counter_layer = text_layer_create(GRect(0, 100, 114, 25));
  #endif
  
  text_layer_set_background_color(counter_layer, GColorClear);
  text_layer_set_text_alignment(counter_layer, GTextAlignmentCenter);
  text_layer_set_text_color(counter_layer, GColorWhite);
  text_layer_set_font(counter_layer, s_res_roboto_condensed_21);
  layer_add_child(window_get_root_layer(s_window), (Layer *)counter_layer); 

  // initialize the current clock layer
  #if defined(PBL_ROUND)
    clock_layer = text_layer_create(GRect(20, 10, 140, 30));
  #else 
    clock_layer = text_layer_create(GRect(0, 10, 120, 30));
  #endif
  
  text_layer_set_background_color(clock_layer, GColorClear);
  text_layer_set_text_alignment(clock_layer, GTextAlignmentCenter);
  text_layer_set_text_color(clock_layer, GColorWhite);
  text_layer_set_font(clock_layer, s_res_gothic_bold);
  layer_add_child(window_get_root_layer(s_window), (Layer *)clock_layer); 
  
  // initialize all buttons in action layer (right most)
  s_actionbarlayer_1 = action_bar_layer_create();
  action_bar_layer_add_to_window(s_actionbarlayer_1, s_window);
  action_bar_layer_set_background_color(s_actionbarlayer_1, GColorClear);
  action_bar_layer_set_icon(s_actionbarlayer_1, BUTTON_ID_SELECT, s_res_play_btn);
  action_bar_layer_set_icon(s_actionbarlayer_1, BUTTON_ID_DOWN, s_res_trash_btn);
  action_bar_layer_set_icon(s_actionbarlayer_1, BUTTON_ID_UP, s_res_config_btn);
  layer_add_child(window_get_root_layer(s_window), (Layer *)s_actionbarlayer_1);
}

static void destroy_ui(void) {
  window_destroy(s_window);
  text_layer_destroy(timer_layer);
  text_layer_destroy(counter_layer);
  text_layer_destroy(clock_layer); 
  action_bar_layer_destroy(s_actionbarlayer_1);
  gbitmap_destroy(s_res_play_btn);
  gbitmap_destroy(s_res_x_btn);
  gbitmap_destroy(s_res_trash_btn);
  gbitmap_destroy(s_res_config_btn);
}

static void configRunningUI(){
  #if defined(PBL_COLOR)
    window_set_background_color(s_window, workScreenColor);
  #elif defined(PBL_BW)
    window_set_background_color(s_window, GColorLightGray);
  #endif
  
  action_bar_layer_set_icon(s_actionbarlayer_1, BUTTON_ID_SELECT, s_res_x_btn);
 
}

static void configRestUI(){
  #if defined(PBL_COLOR)
    window_set_background_color(s_window, restScreenColor);
  #elif defined(PBL_BW)
    window_set_background_color(s_window, GColorLightGray);
  #endif
  
  action_bar_layer_set_icon(s_actionbarlayer_1, BUTTON_ID_SELECT, s_res_x_btn);
}

static void configPauseUI(){
  #if defined(PBL_COLOR)
    window_set_background_color(s_window, pauseScreenColor);
  #elif defined(PBL_BW)
    window_set_background_color(s_window, GColorLightGray);
  #endif
  
  action_bar_layer_set_icon(s_actionbarlayer_1, BUTTON_ID_SELECT, s_res_play_btn);
}

// Display the status (rest, work or paused)
static char* getStatus(){
  if (mode == MODE_RUNNING_PAUSE){
    return "Rest"; 
  } else if (mode == MODE_RUNNING_WORK){
    return "Work";
  } else {
    return "Paused"; 
  }
}

// Display the postfix of the number
static char* postfixNumber(int counters){
  
  if (counters == 0)
    return ""; 
  else if (counters > 3 && counters < 21)  // most usercases will fall in this category
    return "th"; 
  else if ((counters-1) % 10 == 0) {
    return "st"; 
  }
  else if ((counters-2) % 10 == 0)
    return "nd"; 
  else if ((counters-3) % 10 == 0)
    return "rd"; 
  else 
    return "th"; 
}

static void updateStatusAndCounter(){
  if (mode != MODE_PAUSED)
    snprintf(counterText, 20, "%s (%d%s)", getStatus(), counter+1, postfixNumber(counter+1));
  else 
    snprintf(counterText, 20, "Paused");
  
  text_layer_set_text(counter_layer, counterText);
}

static void pauseIt(bool reset){
    
    configPauseUI();
    tick_timer_service_unsubscribe();
  
    mode = MODE_PAUSED; // pause it 
    if (reset) {
      // reset 
      snprintf(timeText, 20, "%.2d:00", worktime);
      
    }
    else {
      // continue
      snprintf(timeText, 20, "%.2d:%.2d", m, s); 
    }
  
    text_layer_set_text(timer_layer, timeText);
  
  updateStatusAndCounter();
}

static void runIt(bool reset){
  
  if (reset) {
      m = worktime;
      s = 0; 
  }
    
    bool prevModeIsPaused = mode == MODE_PAUSED; 
    mode = MODE_RUNNING_WORK;
  
    configRunningUI();
    vibes_short_pulse();
    light_enable_interaction();
  
    if (!prevModeIsPaused)
      updateCounter(false);  // update counter 
  
    tick_timer_service_subscribe(SECOND_UNIT, updateTimer);  //register the timer
    
    updateStatusAndCounter();
    snprintf(timeText, 7, "%.2d:%.2d", m, s);  // changes the time as it elapses
	  text_layer_set_text(timer_layer, timeText);
}

static void restIt(bool reset){
  if (reset) {
    // Counter starts from zero
    m = ((counter+1) % longRestDelay != 0 || (counter+1) == 0) ? resttime : longRestTime; 
    s = 0; 
  }
  
  mode = MODE_RUNNING_PAUSE;
  
  configRestUI();
  vibes_short_pulse();
  light_enable_interaction();
  
  tick_timer_service_subscribe(SECOND_UNIT, updateTimer);
  
  updateStatusAndCounter();
  snprintf(timeText, 7, "%.2d:%.2d", m, s);  // changes the time as it elapses
	text_layer_set_text(timer_layer, timeText);
}

static void runNext(){
  
  if (mode == MODE_RUNNING_WORK){
    restIt(true); 
  } else if (mode == MODE_RUNNING_PAUSE){
    runIt(true); 
  } 
  
}

static void updateCounter(bool clear){
  if (clear) {
    counter = 0;
    m = worktime; 
    s = 0; 
    pauseIt(true); 
  } else {
    counter++;  
  }
  
  updateStatusAndCounter();
  
  persist_write_int(DATA_COUNTER,counter);
}
static void updateClock() {
  // Get a tm structure
  time_t temp = time(NULL);
  struct tm *tick_time = localtime(&temp);

  // Write the current hours and minutes into a buffer
  static char s_buffer[8];
  strftime(s_buffer, sizeof(s_buffer), clock_is_24h_style() ?
                                          "%H:%M" : "%I:%M", tick_time);

  // Display this time on the TextLayer
  text_layer_set_text(clock_layer, s_buffer);
}
// update the time 
static void updateTimer(struct tm *tick_time, TimeUnits units_changed){

  if(s == 0 && m > 0){ // end of the minutes 
    s = 59;
    m-- ;
    updateClock(); 
  } else if(s == 0 && m == 0){  // time ran out 
    runNext(); 
  } else{
    s-- ;
  }
  
  
  snprintf(timeText, 7, "%.2d:%.2d", m, s);  // changes the time as it elapses
	text_layer_set_text(timer_layer, timeText);
}



// middle button
static void select_click_handler(ClickRecognizerRef recognizer, void *context) {
  if(mode == MODE_RUNNING_WORK || mode == MODE_RUNNING_PAUSE){
    beforePause = mode; 
    pauseIt(false); 
  } else {  // This is MODE_PAUSE 
    if (beforePause == MODE_RUNNING_WORK)
      runIt(false); 
    else if (beforePause == MODE_RUNNING_PAUSE)
      restIt(false); 
  }
  
  updateStatusAndCounter(); 
}


static void down_click_handler(ClickRecognizerRef recognizer, void *context){
  beforePause = MODE_RUNNING_WORK; 
  updateCounter(true);
}

static void up_click_handler(ClickRecognizerRef recognizer, void *context){
	runNext();
}

static void click_config_provider(void *context) {
  window_single_click_subscribe(BUTTON_ID_SELECT, select_click_handler);
  window_single_click_subscribe(BUTTON_ID_DOWN, down_click_handler);
  window_single_click_subscribe(BUTTON_ID_UP, up_click_handler);
}

static void handle_window_unload(Window* window) {
  destroy_ui();
}

static void handle_window_appear(Window* window){
  // read all data 
	worktime = persist_read_int(CONFIG_WORK);
	resttime = persist_read_int(CONFIG_REST);
  longRestTime = persist_read_int(CONFIG_LONG_REST); 
  longRestDelay = persist_read_int(CONFIG_LONG_REST_DELAY); 
  
  
  // the system by default is at pause 
  mode = MODE_PAUSED; 
  // if this is paused, this is the default value -> do running next. Otherwise, continue the previous iteration
  beforePause = persist_read_int(SAVED_MODE) != MODE_PAUSED ? persist_read_int(SAVED_MODE) : MODE_RUNNING_WORK; 
  
  m = persist_read_int(SAVED_TIME) / 60; 
  s = persist_read_int(SAVED_TIME) % 60; 
  
  if (m == 0 && s == 0)
	  snprintf(timeText, 7, "%.2d:00", worktime);
  else 
    snprintf(timeText, 7, "%.2d:%.2d", m, s);
  
	text_layer_set_text(timer_layer, timeText);

  updateClock(); 
  
  counter = persist_read_int(DATA_COUNTER);
  updateStatusAndCounter(); 
}

void saveData(){
  persist_write_int(SAVED_TIME, m*60+s); // save the current time stamp 
  if (mode != MODE_PAUSED)
    persist_write_int(SAVED_MODE, mode); // save the current mode
  else 
    persist_write_int(SAVED_MODE, beforePause); // save the current mode
}

static void handle_window_disappear(Window* window){
  saveData(); 
}

void show_mainwindow(void) {
  workScreenColor = GColorFromHEX(persist_read_int(COLOR_WORK)); 
  restScreenColor = GColorFromHEX(persist_read_int(COLOR_REST)); 
  pauseScreenColor = GColorFromHEX(persist_read_int(COLOR_PAUSE)); 
  
  initialise_ui();
  window_set_click_config_provider(s_window, click_config_provider);
  configPauseUI();
  window_set_window_handlers(s_window, (WindowHandlers) {
    .unload = handle_window_unload,
    .appear = handle_window_appear,
  });
  window_stack_push(s_window, true);
}

void hide_mainwindow(void) {
  saveData(); 
  
  window_stack_remove(s_window, true);
  snprintf(timeText, 7, "%.2d:%.2d", m, s);  // changes the time as it elapses
	text_layer_set_text(timer_layer, timeText);
}
