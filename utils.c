#include "utils.h"
#include "cf.h"
#include <ncurses.h>
#include <stdio.h>
#include <string.h>

/**********************
*      SOME UTILS     *
**********************/

/**
 * Just wait an input, do nothing
 */
void wait_a_char() {
    char ch = getchar();
}

/**
 * Check a string whether starts with a certain character.
 */
CF_Bool start_with_char(char* str, char cha) {
    return str[0] == cha ? CF_True : CF_False;
}

/**
 * Check whether is a hidden file
 */
CF_Bool is_hidden(CF_File* file) {

    return start_with_char(file->basename, '.');
}

/**
 * Check file whether exists
 */
CF_Bool is_exist(char* fullname) {
    FILE* fp = fopen(fullname, "rb");
    if (fp == NULL) {
        return CF_False;
    }
    fclose(fp);
    return CF_True;
}

/**
 * strcat
 */
int strjoin(const char* str1, const char* str2, char* dest) {
    int len1 = strlen(str1);
    int len2 = strlen(str2);

    strcpy(dest, str1);
    strcpy(dest+len1, str2);
    return len1 + len2;
}

/**
 * Join two path
 */
void path_join(const char* path1, const char* path2, char* resultbuf) {
    int len1 = strlen(path1);
    int len2 = strlen(path2);
    int len3 = 0;
    int k = 0;

    CF_Bool end_with = CF_False;
    CF_Bool start_with = CF_False;

    if (path1[len1-1] == SEP) {
        end_with = CF_True;
    }

    if (path2[0] == SEP) {
        start_with = CF_True;
    }

    memcpy(resultbuf, path1, sizeof(char) * len1);
    len3 = len1;

    if (start_with == CF_True && end_with == CF_True) {
        // k = 1;
        memcpy(&resultbuf[len1], path2+1, sizeof(char) * len2);
        len3 = len1 + len2 - 1;
    } else if (start_with == CF_False && end_with == CF_True) {
        memcpy(&resultbuf[len1], path2, sizeof(char) * (len2 + 1));
        len3 = len1 + len2;
    } else if (start_with == CF_True && end_with == CF_False) {
        memcpy(&resultbuf[len1], path2, sizeof(char) * (len2 + 1));
        len3 = len1 + len2;
    } else {
        resultbuf[len3++] = SEP;
        memcpy(&resultbuf[len1+1], path2, sizeof(char) * (len2 + 1));
        len3 = len1 + len2 + 1;
    }

    resultbuf[len3] = '\0';
}

/**
 * Create a new window
 */
WINDOW *create_newwin(CF_Window* cfw) {
    WINDOW* local_win;
    local_win = newwin(cfw->rows, cfw->cols, cfw->start_row, cfw->start_col);
    wrefresh(local_win);
    box(local_win, 0, 0);
    wrefresh(local_win);

    if (cfw->title != NULL) {
        mvwprintw(local_win, 0, 2, "%s", cfw->title);
    }

    return local_win;
}

/**
 * Destroy a window
 */
void destroy_win(WINDOW *local_win) {
    wborder(local_win, ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ');
    wrefresh(local_win);
    delwin(local_win);
}

/**
 * Check file's name
 */
CF_Bool check_file_name(char* basename) {
    if (basename[0] == 'a') {
        return CF_False;
    }
    return CF_True;
}

/**
 * Parse a string to Int
 */
CF_Integer parseInt(char* str) {
    CF_Integer n = 0;
    int len = strlen(str);
    for (int i = 0; i < len; i++) {
        n = n * 10 + str[i] - '0';
    }

    return n;
}

/**
 * Compare two strings
 */
int str_compare(CF_File* cff1, CF_File* cff2) {
    return strcmp(cff1->basename, cff2->basename);
}

/**
 * Get icon of file
 */
void get_icon(CF_File* cfw, char icon[]) {
    // strcpy(icon, "");
    strcpy(icon, "[F]");
}

/**
 * Delete tags in string
 */
void delete_tag(char* str) {
    const int len = strlen(str);

    int begin = -1, end = -1;

    for (int i = 1; i < len; i++) {
        if (str[i] == ']' && str[i-1] == ']') {
            end = i;
        }
    }

    for (int i = len - 2; i >= 0; i--) {
        if (str[i] == '[' && str[i+1] == '[') {
            begin = i;
        }
    }

    if (begin >= 0 && end >= 0 && end - begin >= 3 && end - begin + 1 < len) {
        int k = 0;
        for (int i = end + 1; i < len; i++, k++) {
            str[begin+k] = str[i];
        }
        str[begin+k] = '\0';
        for (int i = begin + k; i < len; i++) {
            str[i] = '\0';
        }
    }
}

/**
 * Judge if is a text file
 */
CF_Bool is_text_file(CF_File* cff) {
    return CF_True;
}
