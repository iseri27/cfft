// #include "cf.h"
// #include "config.h"
// #include <stdio.h>
// #include <stdlib.h>
// #include <string.h>
//
// char buf1[255], buf2[255];
//
// void display_files(char** list, int n);
//
// int main(int argc, char *argv[]) {
    /**
     * 1. 列出所有的模板
     * 2. 选择一个模板
     * 3. 指定文件名
     * 4. 将模板拷贝到当前目录
     */
//
//     char **list;
//     int cnt = 0;
//     CF_Bool rc;
//
//     rc = list_directory(TEMPLATES_DIR, False, &cnt, list);
//
//     if (rc == False) {
//         fprintf(stderr, "Faliled to get file list in %s.", TEMPLATES_DIR);
//     }
//
//     display_files(list, cnt);
//
//     int chosen = -1;
//
//     fprintf(stdout, "Choose a file: ");
//     scanf("%d", &chosen);
//
//     // Invalid Number
//     if (chosen <= 0 || chosen > cnt) {
//         fprintf(stdout, "Invalid Number! Please Input a number between 1 and %d.\n", cnt);
//         wait_a_char();
//         exit(1);
//     }
//
//
//     strjoin(TEMPLATES_DIR, list[chosen-1], buf1);
//     copy_file(buf1, list[chosen-1], True);
//
//     wait_a_char();
//
//     return 0;
// }
//
// void display_files(char** list, int n) {
//     // sort before display
//     sort(list, n);
//     for (int i = 0; i < n; i++) {
//         if (n < 10) {
//             fprintf(stdout, "%d. %s\n", i+1, list[i]);
//         } else {
//             fprintf(stdout, "%02d. %s\n", i+1, list[i]);
//         }
//     }
// }
