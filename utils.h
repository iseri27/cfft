#include "cf.h"
#include <ncurses.h>

#ifndef CFFT_UTILS_H
#define CFFT_UTILS_H

// Character used to separate paths
static const char SEP = '/';

/**
 * Just wait an input, do nothing
 */
void wait_a_char();

/**
 * Check whether is a hidden file
 */
CF_Bool is_hidden(CF_File* file);

/**
 * Check file whether exists
 */
CF_Bool is_exist(char* path);

/**
 * strcat
 */
int strjoin(const char* str1, const char* str2, char* dest);

/**
 * Join two path
 */
void path_join(const char* path1, const char* path2, char* resultbuf);

/**
 * Create a new window
 */
WINDOW* create_newwin(CF_Window* cfw);

/**
 * Create an empty area
 */
WINDOW* create_empty_win(CF_Window* cfw);

/**
 * Destroy a window
 */
void destroy_win(WINDOW *local_win);

/**
 * Chekc whether has character in a string
 */
CF_Bool has_char(char* str, char ch);

/**
 * Check file's name
 */
CF_Bool check_file_name(char* basename, char msg[]);

/**
 * Parse a string to Int
 */
CF_Integer parseInt(char* str);

/**
 * Compare two strings
 */
int str_compare(CF_File*, CF_File*);

/**
 * Delete empty character in the beginning
 * and the end of a string
 */
void trim(char str[]);

/**
 * Get icon of file
 */
void get_number(int index, int max, char result[]);

/**
 * Delete tags in string
 */
void delete_tag(char* str);

/**
 * Execute external command
 */
CF_Bool execute_cmd(const char* cmd, char* result, int max_result_len);

/**
 * Check substring
 */
CF_Bool has_substring(const char* str, const char* sub);

/**
 * Judge if is a text file
 */
CF_Bool is_text_file(CF_File* cff);

/**
 * Cut string
 * Characters will be cut until its length 
 * will be less than **cut_len**
 * Strings after cut will be stored in dest
 */
void str_cut(char* dest, const char* src, const int cut_len);

/**
 * Safely(not beyond borders) print a string
 * on a window
 */
void safe_wprint(CF_Window* cfw, int row, int col, const char* str);
#endif
