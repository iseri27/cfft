#include <stdio.h>
#include <dirent.h>
#include <string.h>
#include <stdlib.h>
#include "cf.h" 

/**********************
*      SOME UTILS     *
**********************/

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
    return str[0] == cha ? True : False;
}

/**
 * Check whether is a hidden file
 */
CF_Bool is_hidden(char* basename) {
    return start_with_char(basename, '.');
}

/**
 * Check file whether exists
 */
CF_Bool file_exists(char* path) {
    FILE* fp = fopen(path, "rb");
    if (fp == NULL) {
        return False;
    }
    fclose(fp);
    return True;
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
 * Sort strings
 */
void sort(char** strs, int n) {
    // Just do nothing now.
}

/**********************
*   FILE OPERATIONS   *
**********************/
/**
 * List all files in certain directory
 */
CF_Bool list_directory(const char* path, CF_Bool show_hidden, CF_Integer* cnt, char** list) {
    DIR *directory;
    struct dirent *dir;
    int n = 0, length = 0;
    directory = opendir(path);

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
