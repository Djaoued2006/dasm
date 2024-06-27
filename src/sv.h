#ifndef SV_H
#define SV_H

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

typedef struct {
    char *content;
    size_t size;
} View;

View view_init(char *content) {
    return (View) {
        .content = content,
        .size = strlen(content),
    };
}

char *view_to_cstr(View view) {
    if (view.size == 0) return NULL;
    char *content = (char *)malloc(sizeof(char) * (view.size + 1));
    memcpy(content, view.content, view.size);
    content[view.size] = '\0';
    return content;
}

#endif