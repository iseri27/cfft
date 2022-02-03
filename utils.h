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
WINDOW *create_newwin(int height, int width, int starty, int startx);

/**
 * Destroy a window
 */
void destroy_win(WINDOW *local_win);
#endif
