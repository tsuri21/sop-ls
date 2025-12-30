#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "options.h"
#include "list_core.h"

int main(const int argc, char *argv[])
{
    //option flags
    Options opts = {
        .recursive = false,
        .human_readable = false,
        .show_size = false,
        .show_all = false
    };
    char* target_path = ".";

    for (int i =1; i < argc; i++)
    {
        if (argv[i][0] != '-')
        {
            target_path = argv[i];
            continue;
        }

        char optionLetter = argv[i][1];
        int optionCounter = -1;
        char* optionString = "";

        if (strlen(argv[i]) > 3)
        {
            optionString = argv[i];
            optionString += 3;
            optionCounter = atoi(optionString);
        }
        switch (optionLetter)
        {
        case 'h':
            opts.human_readable = true;
            continue;

        case 's':
            opts.show_size = true;
            opts.unit = optionString;
            continue;

        case 'r':
            opts.recursive = true;
            opts.r_counter = optionCounter;
            continue;

        case 'c':
            opts.show_content = true;
            opts.c_counter = optionCounter;
            continue;

        case 'a':
            opts.show_all = true;
            continue;

        default:
            printf("unknown option\n");
            return 1;
        }
    }
    traverse_directory(target_path, opts);
}