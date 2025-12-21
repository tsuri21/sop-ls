#include <stdio.h>
#include <sys/stat.h>
#include "options.h"
#define ANSI_COLOR_BLUE    "\x1b[34m"
#define ANSI_COLOR_GREEN   "\x1b[32m"
#define ANSI_COLOR_CYAN    "\x1b[36m"
#define ANSI_COLOR_RED     "\x1b[31m"
#define ANSI_COLOR_RESET   "\x1b[0m"
#define ANSI_BOLD          "\x1b[1m"


static void printColoring(struct stat *info)
{
    if (S_ISDIR(info->st_mode)) printf(ANSI_COLOR_BLUE);
    else if (info->st_mode & S_IXUSR || info->st_mode & S_IXGRP || info->st_mode & S_IXOTH) printf(ANSI_COLOR_RED);
}

static void printHumanReadable(int depth, struct stat *info)
{
    for (int i = 0; i<depth; i++) printf("\t");
    if (S_ISDIR(info->st_mode)) printf("\u2514\u2500\u2500 ");
    else printf("\u251c\u2500\u2500 ");
}

static void printSize(struct stat *info)
{
    double size = info->st_size;
    char* units[] = {"B", "KB", "MB", "GB", "TB"};
    int unitIndex = 0;
    while (size >= 1024 && unitIndex < sizeof(units)/sizeof(units[0]))
    {
        size /= 1024;
        unitIndex++;
    }
    if (!S_ISDIR(info->st_mode)) printf("%.2f%s", size, units[unitIndex]);

}

void printUtils(const char* fileName, struct stat *info, Options opts, int depth)
{
    printColoring(info);
    if (opts.human_readable) printHumanReadable(depth, info);
    printf("%s ", fileName);
    if (opts.show_size) printSize(info);
    printf("\n");
    printf(ANSI_COLOR_RESET);
}