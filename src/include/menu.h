#pragma once

#include "util.h"


#define TERM_N_COL 85  // Terminal number of columns
#define TERM_N_ROW 18 // Terminal number of rows

#define ARROW_COL_OFFSET 28
#define ARROW_ROW_OFFSET 10
#define MENU_N_OPTIONS 2

// Main menu constants
#define PSOC_PROMPT_ROW_OFFSET      11
#define PSOC_PROMPT_COL_OFFSET      18
#define PSOC_PROMPT_OFFSET          TERM_N_COL*PSOC_PROMPT_ROW_OFFSET + PSOC_PROMPT_COL_OFFSET
#define MONITOR_PROMPT_ROW_OFFSET   12
#define MONITOR_PROMPT_COL_OFFSET   29
#define MONITOR_PROMPT_OFFSET       TERM_N_COL*MONITOR_PROMPT_ROW_OFFSET + MONITOR_PROMPT_COL_OFFSET
#define LOG_PROMPT_ROW_OFFSET       13
#define LOG_PROMPT_COL_OFFSET       26
#define LOG_PROMPT_OFFSET           TERM_N_COL*LOG_PROMPT_ROW_OFFSET + LOG_PROMPT_COL_OFFSET

// Monitor menu constants
#define STATUS_PROMPT_OFFSET        TERM_N_COL*(TERM_N_ROW - 2) + 3
#define DEFAULT_STATUS_PROMPT_LEN   11 
#define SPINNER_ANIMATION_LEN       4 


#define CANVAS_COLUMNS 80
#define CANVAS_ROWS 21

#define MENU_START_COLUMN 3
#define MAIN_MENU_NUMBER_OF_OPTIONS 4
#define ARROW_MENU_COLUMNS 2

//void update_screen(const char *background, char *input_layer, char *ascii_art);
void update_screen(const char *background, char *input_layer, char *ascii_art[]);
//void update_screen(const char *background, char *input_layer);
int get_menu_option(char *input_layer);
int get_keyboard_str(char *input_layer, char *str_buffer, int max_str_len);


