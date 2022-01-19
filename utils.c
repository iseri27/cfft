#include <stdio.h>
#include <dirent.h>
#include <string.h>
#include <stdlib.h>
#include "utils.h" 

/**
 * 判断是否以某字符开头
 */
CF_Bool start_with_char(char* str, char cha) {
    return str[0] == cha ? True : False;
}

/**
 * 判断是否是隐藏文件
 */
CF_Bool is_hidden(char* basename) {
    return start_with_char(basename, '.');
}

/**
 * 检查文件是否存在
 */
CF_Bool file_exists(char* path) {
    FILE* fp = fopen(path, "rb");
    if (fp == NULL) {
        fclose(fp);
        return False;
    }
    fclose(fp);
    return True;
}

/**
 * 列出给定文件夹下的所有文件
 */
CF_Bool list_directory(char* path, CF_Bool show_hidden, CF_Integer* cnt, char** list) {
    DIR *directory;
    struct dirent *dir;
    int n = 0, length = 0;
    directory = opendir(path);

    if (directory) {
        while ((dir = readdir(directory)) != NULL) {
            if (show_hidden == False && is_hidden(dir->d_name) == True) {
                continue;
            }
            // 存储字符串
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
 * 复制文件
 */
CF_Bool copy_file(char* src, char* dest, CF_Bool force_copy) {
    FILE* fin;
    FILE* fout;
    
    // 检查目标文件是否已经存在
    if (force_copy == False && file_exists(dest) == True) {
        return False;
    }

    fin = fopen(src, "rb");
    fout = fopen(dest, "wb");

    char buf[256];
    int len = 0;
    while ((len = fread(buf, 1, sizeof(buf), fin)) > 0) {
        fwrite(buf, 1, len, fout);
    }

    fclose(fin);
    fclose(fout);

    return True;
}
