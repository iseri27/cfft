#ifndef CFFT_H
#define CFFT_H

typedef int CF_Size;
typedef int CF_Bool;
typedef int CF_Integer;

static const CF_Bool True = 1;
static const CF_Bool False = 0;

typedef struct CF_file {
    char* path;
    char* basename;
    CF_Size size; // Bytes
    CF_Bool is_dir;
} CF_file;

/**********************
*      SOME UTILS     *
**********************/

/**
 * Check a string whether starts with a certain character.
 */
CF_Bool start_with_char(char* str, char cha);

/**
 * Check whether is a hidden file
 */
CF_Bool is_hidden(char* basename);

/**
 * Check file whether exists
 */
CF_Bool file_exists(char* path);

/**
 * List all files in certain directory
 */
CF_Bool list_directory(char* path, CF_Bool show_hidden, CF_Integer* cnt, char** list);

/**
 * Copy a file
 */
CF_Bool copy_file(char* src, char* dest, CF_Bool force_copy);
#endif
