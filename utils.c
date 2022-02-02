#include "utils.h"
#include "cf.h"
#include <stdio.h>
#include <string.h>

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
CF_Bool is_hidden(CF_File* file) {

    return start_with_char(file->basename, '.');
}

/**
 * Check file whether exists
 */
CF_Bool is_exist(char* fullname) {
    FILE* fp = fopen(fullname, "rb");
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
 * Join two path
 */
void path_join(const char* path1, const char* path2, char* resultbuf) {
    int len1 = strlen(path1);
    int len2 = strlen(path2);
    int len3 = 0;
    int k = 0;

    CF_Bool end_with = False;
    CF_Bool start_with = False;

    if (path1[len1-1] == SEP) {
        end_with = True;
    }

    if (path2[0] == SEP) {
        start_with = True;
    }

    memcpy(resultbuf, path1, sizeof(char) * len1);
    len3 = len1;

    if (start_with == True && end_with == True) {
        // k = 1;
        memcpy(&resultbuf[len1], path2+1, sizeof(char) * len2);
        len3 = len1 + len2 - 1;
    } else if (start_with == False && end_with == True) {
        memcpy(&resultbuf[len1], path2, sizeof(char) * (len2 + 1));
        len3 = len1 + len2;
    } else if (start_with == True && end_with == False) {
        memcpy(&resultbuf[len1], path2, sizeof(char) * (len2 + 1));
        len3 = len1 + len2;
    } else {
        resultbuf[len3++] = SEP;
        memcpy(&resultbuf[len1+1], path2, sizeof(char) * (len2 + 1));
        len3 = len1 + len2 + 1;
    }

    // for (; k < len2;) {
    //     resultbuf[len3++] = path2[k++];
    // }

    resultbuf[len3] = '\0';
}
