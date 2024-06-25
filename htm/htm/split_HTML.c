#define _CRT_SECURE_NO_WARNINGS
#include "split_HTML.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_BUFFER_SIZE 4096
#define MAX_TAG_LENGTH 100

typedef struct {
    char **data;
    int size;
    int capacity;
} Stack;

void init_stack(Stack *s, int capacity) {
    s->data = (char **)malloc(sizeof(char *) * capacity);
    s->size = 0;
    s->capacity = capacity;
}

void push_stack(Stack *s, const char *tag) {
    if (s->size == s->capacity) {
        s->capacity *= 2;
        s->data = (char **)realloc(s->data, sizeof(char *) * s->capacity);
    }
    s->data[s->size] = strdup(tag);
    s->size++;
}

char* pop_stack(Stack *s) {
    if (s->size == 0) return NULL;
    s->size--;
    return s->data[s->size];
}

int is_open_tag(const char *tag) {
    const char *open_tags[] = {"<p>", "<b>", "<strong>", "<i>", "<ul>", "<ol>", "<div>", "<span>"};
    for (int i = 0; i < sizeof(open_tags) / sizeof(open_tags[0]); i++) {
        if (strcmp(tag, open_tags[i]) == 0) {
            return 1;
        }
    }
    return 0;
}

int is_close_tag(const char *tag) {
    const char *close_tags[] = {"</p>", "</b>", "</strong>", "</i>", "</ul>", "</ol>", "</div>", "</span>"};
    for (int i = 0; i < sizeof(close_tags) / sizeof(close_tags[0]); i++) {
        if (strcmp(tag, close_tags[i]) == 0) {
            return 1;
        }
    }
    return 0;
}

void close_open_tags(char *output, int *len_output, Stack *stack) {
    for (int i = stack->size - 1; i >= 0; i--) {
        char close_tag[MAX_TAG_LENGTH] = "";
        strcpy(close_tag, stack->data[i]);
        if (close_tag[1] != '/') {
            for (int j = strlen(close_tag); j > 0; j--) {
                close_tag[j] = close_tag[j - 1];
            }
            close_tag[1] = '/';
        }
        strcpy(output + *len_output, close_tag);
        *len_output += strlen(close_tag);
    }
}

void open_closed_tags(char *output, int *len_output, Stack *stack) {
    for (int i = 0; i < stack->size; i++) {
        strcpy(output + *len_output, stack->data[i]);
        *len_output += strlen(stack->data[i]);
    }
}

void write_to_file(char *output, int *len_output, int *number_html, Stack *stack, Stack *open_stack) {
    char name[100];
    snprintf(name, sizeof(name), "HTML-%d.html", (*number_html)++);
    FILE *qw = fopen(name, "w");
    if (qw) {
        fputs(output, qw);
        fclose(qw);
    }
    memset(output, 0, MAX_BUFFER_SIZE);
    *len_output = 0;
    open_closed_tags(output, len_output, open_stack);
}

int split_HTML(FILE *html, int max_len) {
    fseek(html, 0, SEEK_SET);
    char ch;
    char output[MAX_BUFFER_SIZE] = "";
    int len_output = 0;
    Stack stack;
    Stack open_stack;
    init_stack(&stack, 10);
    init_stack(&open_stack, 10);
    char tag[MAX_TAG_LENGTH] = "";
    int index_tag = 0;
    int is_tag_open = 0;
    int number_html = 0;

    while (1) {
        ch = fgetc(html);
        if (ch == EOF) {
            close_open_tags(output, &len_output, &stack);
            write_to_file(output, &len_output, &number_html, &stack, &open_stack);
            break;
        } else if (ch == '<') {
            is_tag_open = 1;
            tag[index_tag++] = ch;
        } else if (ch == '>') {
            is_tag_open = 0;
            tag[index_tag++] = ch;
            tag[index_tag] = '\0';

            if (is_open_tag(tag)) {
                push_stack(&stack, tag);
            } else if (is_close_tag(tag)) {
                char *last_open_tag = pop_stack(&stack);
                if (last_open_tag) {
                    free(last_open_tag);
                }
            }

            if (len_output + strlen(tag) + stack.size * MAX_TAG_LENGTH < max_len) {
                strcpy(output + len_output, tag);
                len_output += strlen(tag);
            } else {
                close_open_tags(output, &len_output, &stack);
                write_to_file(output, &len_output, &number_html, &stack, &open_stack);
                strcpy(output + len_output, tag);
                len_output += strlen(tag);
            }
            memset(tag, 0, sizeof(tag));
            index_tag = 0;
        } else if (is_tag_open) {
            tag[index_tag++] = ch;
        } else {
            output[len_output++] = ch;
            if (len_output + stack.size * MAX_TAG_LENGTH >= max_len) {
                close_open_tags(output, &len_output, &stack);
                write_to_file(output, &len_output, &number_html, &stack, &open_stack);
            }
        }
    }
    for (int i = 0; i < stack.size; i++) {
        free(stack.data[i]);
    }
    free(stack.data);
    for (int i = 0; i < open_stack.size; i++) {
        free(open_stack.data[i]);
    }
    free(open_stack.data);
    return 1;
}

