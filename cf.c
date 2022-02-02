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
CF_File* CF_FILE_new_empty() {
    CF_File* cffp = (CF_File*) calloc(1, sizeof(CF_File));
    return cffp;
}

/**
 * Create a CF_file object
 */
CF_File* CF_FILE_new(const char* path, const char* basename) {
    CF_File* cffp = (CF_File*) calloc(1, sizeof(CF_File));
    const int len1 = strlen(path);
    const int len2 = strlen(basename);
    cffp->path = (char*) malloc((len1 + 1) * sizeof(char));
    cffp->basename = (char*) malloc((len2 + 1) * sizeof(char));
    cffp->fullpath = (char*) malloc((len1 + len2 + 2) * sizeof(char));

    memcpy(cffp->path, path, len1+1);
    memcpy(cffp->basename, basename, len2+1);
    path_join(path, basename, cffp->fullpath);

    return cffp;
}

/**
 * Create an array object
 */
CF_Array* CF_ARRAY_new(CF_Size size) {
    if (size < DEFAULT_ARRAY_LENGTH) {
        size = DEFAULT_ARRAY_LENGTH;
    }

    CF_Array* array = (CF_Array*) calloc(1, sizeof(CF_Array));
    array->capacity = size;
    array->size = 0;
    array->lt = (CF_File**) calloc(array->capacity, sizeof(CF_File*));

    return array;
}

/**
 * Add a CF_file object to array
 */
CF_Bool CF_ARRAY_add(CF_Array* arr, CF_File* cff) {
    if (arr->size == arr->capacity) {
        const int newlen = (int)(1.5 * arr->capacity);
        CF_File** lt = realloc(arr->lt, newlen);
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
CF_Bool CF_ARRAY_del(CF_Array* arr, CF_Integer index) {
    if (index < 0 || index >= arr->size) {
        return False;
    }

    for (int i = index; i < arr->size - 1; i++) {
        arr->lt[i] = arr->lt[i + 1];
    }

    arr->size--;

    return True;
}

/**
 * Get an element from array
 */
CF_File* CF_ARRAY_get(CF_Array* arr, CF_Integer index) {
    if (index < 0 || arr->size <= 0 || index >= arr->size) {
        return NULL; 
    }

    return arr->lt[index];
}

void CF_ARRAY_sort(CF_Array* arr) {
    int (*compare)(CF_File*, CF_File*);
    compare = arr->compare;

    for (int i = 0; i < arr->size; i++) {
        for (int j = 0; j < arr->size; j++) {
            if (compare(arr->lt[i], arr->lt[j]) > 0) {
                CF_File* tmp = arr->lt[i];
                arr->lt[i] = arr->lt[j];
                arr->lt[j] = tmp;
            }
        }
    }
}

/**
 * Clear the array
 */
void CF_ARRAY_clear(CF_Array* arr) {

}

/**
 * Delete an array and free its space
 */
void CF_ARRAY_free(CF_array* arr) {

}

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
void free_strings(CF_File* cff) {
    free_a_strp(cff->path);
    free_a_strp(cff->basename);
    free_a_strp(cff->fullpath);
}

/**
 * Release a CF_file pointer
 */
void CF_FILE_free(CF_file* cff) {
    // Release strings
    free_strings(cff);
    free(cff);
}

/**
 * Get file's detailed information by its path and basename
 */
CF_Bool CF_FILE_get_info(const char* path, const char* basename, CF_File* cff) {
    free_strings(cff);
    const int len1 = strlen(path);
    const int len2 = strlen(basename);
    cff->path = (char*) malloc(sizeof(char) * (len1 + 1));
    cff->basename = (char *) malloc(sizeof(char) * (len2 + 1));
    cff->fullpath = (char *) malloc((sizeof(char) * (len1 + len2 + 1)));

    strcpy(cff->path, path);
    strcpy(cff->basename, basename);
    path_join(path, basename, cff->fullpath);
    
    return True;
}

/**
 * List all files in certain directory
 * @param cffp: directory to be listed
 * @param show_hidden: True for showing hidden files; False for else
 * @param arr: used to store file names
 * @return True for Success; False for failed.
 */
CF_Bool CF_FILE_list_directory(CF_File* cffp, CF_Bool show_hidden, CF_Array* arr) {
    DIR *directory;
    struct dirent *dir;
    directory = opendir(cffp->fullpath);

    if (directory) {
        while ((dir = readdir(directory)) != NULL) {
            if (show_hidden == False && dir->d_name[0] == '.') {
                continue;
            }

            CF_File *fp = CF_FILE_new(cffp->fullpath, dir->d_name);
            CF_ARRAY_add(arr, fp);

        }
        closedir(directory);
    } else {
        return False;
    }

    return True;
}

/**
 * Copy a file
 */
CF_Bool CF_FILE_copy(CF_File* src, char* dest, CF_Bool force_copy) {
    FILE* fin;
    FILE* fout;
    
    // Check if file already exists.
    if (force_copy == False && is_exist(dest) == True) {
        return False;
    }

    fin = fopen(src->fullpath, "rb");
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
