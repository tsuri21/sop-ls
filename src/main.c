#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "options.h"
#include "list_core.h"

#ifdef _WIN32
#include <windows.h> // Wird für SetConsoleOutputCP und SetConsoleMode benötigt
#endif

int main(const int argc, char *argv[])
{
    // --- Windows-spezifische Einstellungen START ---
#ifdef _WIN32
    // 1. Terminal auf UTF-8 umstellen (behebt die Ôö£ÔöÇ Zeichen)
    SetConsoleOutputCP(65001);

    // 2. ANSI-Farbcodes aktivieren (behebt die ←[0m Zeichen)
    HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
    if (hOut != INVALID_HANDLE_VALUE) {
        DWORD dwMode = 0;
        if (GetConsoleMode(hOut, &dwMode)) {
            dwMode |= 0x0004; // ENABLE_VIRTUAL_TERMINAL_PROCESSING
            SetConsoleMode(hOut, dwMode);
        }
    }
#endif
    //option flags
    Options opts = {
        .recursive = false,
        .human_readable = false,
        .show_size = false,
        .show_all = false,
        .show_permissions = false
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

        case 'p':
            opts.show_permissions = true;
            opts.p_counter = optionCounter;
            continue;

        default:
            printf("unknown option\n");
            return 1;
        }
    }
    traverse_directory(target_path, opts);
}