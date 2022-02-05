#include "cf.h"
#include "utils.h"
#include "config.h"
#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include <string.h>
#include <ncurses.h>

/**********************
*      SOME UTILS     *
**********************/

char ubuf1[255], ubuf2[255];

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
 * Chekc whether has character in a string
 */
CF_Bool has_char(char* str, char ch) {
    int len = strlen(str);
    for (int i = 0; i < len; i++) {
        if (str[i] == ch) {
            return CF_True;
        }
    }
    return CF_False;
}

/**
 * Check file's name
 */
CF_Bool check_file_name(char* basename, char msg[]) {
    int len1 = strlen(INVALID_CHARS);
    
    // Check if has invalid character
    for (int i = 0; i < len1; i++) {
        if (has_char(basename, INVALID_CHARS[i])) {
            sprintf(msg, "Error %d: Invalid Character!", ERROR_INVALID_CHAR);
            return CF_False;
        }
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
 * Delete empty character in the beginning
 * and the end of a string
 */
void trim(char str[]) {
    int len = strlen(str);
    for (int i = len - 1; i >= 0; i--) {
        if (!isspace(str[i])) {
            break;
        }
        str[i] = '\0';
    }

    len = strlen(str);

    int k = 0;
    for (int i = 0; i < len; i++) {
        if (!isspace(str[i])) {
            break;
        }
        k++;
    }

    for (int i = 0; i < len - k; i++) {
        str[i] = str[i+k];
    }

    str[len - k] = '\0';
}

/**
 * Get icon of file
 */
void get_icon(CF_File* cfw, char icon[]) {
    strcpy(icon, "");
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
 * Execute external command
 */
CF_Bool execute_cmd(const char* cmd, char* result, int max_result_len) {

    FILE* fp = popen(cmd, "r");

    if (fp == NULL) {
         return CF_False;
    }

    fgets(result, max_result_len * sizeof(char), fp);
    trim(result);

    return CF_True;
}

/**
 * Check substring
 */
CF_Bool has_substring(const char* str, const char* sub) {
    const int len1 = strlen(str);
    const int len2 = strlen(sub);

    if (len2 > len1) {
        return CF_False;
    }

    for (int i = 0; i < len1; i++) {
        for (int j = 0; j < len2; j++) {
            if (i + j >= len1 || tolower(str[i+j]) != tolower(sub[j])) {
                break;
            }

            if (j == len2 - 1) {
                return CF_True;
            }
        }
    }

    return CF_False;
}

/**
 * Judge if is a text file
 */
CF_Bool is_text_file(CF_File* cff) {

    int max_result_len = 255;
    char* resultbuf = (char*) calloc(max_result_len, sizeof(char));
    char* cmd = (char*) calloc(255, sizeof(char));
    sprintf(cmd, "file %s", cff->fullpath);
    execute_cmd(cmd, resultbuf, max_result_len);

    CF_Bool ok = has_substring(resultbuf, "text");

    free(cmd);
    free(resultbuf);

    return ok;
}
