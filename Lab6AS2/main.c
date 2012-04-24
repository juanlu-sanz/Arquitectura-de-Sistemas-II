#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "actions.h"
#include "fatlib.h"


int main(int argc, char* argv[]) {
    int i;
    int* fat;
    char* fs;
    DirEntry* dirTable;

    fat = init_fat();
    fs = init_fs();
    dirTable = init_directory_table();

    for (i = 0; i < NUMBER_OF_ACTIONS; i++) {
        process(fs, fat, dirTable, actions[3*i], actions[3*i+1], actions[3*i+2]);
    }
    dump_fs(fs, "result.fs");
    dump_fat(fat);


    free(dirTable);
    free(fs);
    free(fat);
    return 0;
}
