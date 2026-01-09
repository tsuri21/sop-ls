#pragma once

#include <stdbool.h>

typedef struct {
    bool recursive;
    int r_counter;
    bool human_readable;
    bool show_size;
    char* unit;
    bool show_content;
    int c_counter;
    bool show_all;
    bool show_permissions;
    bool show_last_modified;
} Options;