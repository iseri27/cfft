#include <ncurses.h>
#include <strings.h>

#ifndef CFFT_H
#define CFFT_H

#define BUFFER_SIZE 31
#define DEFAULT_ARRAY_LENGTH 10

typedef unsigned int CF_Size;
typedef int CF_Bool;
typedef int CF_Integer;
typedef int CF_Error;

static const CF_Bool CF_True = 1;
static const CF_Bool CF_False = 0;

typedef struct CF_File {
    char* path;
    char* basename;
    char* fullpath;
    CF_Size size; // Bytes
    CF_Bool is_dir;
} CF_File;

typedef struct CF_Array {
    CF_File** lt;
    CF_Size size;
    CF_Size capacity;
    CF_Integer (*compare)(CF_File*, CF_File*);
} CF_Array;

typedef struct CF_Window {
    CF_Integer cols, rows;
    CF_Integer start_col, start_row;
    CF_Integer color_border, color_content, color_title;
    WINDOW* win;
    char* title;
} CF_Window;

/**********************
 *  WINDOW OPERATION  *
 **********************/
/**
 * Create a window struct
 */
CF_Window* CF_WINDOW_new(
        CF_Integer rows,
        CF_Integer cols,
        CF_Integer start_row,
        CF_Integer start_col,
        CF_Integer color_border,
        CF_Integer color_content,
        CF_Integer color_title,
        const char* title
    );

/**
 * Release an CF_Window pointer
 * and free its spaces
 */
void CF_WINDOW_free(CF_Window** cfw);

/**********************
 *  ARRAY OPERATION   *
 **********************/
/**
 * Create an array object
 */
CF_Array* CF_ARRAY_new(CF_Size size);

/**
 * Add a CF_file object to array
 */
CF_Bool CF_ARRAY_add(CF_Array* arr, CF_File* cff);

/**
 * Delete an element
 */
CF_Bool CF_ARRAY_del(CF_Array* arr, CF_Integer index);

/**
 * Get an element from array
 */
CF_File* CF_ARRAY_get(CF_Array* arr, CF_Integer index);

/**
 * Sort an array
 */
void CF_ARRAY_sort(CF_Array* arr);

/**
 * Clear the array
 */
void CF_ARRAY_clear(CF_Array* arr);

/**
 * Release an CF_Array pointer
 * and free its spaces
 */
void CF_ARRAY_free(CF_Array** arr);

/**********************
 *   FILE OPERATION   *
 **********************/
/**
 * Create a CF_file object
 */
CF_File* CF_FILE_new_empty();

/**
 * Create a CF_file object
 */
CF_File* CF_FILE_new(const char* path, const char* basename);

/**
 * Get file's detailed information by its path and basename.
 */
CF_Bool CF_FILE_get_info(const char* path, const char* basename, CF_File* file);

/**
 * List all files in certain directory
 * @param cffp: directory to be listed
 * @param show_hidden: True for showing hidden files; False for else
 * @param arr: used to store file names
 * @return True for Success; False for failed.
 */
CF_Bool CF_FILE_list_directory(CF_File* cff, CF_Bool show_hidden, CF_Array* arr);

/**
 * Copy a file
 */
CF_Bool CF_FILE_copy(CF_File* cff, char* dest, CF_Bool force_copy);

/**
 * Release a CF_File pointer
 * and free its spaces
 */
void CF_FILE_free(CF_File** cff);
#endif
