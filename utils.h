 
#ifndef UTILS_H
#define UTILS_H

typedef int CF_Size;
typedef int CF_Bool;
typedef int CF_Integer;

static const CF_Bool True = 1;
static const CF_Bool False = 0;

typedef struct CF_file {
    char* path; // 文件路径
    char* basename; // 文件名
    CF_Size size; // 文件的大小(字节)
    CF_Bool is_dir; // 是否是文件夹
} CF_file;

/**
 * 判断是否以某字符开头
 */
CF_Bool start_with_char(char* str, char cha);

/**
 * 判断是否是隐藏文件
 */
CF_Bool is_hidden(char* basename);

/**
 * 检查文件是否存在
 */
CF_Bool file_exists(char* path);

/**
 * 列出给定文件夹下的所有文件
 */
CF_Bool list_directory(char* path, CF_Bool show_hidden, CF_Integer* cnt, char** list);

/**
 * 复制文件
 */
CF_Bool copy_file(char* src, char* dest, CF_Bool force_copy);
#endif /* ifndef UTILS_H */
