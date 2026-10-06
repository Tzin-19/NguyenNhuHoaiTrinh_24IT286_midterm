#include <stdio.h>
#include <stdlib.h>
#include "flags.h"
#include "file_info.h"
#include "sort.h"
#include "display.h"

int main(int argc, char *argv[])
{
    Options opts;
    int opt_ind = 1;

    if (parse_flags(argc, argv, &opts, &opt_ind) != 0)
    {
        return 1;
    }

    printf("my_ls program initialized successfully!\n");
    printf("Remaining arguments start at index: %d\n", opt_ind);

    return 0;
}