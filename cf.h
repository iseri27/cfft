#ifndef CFFT_H
#define CFFT_H

#define BUFFER_SIZE 31

typedef int CF_Size;
typedef int CF_Bool;
typedef int CF_Integer;
typedef int CF_Error;

static const CF_Bool True = 1;
static const CF_Bool False = 0;

typedef struct CF_file {
    char* path;
    char* basename;
    CF_Size size; // Bytes
    CF_Bool is_dir;
} CF_file;

/**********************
 *     SOME UTILS     *
 **********************/

/**
 * Just wait an input, do nothing
 */
void wait_a_char();

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
 * strcat
 */
int strjoin(const char* str1, const char* str2, char* dest);

/**
 * Sort strings
 */
void sort(char** strs, int n);

/**********************
 *   FILE OPERATION   *
 **********************/
/**
 * List all files in certain directory
 * @param path: directory to be listed
 * @param show_hidden: True for showing hidden files; False for else
 * @param cnt: file number that exists in target directory
 * @param list: used to store file names
 * @return True for Success; False for failed.
 */
CF_Bool list_directory(const char* path, CF_Bool show_hidden, CF_Integer* cnt, char** list);

/**
 * Copy a file
 */
CF_Bool copy_file(char* src, char* dest, CF_Bool force_copy);
#endif
