#include <stdio.h>
#include "cf.h"
#include "utils.h"
#include "config.h"

char buf[255];
int main(int argc, char *argv[]) {

    char* path1 = "/home/corona/Templates";
    char* path2 = "Makefile[[Latex]]";
    for (int i = 0; i < 255; i++) {
        buf[i] = '1';
    }

    path_join(path1, path2, buf);

    printf("%s\n", buf);
    
    CF_array* array = CF_ARRAY_new(10);
    CF_file* template_dir = CF_FILE_new("/home/corona/", "/Templates");
    CF_FILE_list_directory(template_dir, False, array);

    for (int i = 0; i < array->size; i++) {
        printf("#%d:\n", i+1);
        printf("Path: %s\n", CF_ARRAY_get(array, i)->path);
        printf("Basename: %s\n", CF_ARRAY_get(array, i)->basename);
        printf("Fullpath: %s\n", CF_ARRAY_get(array, i)->fullpath);
        printf("\n");

        path_join("/home/corona/Downloads", CF_ARRAY_get(array, i)->basename, buf);

        // printf("%d: %s\n", i+1, buf);
        //
        CF_FILE_copy(CF_ARRAY_get(array, i), buf, True);
    }

    return 0;
}
