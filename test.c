#include <stdio.h>
#include "utils.h"

char buf[255];
int main(int argc, char *argv[]) {
    char* path1 = "/home/corona/模板";
    char* path2 = "notes/cg";

    path_join(path1, path2, buf);

    printf("%s\n", buf);
    return 0;
}
