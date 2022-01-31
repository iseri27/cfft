#include <stdio.h>
#include <dirent.h>
#include <string.h>
#include <stdlib.h>
#include "cf.h" 
#include "utils.h"

/**********************
 *  ARRAY OPERATION   *
 **********************/
/**
 * Create a CF_file object
 */
CF_file* CF_new_file() {
    CF_file* cffp = (CF_file*) calloc(1, sizeof(CF_file));
    return cffp;
}

/**
 * Add a CF_file object to array
 */
CF_Bool CF_add(CF_array* arr, CF_file* cff) {
    if (arr->size == arr->capacity) {
        const int newlen = (int)(1.5 * arr->capacity);
        CF_file** lt = realloc(arr->lt, newlen);
        if (lt == NULL) {
            return False;
        } else {
            arr->capacity = newlen;
        }

    }

    arr->lt[arr->size++] = cff;
    return True;
}

/**
 * Delete an element
 */
CF_Bool CF_del(CF_array* arr, CF_Integer index) {
    if (index < 0 || index >= arr->size) {
        return False;
    }

    for (int i = index; i < arr->size - 1; i++) {
        arr->lt[i] = arr->lt[i + 1];
    }

    return True;
}

/**
 * Get an element from array
 */
CF_file* CF_get(CF_array* arr, CF_Integer index) {
    if (index < 0 || arr->size <= 0 || index >= arr->size) {
        return NULL; 
    }

    return arr->lt[index];
}

void CF_sort(CF_array* arr) {
    int (*compare)(CF_file*, CF_file*);
    compare = arr->compare;

    for (int i = 0; i < arr->size; i++) {
        for (int j = 0; j < arr->size; j++) {
            if (compare(arr->lt[i], arr->lt[j]) > 0) {
                CF_file* tmp = arr->lt[i];
                arr->lt[i] = arr->lt[j];
                arr->lt[j] = tmp;
            }
        }
    }
}

/**
 * Clear the array
 */
void CF_clear(CF_array* arr);

/**********************
 *   FILE OPERATION   *
 **********************/
/**
 * Release a string pointer
 * **NOT DEFIINED IN cf.h**
 */
void free_a_strp(char* str) {
    if (str != NULL) {
        free(str);
    }
}

/**
 * Release string pointers of a CF_file pointer
 * **NOT DEFIINED IN cf.h**
 */
void CF_free_strings(CF_file* cff) {
    free_a_strp(cff->path);
    free_a_strp(cff->basename);
    free_a_strp(cff->fullname);
}

/**
 * Release a CF_file pointer
 */
void CF_free(CF_file* cff) {
    // Release strings
    CF_free_strings(cff);
    free(cff);
}

/**
 * Get file's detailed information by its path and basename
 */
CF_Bool get_file_info(const char* path, const char* basename, CF_file* cff) {
    CF_free_strings(cff);
    const int len1 = strlen(path);
    const int len2 = strlen(basename);
    cff->path = (char*) malloc(sizeof(char) * (len1 + 1));
    cff->basename = (char *) malloc(sizeof(char) * (len2 + 1));
    cff->fullname = (char *) malloc((sizeof(char) * (len1 + len2 + 1)));

    strcpy(cff->path, path);
    strcpy(cff->basename, basename);
    path_join(path, basename, cff->fullname);
    
    return True;
}

/**
 * List all files in certain directory
 * @param cffp: directory to be listed
 * @param show_hidden: True for showing hidden files; False for else
 * @param arr: used to store file names
 * @return True for Success; False for failed.
 */
CF_Bool list_directory(CF_file* cffp, CF_Bool show_hidden, CF_array* arr) {
    DIR *directory;
    struct dirent *dir;
    int n = 0, length = 0;
    directory = opendir(cffp->fullname);

    if (directory) {
        while ((dir = readdir(directory)) != NULL) {
            if (show_hidden == False && is_hidden(dir->d_name) == True) {
                continue;
            }

            length = strlen(dir->d_name);
            list[n] = (char*) malloc(length * sizeof(char));
            sprintf(list[n], "%s", dir->d_name);
            n++;
        }
        closedir(directory);
    } else {
        return False;
    }

    *cnt = n;
    return True;
}

/**
 * Copy a file
 */
CF_Bool copy_file(char* src, char* dest, CF_Bool force_copy) {
    FILE* fin;
    FILE* fout;
    
    // Check if file already exists.
    if (force_copy == False && file_exists(dest) == True) {
        return False;
    }

    fin = fopen(src, "rb");
    fout = fopen(dest, "wb");

    char buf[BUFFER_SIZE];
    int len = 0;
    while ((len = fread(buf, 1, sizeof(buf), fin)) > 0) {
        fwrite(buf, 1, len, fout);
    }

    fclose(fin);
    fclose(fout);

    return True;
}
