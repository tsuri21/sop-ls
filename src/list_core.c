#include "options.h"
#include <sys/stat.h>
#include <dirent.h>
#include <stdio.h>
#include <string.h>
#include <limits.h>

#include "output_utils.h"

static void traverse_recursive(const char* path, const Options opts, const int depth)
{
    if (depth > opts.r_counter && opts.r_counter >= 0) return;
    struct stat info;
    char fullPath[PATH_MAX];
    DIR* dir = opendir(path);
    struct dirent* entry;

    if (dir == NULL)
    {
        perror("opendir");
        return;
    }

    while ((entry = readdir(dir)) != NULL)
    {
        if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0) continue;
        if (entry->d_name[0] == '.' && !opts.show_all) continue;
        snprintf(fullPath, PATH_MAX, "%s/%s", path, entry->d_name);
        stat(fullPath, &info);

        printUtils(fullPath, entry->d_name, &info, opts, depth);
        if (S_ISDIR(info.st_mode)) traverse_recursive(fullPath, opts, depth+1);
    }
    closedir(dir);
}

void traverse_directory(const char* path, const Options opts)
{
    if (opts.recursive)
    {
        traverse_recursive(path, opts, 0);
        return;
    }
    struct stat info;
    char fullPath[PATH_MAX];
    DIR* dir = opendir(path);
    struct dirent* entry;

    if (dir == NULL)
    {
        perror("opendir");
        return;
    }

    while ((entry = readdir(dir)) != NULL)
    {
        if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0) continue;
        if (entry->d_name[0] == '.' && !opts.show_all) continue;
        snprintf(fullPath, PATH_MAX, "%s/%s", path, entry->d_name);
        stat(fullPath, &info);
        printUtils(fullPath, entry->d_name, &info, opts, 0);
    }
    closedir(dir);
}