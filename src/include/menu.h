#pragma once

#include "util.h"


#define TERM_N_COL 85  // Terminal number of columns
#define TERM_N_ROW 18 // Terminal number of rows

#define ARROW_COL_OFFSET 28
#define ARROW_ROW_OFFSET 10
#define MENU_N_OPTIONS 2



#define CANVAS_COLUMNS 80
#define CANVAS_ROWS 21

#define MENU_START_COLUMN 3
#define MAIN_MENU_NUMBER_OF_OPTIONS 4
#define ARROW_MENU_COLUMNS 2


void update_screen(const char *background, char *input_layer);
int get_menu_option(char *input_layer);
int get_keyboard_str(char *input_layer, char *str_buffer, int max_str_len);


