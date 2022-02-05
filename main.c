#include "cf.h"
#include "utils.h"
#include "config.h"
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <locale.h>
#include <ncurses.h>

char buf1[255], buf2[255], buf3[255];

CF_Window* win_title   = NULL;
CF_Window* win_list    = NULL;
CF_Window* win_preview = NULL;
CF_Window* win_msg     = NULL;
CF_Window* win_input   = NULL;

CF_Array* array = NULL;
CF_File* template_dir = NULL;

void init();

void window_cfft(CF_Window* cfw);
void window_list(CF_Window* cfw, CF_Array* array, int selected);
void window_preview(CF_Window* cfw, CF_File* cff);
void window_input(CF_Window* cfw, char* input_buffer, char* prompt);
void window_msg(CF_Window* cfw, const char* title, const char* msg, CF_Integer color_title, CF_Integer color_font);

int main(int argc, char *argv[]) {
    setlocale(LC_ALL, "");

    // Some init setting for ncurses
    initscr();
    cbreak();
    keypad(stdscr, TRUE);
    start_color();

    int selected = 0;
    int ch;
    CF_Bool done = CF_False;

    while (CF_True) {
        init();

        window_cfft(win_title);
        window_list(win_list, array, selected);
        window_preview(win_preview, CF_ARRAY_get(array, selected));

        curs_set(0);
        
        ch = getchar();

        // Press ESC
        if (ch == 27 || ch == (int) 'q') {
            done = CF_True;
        } else if (ch == (int)'j' || ch == (int)'J') {
            selected = (selected + 1) % array->size;
        } else if (ch == (int)'k' || ch == (int)'K') {
            selected = (selected - 1) % array->size;
        } else if (ch == (int)'\n' || ch == (int)'\r') {
            curs_set(1);
            do {
                memset(buf2, 0, sizeof(buf2));
                window_input(win_input, buf2, "Please Input File Name(Leave Empty to use default name)");
                destroy_win(win_input->win);

                CF_Bool ok = CF_True;

                if (strlen(buf2) == 0) {
                    // Use default file name.
                    strcpy(buf2, CF_ARRAY_get(array, selected)->basename);
                    delete_tag(buf2);
                } else {
                    // Use usr-input name
                    if (check_file_name(buf2) != CF_True) {
                        // User's input is not valid
                        ok = CF_False;

                        // Display warning
                        curs_set(0);
                        window_msg(win_msg, "Error", "Invalid File Name!", COLOR_PAIR_ERROR, COLOR_PAIR_ERROR);

                        // Hit any key to end warning wnidow
                        int c = getchar();
                        curs_set(1);
                        destroy_win(win_msg->win);
                    }
                }

                if (ok == CF_True) {
                    break;
                }
            } while (CF_True);

            CF_FILE_copy(CF_ARRAY_get(array, selected), buf2, CF_True);

            window_msg(win_msg, "Success", "Successfully to Create File!", COLOR_PAIR_SUCCESS, COLOR_PAIR_INFO);
            int c = getchar();
            destroy_win(win_msg->win);

            done = CF_True;
        } else {
            // do nothing
        }

        CF_WINDOW_free(&win_title);
        CF_WINDOW_free(&win_list);
        CF_WINDOW_free(&win_preview);


        if (done == CF_True) {
            break;
        }
    }

    endwin();

    return 0;
}

void init() {/*{{{*/

    PADDING_TOP = 3;
    PADDING_LEFT = 3;
    PADDING_RIGHT = 3;
    PADDING_BOTTOM = 5;
    MARGIN_INTERVAL = 1;

    // format: init_pair(index, fg, bg)
    init_pair(COLOR_PAIR_SUCCESS, COLOR_GREEN, COLOR_BG);
    init_pair(COLOR_PAIR_ERROR, COLOR_RED, COLOR_BG);
    init_pair(COLOR_PAIR_INFO, COLOR_CYAN, COLOR_BG);
    init_pair(COLOR_PAIR_WARNING, COLOR_YELLOW, COLOR_BG);
    
    win_title = CF_WINDOW_new(
            5,
            30,
            PADDING_TOP,
            PADDING_LEFT,
            COLOR_WHITE,
            COLOR_WHITE,
            NULL
        );

    win_list = CF_WINDOW_new(
            LINES - PADDING_TOP - win_title->rows - MARGIN_INTERVAL - PADDING_BOTTOM, 
            win_title->cols, 
            PADDING_TOP + win_title->rows + MARGIN_INTERVAL,
            PADDING_LEFT, 
            COLOR_WHITE,
            COLOR_WHITE,
            "List" 
        );

    win_preview = CF_WINDOW_new(
            LINES - PADDING_TOP - PADDING_BOTTOM,
            COLS - PADDING_LEFT - win_title->cols - PADDING_RIGHT,
            PADDING_TOP,
            PADDING_LEFT + win_title->cols + MARGIN_INTERVAL,
            COLOR_WHITE,
            COLOR_WHITE,
            "Preview"
        );
    win_input = CF_WINDOW_new(
            3, 
            COLS - PADDING_LEFT - PADDING_RIGHT - 5 * MARGIN_INTERVAL, 
            (LINES - 3) / 3,
            PADDING_LEFT + 5 * MARGIN_INTERVAL, 
            COLOR_WHITE,
            COLOR_WHITE,
            NULL
        );
    win_msg = CF_WINDOW_new(
            3, 
            win_input->cols, 
            win_input->start_row,
            win_input->start_col, 
            COLOR_WHITE,
            COLOR_WHITE,
            NULL
        );

    template_dir = CF_FILE_new(HOME_DIR, TEMPLATES_DIR);
    array = CF_ARRAY_new(DEFAULT_ARRAY_LENGTH);
    CF_FILE_list_directory(template_dir, CF_False, array);

    array->compare = str_compare;
    CF_ARRAY_sort(array);
}/*}}}*/

void window_cfft(CF_Window* cfw) {/*{{{*/
    cfw->win = create_newwin(cfw);
    char msg[31];

    wattron(cfw->win, COLOR_PAIR(COLOR_PAIR_SUCCESS));

    sprintf(msg, "CFFT version %s", VERSION);
    int len = strlen(msg);
    mvwprintw(cfw->win, 0, 2, "%s", msg);

    wattroff(cfw->win, COLOR_PAIR(COLOR_PAIR_SUCCESS));

    wrefresh(cfw->win);
}/*}}}*/

void window_list(CF_Window* cfw, CF_Array* array, int selected) {/*{{{*/
    cfw->win = create_newwin(cfw);

    char icon[5];
    
    for (int i = 0; i < array->size; i++) {
        get_icon(CF_ARRAY_get(array, i), icon);

        mvwprintw(cfw->win, i+1, 2, "%s", icon);
        
        if (i == selected) {
            wattron(cfw->win, A_REVERSE);
        }

        int name_len = strlen(CF_ARRAY_get(array, i)->basename);

        if (name_len > 22) {
            memcpy(buf1, CF_ARRAY_get(array, i)->basename, 19 * sizeof(char));
            buf1[19] = buf1[20] = buf1[21]  = '.';
            buf1[22] = '\0';
        } else {
            memcpy(buf1, CF_ARRAY_get(array, i)->basename, (name_len + 1) * sizeof(char));
        }

        mvwprintw(cfw->win, i+1, 6, "%-22s", buf1);

        if (i == selected) {
            wattroff(cfw->win, A_REVERSE);
        }
    }

    wrefresh(cfw->win);
}/*}}}*/

void window_preview(CF_Window* cfw, CF_File* cff) {/*{{{*/
    cfw->win = create_newwin(cfw);
    
    FILE* fin = NULL;
    fin = fopen(cff->fullpath, "r");

    int row = 1;
    while ((fgets(buf1, cfw->cols - 2, fin)) != NULL) {
        int len = strlen(buf1);
        if (buf1[len - 1] == '\n') {
            buf1[len - 1] = '\0';
        }

        mvwprintw(cfw->win, row++, 1, "%s", buf1);

        if (row >= cfw->rows - 2) {
            break;
        }
    }

    fclose(fin);

    wrefresh(cfw->win);
}/*}}}*/

void window_input(CF_Window* cfw, char* input_buffer, char* prompt) {/*{{{*/
    cfw->win = create_newwin(cfw);
    mvwprintw(cfw->win, 0, 2, "%s", prompt);
    wrefresh(cfw->win);

    mvwscanw(cfw->win, 1, 2, "%s", input_buffer);
    wrefresh(cfw->win);
}/*}}}*/

void window_msg(CF_Window* cfw, const char* title, const char* msg, CF_Integer color_title, CF_Integer color_font) {/*{{{*/
    cfw->win = create_newwin(cfw);

    wattron(cfw->win, COLOR_PAIR(color_title));
    mvwprintw(cfw->win, 0, 2, "%s", title);
    wattroff(cfw->win, COLOR_PAIR(color_title));

    wattron(cfw->win, COLOR_PAIR(color_font));
    mvwprintw(cfw->win, 1, 2, "%s", msg);
    wattroff(cfw->win, COLOR_PAIR(color_font));

    wrefresh(cfw->win);
}/*}}}*/
