#include "options.h"
#include <sys/stat.h>
#include <dirent.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "output_utils.h"

typedef struct linked_list{
    char fullPath[4096];
    char fileName[256];
    struct stat info;
    int depth;
    struct linked_list* next;
}Node;

Node* head;

static int compare_full_paths(const char* s1, const char* s2){
    const size_t min = strlen(s1) > strlen(s2) ? strlen(s2) : strlen(s1);
    for(int i = 0; i < min; i++){
        char c1 = s1[i];
        char c2 = s2[i];
        if (c1 >= 'A' && c1 <= 'Z') c1 += 'a' - 'A';
        if (c2 >= 'A' && c2 <= 'Z') c2 += 'a' - 'A';
        if (c1 < c2) return -1;
        if (c1 > c2) return 1;
    }
    return 0;
}

static void insert(const char* fullPath, const char* fileName, const struct stat* info, const int depth, const bool descending)
{
    Node* newNode = malloc(sizeof(Node));
    strncpy(newNode->fullPath, fullPath, sizeof(newNode->fullPath));
    strncpy(newNode->fileName, fileName, sizeof(newNode->fileName));
    newNode->info = *info;
    newNode->depth = depth;
    newNode->next = NULL;

    Node* current = head;
    while (current->next != NULL && ((compare_full_paths(current->next->fullPath, newNode->fullPath) <= 0 && !descending) ||
        (compare_full_paths(current->next->fullPath, newNode->fullPath) >= 0 && descending))){
        current = current->next;
    }
    newNode->next = current->next;
    current->next = newNode;
}

static void print_and_free(const Options opts){
    Node* current = head->next;
    while (current != NULL){
        print_utils(current->fullPath, current->fileName, &current->info, opts, current->depth);
        Node* temp = current;
        current = current->next;
        free(temp);
    }
    free(head);
}

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

        insert(fullPath, entry->d_name, &info, depth, opts.descending_order);
        if (S_ISDIR(info.st_mode)) traverse_recursive(fullPath, opts, depth+1);
    }
    closedir(dir);
}

void traverse_directory(const char* path, const Options opts)
{
    head = malloc(sizeof(Node));
    strncpy(head->fullPath, path, sizeof(head->fullPath));
    head->next = NULL;
    if (opts.recursive)
    {
        traverse_recursive(path, opts, 0);
        print_and_free(opts);
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

        insert(fullPath, entry->d_name, &info, 0, opts.descending_order);
    }
    closedir(dir);
    print_and_free(opts);
}