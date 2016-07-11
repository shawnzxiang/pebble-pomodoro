// Written by Shawn Xiang

void show_mainwindow(void);
void hide_mainwindow(void);
static void updateCounter(bool clear); 
static void initialise_ui(void);
static void destroy_ui(void);
static void configPauseUI(); 
static void configRunningUI();
static void configRestUI(); 
static void pauseIt(bool);
static void runIt(bool);
static void restIt(bool); 
static void runNext(); 
static void updateCounter(bool clear); 
static void updateTimer(struct tm *tick_time, TimeUnits units_changed);
