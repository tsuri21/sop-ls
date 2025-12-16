#include <stdio.h>
#include "options.h"
#define ANSI_COLOR_BLUE    "\x1b[34m"
#define ANSI_COLOR_GREEN   "\x1b[32m"
#define ANSI_COLOR_CYAN    "\x1b[36m"
#define ANSI_COLOR_RESET   "\x1b[0m"
#define ANSI_BOLD          "\x1b[1m"


void printFile(const char* fileName, Options opts)
{
    printf("\u251c\u2500\u2500 %s \n", fileName);
}

void printFileWithDepth(const char* fileName, Options opts, int depth)
{
    for (int i = 0; i<depth; i++) printf("\t");
    printf("\u251c\u2500\u2500 %s \n", fileName);
}

void printDirectory(const char* fileName, Options opts)
{
    printf("\u251c\u2500\u2500 %s (D) \n", fileName);
}

void printDirectoryWithDepth(const char* fileName, Options opts, int depth)
{
    for (int i = 0; i<depth; i++) printf("\t");
    printf(ANSI_COLOR_BLUE "\u2514\u2500\u2500 %s (D) \n" ANSI_COLOR_RESET, fileName);
}