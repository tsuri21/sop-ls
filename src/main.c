#include <stdio.h>
#include <unistd.h>
#include "options.h"
#include "list_core.h"

int main(int argc, char *argv[])
{
    //option flags
    Options opts = {
        .recursive = false,
        .human_readable = false,
        .show_size = false
    };
    int opt;
    char* target_path = ".";

    while ((opt = getopt(argc, argv, "-hrs")) != -1) {
        switch (opt) {
        case 'h':
            opts.human_readable = true;
            break;
        case 'r':
            opts.recursive = true;
            break;
        case 's':
            opts.show_size = true;
            break;
        case '?':
            fprintf(stderr, "Unknown option `-%c'.\n", optopt);
            return 1;
        default:
            break;
        }
    }
    if (optind < argc)
    {
        target_path = argv[optind];
    }


}