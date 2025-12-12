#ifndef OPTIONS_H
#define OPTIONS_H
#include <stdbool.h>
typedef struct {
    bool recursive;
    bool human_readable;
    bool show_size;
} Options;
#endif