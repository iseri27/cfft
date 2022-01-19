#include "utils.h"
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[]) {
    char* src = "/home/corona/Templates";
    char* dest = "/home/corona/Downloads";
    CF_Bool show_hidden = False;
    CF_Integer cnt = 0;
    char **list;

    list_directory(src, show_hidden, &cnt, list);

    printf("%d\n", cnt);

    for (int i = 0; i < cnt; i++) {
    }

    for (int i = 0; i < cnt; i++) {
        free(list[i]);
    }
    return 0;
}

