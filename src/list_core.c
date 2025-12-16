#include "options.h"
#include <sys/stat.h>
#include <dirent.h>
#include <stdio.h>
#include <string.h>

#include "output_utils.h"

static void traverse_recursive(const char* path, Options opts, int depth)
{
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
        snprintf(fullPath, PATH_MAX, "%s/%s", path, entry->d_name);
        stat(fullPath, &info);
        if (S_ISDIR(info.st_mode))
        {
            printDirectoryWithDepth(entry->d_name, opts, depth);
            traverse_recursive(fullPath, opts, depth+1);
        }else
        {
            printFileWithDepth(entry->d_name, opts, depth);
        }
    }
    closedir(dir);
}
void traverse_directory(const char* path, Options opts)
{
    if (opts.recursive) traverse_recursive(path, opts, 0);
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
        snprintf(fullPath, PATH_MAX, "%s/%s", path, entry->d_name);
        stat(fullPath, &info);
        if (S_ISDIR(info.st_mode))
        {
            printDirectory(entry->d_name, opts);
        }else if (S_ISREG(info.st_mode))
        {
            printFile(entry->d_name, opts);
        }
    }
    closedir(dir);
}