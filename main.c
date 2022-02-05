#include "cf.h"
#include "utils.h"
#include "config.h"
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <locale.h>
#include <ncurses.h>

char buf1[255], buf2[255], buf3[255], errmsg[255];

CF_Window* win_cfft   = NULL;
CF_Window* win_list    = NULL;
CF_Window* win_preview = NULL;
CF_Window* win_msg     = NULL;
CF_Window* win_input   = NULL;

CF_Array* array = NULL;
CF_File* template_dir = NULL;

void init(int argc, char* argv[]);
CF_Bool check_env(char* errmsg);
void load_window();

void window_cfft(CF_Window* cfw);
void window_list(CF_Window* cfw, CF_Array* array, int selected);
void window_preview(CF_Window* cfw, CF_File* cff);
void window_input(CF_Window* cfw, char* input_buffer, char* prompt);
void window_msg(CF_Window* cfw, const char* title, const char* msg, CF_Integer color_title, CF_Integer color_font);

int main(int argc, char *argv[]) {
    init(argc, argv);

    while (check_env(buf1) != CF_True) {
        win_msg = CF_WINDOW_new(
                3, 
                COLS - PADDING_LEFT - PADDING_RIGHT - 5 * MARGIN_INTERVAL, 
                (LINES - 3) / 3,
                PADDING_LEFT + 5 * MARGIN_INTERVAL, 
                COLOR_WHITE,
                COLOR_WHITE,
                NULL
            );

        window_msg(win_msg, "Error", buf1, COLOR_PAIR_WARNING, COLOR_PAIR_ERROR);

        int ch = getchar();
        CF_WINDOW_free(&win_msg);

        // When press ESC, exit the program
        if (ch == 27) {
            curs_set(1);
            exit(0);
        }
    }

    memset(buf1, 0, sizeof(buf1));

    int selected = 0;
    int ch;
    CF_Bool done = CF_False;

    while (CF_True) {
        load_window();

        window_cfft(win_cfft);
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
                    if (check_file_name(buf2, errmsg) != CF_True) {
                        // User's input is not valid
                        ok = CF_False;

                        // Display warning
                        curs_set(0);
                        window_msg(win_msg, "Error", errmsg, COLOR_PAIR_ERROR, COLOR_PAIR_ERROR);

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

        CF_WINDOW_free(&win_cfft);
        CF_WINDOW_free(&win_list);
        CF_WINDOW_free(&win_preview);

        if (done == CF_True) {
            break;
        }
    }

    endwin();

    return 0;
}

void init(int argc, char* argv[]) {
    // Enable Characters except English
    setlocale(LC_ALL, "");

    // Some init setting for ncurses
    initscr();
    cbreak();
    keypad(stdscr, TRUE);
    start_color();

    // Define COLOR
    // init_color(COLOR_BLACK, 100, 100, 100);
    // init_color(COLOR_YELLOW, 255, 214,  102); // rgb(250, 144,  22)

    // format: init_pair(index, fg, bg)
    init_pair(COLOR_PAIR_SUCCESS, COLOR_GREEN , COLOR_BACKGROUND);
    init_pair(COLOR_PAIR_ERROR  , COLOR_RED   , COLOR_BACKGROUND);
    init_pair(COLOR_PAIR_INFO   , COLOR_CYAN  , COLOR_BACKGROUND);
    init_pair(COLOR_PAIR_WARNING, COLOR_YELLOW, COLOR_BACKGROUND);
}

CF_Bool check_env(char* errmsg) {
    char* cmd = (char*) calloc(31, sizeof(char));
    char* pwd = (char*) calloc(255, sizeof(char));
    char* tmp_buf = (char*) calloc(255, sizeof(tmp_buf));

    sprintf(cmd, "pwd");
    execute_cmd(cmd, pwd, 255);
    path_join(HOME_DIR, TEMPLATES_DIR, tmp_buf);

    if (strcmp(pwd, tmp_buf) == 0) {
        curs_set(0);
        sprintf(errmsg, "Error %d: CFFT Cannot Run Under TEMPLATE DIR. Please Press ESC to Exit.", ERROR_INVALID_CWD);
        free(tmp_buf);
        free(pwd);
        free(cmd);
        return CF_False;
    }

    // int row, col;
    // getmaxyx(stdscr, row, col);
    //
    // if (row < MIN_WINDOW_ROW) {
    //     sprintf(errmsg, "Error %d: Window height is too small!", ERROR_INVALID_HEIGHT);
    //     free(tmp_buf);
    //     free(pwd);
    //     free(cmd);
    //     return CF_False;
    // }
    //
    // if (col < MIN_WINDOW_COL) {
    //     sprintf(errmsg, "Error %d: Window width is too small!", ERROR_INVALID_WIDTH);
    //     free(tmp_buf);
    //     free(pwd);
    //     free(cmd);
    //     return CF_False;
    // }

    free(tmp_buf);
    free(pwd);
    free(cmd);
    return CF_True;
}

void load_window() {
    
    win_cfft = CF_WINDOW_new(
            8,
            30,
            PADDING_TOP,
            PADDING_LEFT,
            COLOR_WHITE,
            COLOR_WHITE,
            NULL
        );

    win_list = CF_WINDOW_new(
            LINES - PADDING_TOP - win_cfft->rows - MARGIN_INTERVAL - PADDING_BOTTOM, 
            win_cfft->cols, 
            PADDING_TOP + win_cfft->rows + MARGIN_INTERVAL,
            PADDING_LEFT, 
            COLOR_WHITE,
            COLOR_WHITE,
            "List" 
        );

    win_preview = CF_WINDOW_new(
            LINES - PADDING_TOP - PADDING_BOTTOM,
            COLS - PADDING_LEFT - win_cfft->cols - PADDING_RIGHT,
            PADDING_TOP,
            PADDING_LEFT + win_cfft->cols + MARGIN_INTERVAL,
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
}

void window_cfft(CF_Window* cfw) {
    cfw->win = create_newwin(cfw);
    char msg[31];

    wattron(cfw->win, COLOR_PAIR(COLOR_PAIR_SUCCESS));
    sprintf(msg, "CFFT version %s", VERSION);
    int len = strlen(msg);
    mvwprintw(cfw->win, 0, 2, "%s", msg);
    wattroff(cfw->win, COLOR_PAIR(COLOR_PAIR_SUCCESS));

    wattron(cfw->win, COLOR_PAIR(COLOR_PAIR_INFO));
    mvwprintw(cfw->win, 1, 2, "   ____ _____ _____ _____ ");
    mvwprintw(cfw->win, 2, 2, "  / ___|  ___|  ___|_   _|");
    mvwprintw(cfw->win, 3, 2, " | |   | |_  | |_    | |  ");
    mvwprintw(cfw->win, 4, 2, " | |___|  _| |  _|   | |  ");
    mvwprintw(cfw->win, 5, 2, "  \\____|_|   |_|     |_|  ");
    wattroff(cfw->win, COLOR_PAIR(COLOR_PAIR_INFO));

    wrefresh(cfw->win);
}

void window_list(CF_Window* cfw, CF_Array* array, int selected) {
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

        mvwprintw(cfw->win, i+1, 5, "%-22s", buf1);

        if (i == selected) {
            wattroff(cfw->win, A_REVERSE);
        }
    }

    wrefresh(cfw->win);
}

void window_preview(CF_Window* cfw, CF_File* cff) {
    if (cfw->cols < MIN_PREVIEW_WIN_WIDTH) {
        cfw->win = create_empty_win(cfw);
        return;
    }

    cfw->win = create_newwin(cfw);

    if (is_text_file(cff) == CF_True) {
        FILE* fin = NULL;
        fin = fopen(cff->fullpath, "r");

        int row = 1;
        while ((fgets(buf1, cfw->cols - 2, fin)) != NULL) {
            int len = strlen(buf1);
            if (buf1[len - 1] == '\n') {
                buf1[len - 1] = '\0';
            }

            if (row >= cfw->rows - 2) {
                break;
            }

            safe_wprint(cfw, row++, 2, buf1);
        }

        fclose(fin);
    } else {
        char* preview  = (char*) calloc(255, sizeof(char));
        char* cmd = (char*) calloc(255, sizeof(char));
        
        sprintf(cmd, "file \"%s\"", cff->fullpath);
        execute_cmd(cmd, preview, 255);

        wattron(cfw->win, COLOR_PAIR(COLOR_PAIR_INFO));
        mvwprintw(cfw->win, 1, 2, "%s", preview);
        wattroff(cfw->win, COLOR_PAIR(COLOR_PAIR_INFO));

        free(cmd);
        free(preview);
    }

    wrefresh(cfw->win);
}

void window_input(CF_Window* cfw, char* input_buffer, char* prompt) {
    cfw->win = create_newwin(cfw);
    mvwprintw(cfw->win, 0, 2, "%s", prompt);
    wrefresh(cfw->win);

    mvwscanw(cfw->win, 1, 2, "%[^\n]", input_buffer);
    wrefresh(cfw->win);
}

void window_msg(CF_Window* cfw, const char* title, const char* msg, CF_Integer color_title, CF_Integer color_font) {
    cfw->win = create_newwin(cfw);

    wattron(cfw->win, COLOR_PAIR(color_title));
    mvwprintw(cfw->win, 0, 2, "%s", title);
    wattroff(cfw->win, COLOR_PAIR(color_title));

    wattron(cfw->win, COLOR_PAIR(color_font));
    mvwprintw(cfw->win, 1, 2, "%s", msg);
    wattroff(cfw->win, COLOR_PAIR(color_font));

    wrefresh(cfw->win);
}
