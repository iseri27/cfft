#include "cf.h"
#include "config.h"
#include "utils.h"
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ncurses.h>

char buf1[255], buf2[255];
CF_Integer PADDING_TOP;
CF_Integer PADDING_LEFT;
CF_Integer MAIN_BOX_COLS;
CF_Integer MAIN_BOX_ROWS;
CF_Integer PROMPT_BOX_COLS;
CF_Integer PROMPT_BOX_ROWS;
CF_Integer GAP;

WINDOW* window_display(CF_Array*);
WINDOW* window_prompt(const char* prompt);
WINDOW* window_entry();
WINDOW* window_message(char* msg);
CF_Bool check_index_valid(char* str, CF_Integer max_index);
CF_Integer parseInt(char* str);
int compare(CF_File*, CF_File*);
void init();

int main(int argc, char *argv[]) {
    // Some init setting for ncurses
    initscr();
    cbreak();
    keypad(stdscr, TRUE);

    init();

    CF_Array* array = NULL;
    CF_File* template_dir = NULL;

    array = CF_ARRAY_new(DEFAULT_ARRAY_LENGTH);
    template_dir = CF_FILE_new(HOME_DIR, TEMPLATES_DIR);

    CF_Bool rc;

    rc = CF_FILE_list_directory(template_dir, False, array);

    if (rc == False) {
        // fprintf(stderr, "Faliled to get file list in %s.", template_dir->fullpath);
    }

    array->compare = compare;
    CF_ARRAY_sort(array);

    // Create Boxes
    WINDOW* main_box = window_display(array);
    WINDOW* prompt_choose = window_prompt("Choose a file");
    WINDOW* entry_index = window_entry();
    WINDOW* msg_error = NULL;

    wscanw(entry_index, "%s", buf1);

    while (check_index_valid(buf1, array->size) == False) {
        // Display Warning
        msg_error = window_message("Invalid Input!(Press any key to reinput)");
        // Wait User's Operation to delete warning message.
        int ch = getchar();
        destroy_win(msg_error);

        destroy_win(prompt_choose);
        destroy_win(entry_index);
        prompt_choose = window_prompt("Choose a file");
        entry_index = window_entry();

        wscanw(entry_index, "%s", buf1);
    }

    int chosen = parseInt(buf1);
    chosen = chosen - 1;
    
    CF_FILE_copy(CF_ARRAY_get(array, chosen), CF_ARRAY_get(array, chosen)->basename, True);

    WINDOW* msg_success = window_message("Successfully to Create File!");
    int ch = getchar();
    
    CF_FILE_free(&template_dir);
    CF_ARRAY_free(&array);

    destroy_win(main_box);
    destroy_win(prompt_choose);
    destroy_win(entry_index);
    destroy_win(msg_success);
    endwin();

    return 0;
}

void init() {/*{{{*/
    PADDING_TOP = (CF_Integer) (0.15 * LINES);
    PADDING_LEFT = (CF_Integer) (0.3 * COLS);
    MAIN_BOX_COLS = (CF_Integer) (0.4 * COLS);
    MAIN_BOX_ROWS = (CF_Integer) (0.3 * LINES);

    GAP = 1;
}/*}}}*/

int compare(CF_File* cff1, CF_File* cff2) {/*{{{*/
    return strcmp(cff1->basename, cff2->basename);
}/*}}}*/

WINDOW* window_display(CF_Array* array) {/*{{{*/
    // Get max length of file name.
    int cols = MAIN_BOX_COLS;
    int rows = MAIN_BOX_ROWS;

    int start_col = PADDING_LEFT;
    int start_row = PADDING_TOP;

    WINDOW *main_box = NULL;
    main_box = create_newwin(rows, cols, start_row, start_col);

    for (int i = 0; i < array->size; i++) {
        if (array->size < 10) {
            mvwprintw(main_box, i+1, 2, "%d. %s", i+1, CF_ARRAY_get(array, i)->basename);
            wrefresh(main_box);
        } else {
            mvwprintw(main_box, i+1, 2, "%2d. %s", i+1, CF_ARRAY_get(array, i)->basename);
            wrefresh(main_box);
        }
    }

    return main_box;
}/*}}}*/

WINDOW* window_prompt(const char* prompt) {/*{{{*/
    int cols = strlen(prompt) + 2;
    int rows = 3;

    PROMPT_BOX_COLS = cols;
    PROMPT_BOX_ROWS = rows;
    
    int start_col = PADDING_LEFT;
    int start_row = PADDING_TOP + MAIN_BOX_ROWS + GAP;

    WINDOW* w_prompt = create_newwin(rows, cols, start_row, start_col);

    mvwprintw(w_prompt, 1, 1, "%s", prompt);
    wrefresh(w_prompt);

    return w_prompt;
}/*}}}*/

WINDOW* window_entry() {/*{{{*/
    int cols = MAIN_BOX_COLS - GAP - PROMPT_BOX_COLS;
    int rows = PROMPT_BOX_ROWS;

    int start_col = PADDING_LEFT + PROMPT_BOX_COLS + GAP;
    int start_row = PADDING_TOP + MAIN_BOX_ROWS + GAP;

    WINDOW* entry = create_newwin(rows, cols, start_row, start_col);
    wmove(entry, 1, 1);
    wrefresh(entry);
    return entry;
}/*}}}*/

WINDOW* window_message(char* msg) {/*{{{*/
    int rows = PROMPT_BOX_ROWS;
    int cols = MAIN_BOX_COLS;

    int start_col = PADDING_LEFT;
    int start_row = PADDING_TOP + MAIN_BOX_ROWS + GAP;

    WINDOW* w_msg = create_newwin(rows, cols, start_row, start_col);
    mvwprintw(w_msg, 1, 1, "%s", msg);
    wrefresh(w_msg);
    return w_msg;
}/*}}}*/

CF_Bool check_index_valid(char* str, CF_Integer max_index) {/*{{{*/
    int len = strlen(str);
    int n = 0;

    for (int i = 0; i < len; i++) {
        if (str[i] < '0' || str[i] > '9') {
            return False;
        }

        n = n * 10 + str[i] - '0';
    }

    if (n <= 0 || n > max_index) {
        return False;
    }

    return True;
}/*}}}*/

CF_Integer parseInt(char* str) {/*{{{*/
    CF_Integer n = 0;
    int len = strlen(str);
    for (int i = 0; i < len; i++) {
        n = n * 10 + str[i] - '0';
    }

    return n;
}/*}}}*/
