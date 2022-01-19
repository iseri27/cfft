#include "cf.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char buf1[255], buf2[255];

int main(int argc, char *argv[]) {
    char* src = "/home/corona/Downloads/";
    char* dest = "/home/corona/Templates/";

    CF_Bool show_hidden = False;
    CF_Integer cnt = 0;
    char **list;

    list_directory(src, show_hidden, &cnt, list);

    printf("%d\n", cnt);

    for (int i = 0; i < cnt; i++) {
        strjoin(src, list[i], buf1);
        strjoin(dest, list[i], buf2);

        printf("Src File: %s\n", buf1);
        printf("Dst File: %s\n", buf2);

        CF_Bool ok = copy_file(buf1, buf2, True);
        if (ok == True) {
            printf("Successfully to Copy.\n\n");
        } else {
            printf("Failed to Copy.\n\n");
        }
    }

    for (int i = 0; i < cnt; i++) {
        free(list[i]);
    }
    return 0;
}

