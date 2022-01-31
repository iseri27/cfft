#include "cf.h"

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
CF_Bool is_hidden(CF_file* file);

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
#endif
