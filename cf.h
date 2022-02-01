#ifndef CFFT_H
#define CFFT_H

#include <strings.h>
#define BUFFER_SIZE 31
#define DEFAULT_ARRAY_LENGTH 10

typedef unsigned int CF_Size;
typedef int CF_Bool;
typedef int CF_Integer;
typedef int CF_Error;

static const CF_Bool True = 1;
static const CF_Bool False = 0;

typedef struct CF_file {
    char* path;
    char* basename;
    char* fullpath;
    CF_Size size; // Bytes
    CF_Bool is_dir;
} CF_file;

typedef struct CF_array {
    CF_file** lt;
    CF_Size size;
    CF_Size capacity;
    CF_Integer (*compare)(CF_file*, CF_file*);
} CF_array;

/**********************
 *  ARRAY OPERATION   *
 **********************/
/**
 * Create a CF_file object
 */
CF_file* CF_FILE_new_empty();

/**
 * Create a CF_file object
 */
CF_file* CF_FILE_new(const char* path, const char* basename);

/**
 * Create an array object
 */
CF_array* CF_ARRAY_new(CF_Size size);

/**
 * Add a CF_file object to array
 */
CF_Bool CF_ARRAY_add(CF_array* arr, CF_file* cff);

/**
 * Delete an element
 */
CF_Bool CF_ARRAY_del(CF_array* arr, CF_Integer index);

/**
 * Get an element from array
 */
CF_file* CF_ARRAY_get(CF_array* arr, CF_Integer index);

/**
 * Sort an array
 */
void CF_ARRAY_sort(CF_array* arr);

/**
 * Clear the array
 */
void CF_ARRAY_clear(CF_array* arr);

/**********************
 *   FILE OPERATION   *
 **********************/
/**
 * Release a CF_file pointer
 */
void CF_FILE_free(CF_file* cff);

/**
 * Get file's detailed information by its path and basename.
 */
CF_Bool CF_FILE_get_info(const char* path, const char* basename, CF_file* file);

/**
 * List all files in certain directory
 * @param cffp: directory to be listed
 * @param show_hidden: True for showing hidden files; False for else
 * @param arr: used to store file names
 * @return True for Success; False for failed.
 */
CF_Bool CF_FILE_list_directory(CF_file* cff, CF_Bool show_hidden, CF_array* arr);

/**
 * Copy a file
 */
CF_Bool CF_FILE_copy(CF_file* cff, char* dest, CF_Bool force_copy);
#endif
