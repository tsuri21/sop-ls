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

static void insert(Node* head, const char* fullPath, const char* fileName, const struct stat* info, const int depth)
{
    Node* newNode = malloc(sizeof(Node));
    strncpy(newNode->fullPath, fullPath, sizeof(newNode->fullPath));
    strncpy(newNode->fileName, fileName, sizeof(newNode->fileName));
    newNode->info = *info;
    newNode->depth = depth;
    newNode->next = NULL;

    Node* current = head;
    //important: don't use strcmp because it is not case-sensitive, instead use staircase
    while (current->next != NULL && strcasecmp(current->next->fullPath, newNode->fullPath) < 0){
        current = current->next;
    }
    newNode->next = current->next;
    current->next = newNode;
}

static void print_and_free(Node* head, const Options opts){
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

        insert(head, fullPath, entry->d_name, &info, depth);
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
        print_and_free(head, opts);
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
        //instead of printing, it should insert in the linkedList
        insert(head, fullPath, entry->d_name, &info, 0);
    }
    closedir(dir);
    print_and_free(head, opts);
}