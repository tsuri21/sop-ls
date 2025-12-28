#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include "options.h"
#define ANSI_COLOR_RED     "\x1b[31m"
#define ANSI_COLOR_GREEN   "\x1b[32m"
#define ANSI_COLOR_YELLOW  "\x1b[33m"
#define ANSI_COLOR_BLUE    "\x1b[34m"
#define ANSI_COLOR_MAGENTA "\x1b[35m"
#define ANSI_COLOR_CYAN    "\x1b[36m"
#define ANSI_COLOR_RESET   "\x1b[0m"
#define ANSI_COLOR_BOLD    "\x1b[1m"


static void printColoring(const struct stat *info)
{
    if (S_ISDIR(info->st_mode)) printf(ANSI_COLOR_BLUE);
    else if (info->st_mode & S_IXUSR || info->st_mode & S_IXGRP || info->st_mode & S_IXOTH) printf(ANSI_COLOR_RED);
}

static void printHumanReadable(const int depth, const struct stat *info)
{
    for (int i = 0; i<depth; i++) printf("\t");
    if (S_ISDIR(info->st_mode)) printf("\u2514\u2500\u2500 ");
    else printf("\u251c\u2500\u2500 ");
}

static void printSize(const struct stat *info, const char* unit)
{
    if (S_ISDIR(info->st_mode)) return;
    char* units[] = {"B", "KB", "MB", "GB", "TB"};

    bool unitsContainUnit = false;
    int counter = 0;
    while (units[counter] != NULL && strcmp(unit, "") != 0)
    {
        if (strcmp(units[counter], unit) == 0)
        {
            unitsContainUnit = true;
            break;
        }
        counter++;
    }

    double size = (double)info->st_size;
    int unitIndex = 0;
    while (unitIndex < sizeof(units)/sizeof(units[0]))
    {
        if ((size < 1024 && !unitsContainUnit) || strcmp(units[unitIndex], unit) == 0) break;
        size /= 1024;
        unitIndex++;
    }
    printf(ANSI_COLOR_MAGENTA "%.2f%s", size, units[unitIndex]);
}

static void printContent(const char* fullPath, const struct stat *info, const Options opts, const int depth)
{
    if (opts.c_counter == 0 || S_ISDIR(info->st_mode)) return;

    int maxLineCount = opts.c_counter < 0 ? 5 : opts.c_counter;

    FILE* file = fopen(fullPath, "r");
    if (!file) return;

    unsigned char checkBuffer[4096];
    const size_t  bytesRead = fread(checkBuffer, 1, sizeof(checkBuffer), file);
    if (bytesRead == 0)
    {
        fclose(file);
        return;
    }
    for (size_t i = 0; i<bytesRead; i++)
    {
        if (checkBuffer[i] == '\0' || (checkBuffer[i] < 7) || (checkBuffer[i] > 14 && checkBuffer[i] < 32))
        {
            fclose(file);
            return;
        }
    }
    rewind(file);

    const int boxLength = 40;
    if (opts.human_readable)
    {
        //printing the content box
        for (int i = 0; i < depth; i++) printf("\t");
        printf(ANSI_COLOR_CYAN "┌");
        for (int i = 0; i <= boxLength; i++) printf(ANSI_COLOR_CYAN "─");
        printf(ANSI_COLOR_CYAN "┐\n");

        for (int i = 0; i < depth; i++) printf("\t");
        printf(ANSI_COLOR_CYAN "│" ANSI_COLOR_YELLOW " Content Preview ");
        for (int i = 0; i <= boxLength - strlen(" Content Preview "); i++) printf(" ");
        printf(ANSI_COLOR_CYAN "│\n");

        for (int i = 0; i < depth; i++) printf("\t");
        printf(ANSI_COLOR_CYAN "├");
        for (int i = 0; i <= boxLength; i++) printf("─");
        printf(ANSI_COLOR_CYAN "┤\n");
    }

    char line[512];
    while (maxLineCount != 0 && fgets(line, sizeof(line), file))
    {
        if (opts.human_readable) for (int i = 0; i <= depth; i++) printf("\t");
        else printf("\t");
        printf(ANSI_COLOR_GREEN "%.32s%s", line, (strlen(line) > 32 ? ANSI_COLOR_RED "...\n" : ""));
        if (line[strlen(line)-1] != '\n') printf("\n");
        maxLineCount--;
    }
    fclose(file);
    if (opts.human_readable)
    {
        //printing content box
        for (int i = 0; i < depth; i++) printf("\t");
        printf(ANSI_COLOR_CYAN "┗");
        for (int i = 0; i <= boxLength; i++) printf("━");
        printf(ANSI_COLOR_CYAN "┛\n");
    }
}

void printUtils(const char* fullPath ,const char* fileName, const struct stat *info, const Options opts, const int depth)
{
    printColoring(info);
    if (opts.human_readable) printHumanReadable(depth, info);
    printf("%s ", fileName);
    if (opts.show_size) printSize(info, opts.unit);
    printf("\n");
    if (opts.show_content) printContent(fullPath, info, opts, depth);
    printf(ANSI_COLOR_RESET);
}