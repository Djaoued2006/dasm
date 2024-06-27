#ifndef UTILS_H
#define UTILS_H

#include <stdio.h>
#include <stdlib.h>

int isnumber(char c) {
    return (c <= '9' && c >= '0');
}

int ischar(char c) {
    return ((c == '_') || (c <= 'Z' && c >= 'A') || (c <= 'z' && c >= 'a'));
}

int isnewline(char c) {
    return (c == '\n');
}

int istab(char c) {
    return (c == '\t');
}

int iswhitespace(char c) {
    return (c == ' ');
}

int issemicolon(char c) {
    return (c == ';');
}

int iscomma(char c) {
    return (c == ',');
}

int isquote(char c) {
    return (c == '"');
}

char *get_file_content(const char * const filename) {
    FILE *fp = fopen(filename, "r");

    if (fp == NULL) {
        fprintf(stderr, "Couldn't open file %s\n", filename);
        exit(1);
    }

    fseek(fp, 0, SEEK_END);
    size_t size = ftell(fp);
    fseek(fp, 0, SEEK_SET);

    char *content = (char *)malloc(sizeof(char) * (size + 1));

    if (fread(content, sizeof(char), size, fp) != size) {
        fprintf(stderr, "Couldn't read the file, %s\n", filename);
        free(content);
        exit(1);
    }   

    content[size] = '\0';

    fclose(fp);
    
    return content;
}

#endif