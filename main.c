#include "cf.h"
#include "config.h"
#include "utils.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char buf1[255], buf2[255];

void display_files(CF_array*);
int compare(CF_file*, CF_file*);

int main(int argc, char *argv[]) {

    CF_array* array = CF_ARRAY_new(DEFAULT_ARRAY_LENGTH);
    CF_file* template_dir = CF_FILE_new(HOME_DIR, TEMPLATES_DIR);

    CF_Bool rc;

    rc = CF_FILE_list_directory(template_dir, False, array);

    if (rc == False) {
        fprintf(stderr, "Faliled to get file list in %s.", template_dir->fullpath);
    }

    array->compare = compare;
    CF_ARRAY_sort(array);

    display_files(array);

    int chosen = -1;

    fprintf(stdout, "Choose a file: ");
    scanf("%d", &chosen);

    // Invalid Number
    if (chosen <= 0 || chosen > array->size) {
        fprintf(stdout, "Invalid Number! Please Input a number between 1 and %d.\n", array->size);
        wait_a_char();
        exit(1);
    }

    chosen = chosen - 1;

    CF_FILE_copy(CF_ARRAY_get(array, chosen), 
            CF_ARRAY_get(array, chosen)->basename, True);

    return 0;
}

void display_files(CF_array* array) {

    for (int i = 0; i < array->size; i++) {
        if (array->size < 10) {
            fprintf(stdout, "%d. %s\n", i+1, CF_ARRAY_get(array, i)->basename);
        } else {
            fprintf(stdout, "%02d. %s\n", i+1, CF_ARRAY_get(array, i)->basename);
        }
    }
}

int compare(CF_file* cff1, CF_file* cff2) {
    return strcmp(cff1->basename, cff2->basename);
}
