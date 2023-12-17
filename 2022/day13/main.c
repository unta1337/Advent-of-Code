#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <ctype.h>

#include "list.h"

#define LEN_LINES 2000
#define LEN_LINE 2000

typedef struct {
    char* str;
    size_t len;
} string_t;

struct box {
    bool is_list;
    union {
        size_t num;
        struct list* lst;
    } value;
};

struct box* box_create_as_number(size_t num) {
    struct box* bx = (struct box*)calloc(1, sizeof(struct box));

    bx->is_list = false;
    bx->value.num = num;

    return bx;
}

struct box* box_create_as_list(struct list* lst) {
    struct box* bx = (struct box*)calloc(1, sizeof(struct box));

    bx->is_list = true;
    bx->value.lst = lst;

    return bx;
}

size_t parse_number(char** str) {
    size_t ret = 0;
    while (isdigit(**str)) {
        ret *= 10;
        ret += **str - '0';
        *str += 1;
    }

    return ret;
}

struct list* gen_list(char** str) {
    struct list* lst = list_create(sizeof(struct box*));

    while (**str != '\0') {
        if (isdigit(**str)) {
            size_t num = parse_number(str);
            struct box* temp = box_create_as_number(num);
            list_push(lst, &temp);
        } else if (**str == '[') {
            *str += 1;
            struct list* temp_list = gen_list(str);
            struct box* temp = box_create_as_list(temp_list);
            list_push(lst, &temp);
        } else if (**str == ']') {
            *str += 1;
            return lst;
        } else {
            *str += 1;
        }
    }

    return lst;
}

void print_list(struct list* lst) {
    for (size_t i = 0; i < lst->size; i++) {
        struct box* bx = NULL;
        list_get(lst, &bx, i);

        if (bx->is_list) {
            printf("{ ");
            print_list(bx->value.lst);
            printf(" }");
        } else {
            printf("%zu", bx->value.num);
        }
        if (i < lst->size - 1) {
            printf(", ");
        }
    }
}

int comp_box(struct box* p, struct box* q);
int comp_list(struct list* p, struct list* q);
int comp_list_(const void* p, const void* q);

int comp_list_(const void* p, const void* q) {
    return comp_list(*(struct list**)p, *(struct list**)q);
}

void print_box(struct box* bx) {
    printf("%s", bx->is_list ? "list" : "number");
    printf("[");
    if (bx->is_list) {
        print_list(bx->value.lst);
    } else {
        printf("%zu", bx->value.num);
    }
    printf("]");
}

int comp_box(struct box* p, struct box* q) {
    int res = 0;
    if (!p->is_list && !q->is_list) {
        res = (p->value.num > q->value.num) - (p->value.num < q->value.num);
    } else if (p->is_list && q->is_list) {
        res = comp_list(p->value.lst, q->value.lst);
    } else if (p->is_list && !q->is_list) {
        if (true) {
            struct list* temp_list = list_create(sizeof(struct box*));
            struct box* temp = box_create_as_number(q->value.num);
            list_push(temp_list, &temp);
            res = comp_list(p->value.lst, temp_list);
        } else {
            if (p->value.lst->size > 1) {
                res = 1;
            } else {
                size_t* elem = NULL;
                list_get(p->value.lst, &elem, 0);
                res = (*elem > q->value.num) - (*elem < q->value.num);
            }
        }
    } else if (!p->is_list && q->is_list) {
        if (true) {
            struct list* temp_list = list_create(sizeof(struct box*));
            struct box* temp = box_create_as_number(p->value.num);
            list_push(temp_list, &temp);
            res = comp_list(temp_list, q->value.lst);
        } else {
            if (q->value.lst->size > 1) {
                res = -1;
            } else {
                size_t* elem = NULL;
                list_get(q->value.lst, &elem, 0);
                res = (p->value.num > *elem) - (p->value.num < *elem);
            }
        }
    }

    return res;
}

int comp_list(struct list* p, struct list* q) {
    size_t iter = p->size < q->size ? p->size : q->size;
    for (size_t i = 0; i < iter; i++) {
        struct box* r = NULL;
        list_get(p, &r, i);

        struct box* s = NULL;
        list_get(q, &s, i);

        int res = comp_box(r, s);

        if (res != 0) {
            return res;
        }
    }

    return (p->size > q->size) - (p->size < q->size);
}

size_t part_one(string_t* lines, size_t len_lines) {
    struct list* lsts[LEN_LINES] = { 0, };
    size_t lst_count = 0;

    for (size_t i = 0; i < len_lines; i++) {
        if (lines[i].len == 0) {
            continue;
        }

        char* buf = lines[i].str;
        struct list* lst = gen_list(&buf);
        lsts[lst_count++] = lst;
    }

    size_t answer = 0;
    for (size_t i = 0; i < lst_count; i += 2) {
        int res = comp_list(lsts[i], lsts[i + 1]);
        answer += (res == -1) * (i / 2 + 1);
    }

    return answer;
}

size_t part_two(string_t* lines, size_t len_lines) {
    struct list* lsts[LEN_LINES] = { 0, };
    size_t lst_count = 0;

    for (size_t i = 0; i < len_lines; i++) {
        if (lines[i].len == 0) {
            continue;
        }

        char* buf = lines[i].str;
        struct list* lst = gen_list(&buf);
        lsts[lst_count++] = lst;
    }

    char* packet1_str = "[[2]]";
    struct list* packet1 = gen_list(&packet1_str);
    lsts[lst_count++] = packet1;

    char* packet2_str = "[[6]]";
    struct list* packet2 = gen_list(&packet2_str);
    lsts[lst_count++] = packet2;

    qsort(lsts, lst_count, sizeof(struct list*), comp_list_);

    size_t answer = 1;
    for (size_t i = 0; i < lst_count; i++) {
        if (lsts[i] == packet1 || lsts[i] == packet2) {
            answer *= i + 1;
        }
    }

    return answer;
}

void solve(const char* input_path) {
    // read input file.
    FILE* fp = fopen(input_path, "r");
    if (fp == NULL) {
        fprintf(stderr, "Error: could not open input file \"%s\": %s.\n", input_path, strerror(errno));
        exit(errno);
    }

    string_t lines[LEN_LINES] = { 0, };
    for (size_t i = 0; i < LEN_LINES; i++) {
        lines[i].str = (char*)calloc(LEN_LINE, sizeof(char));
    }
    size_t line_count = 0;

    // load input file into buffer line-by-line.
    for (line_count = 0; fgets(lines[line_count].str, LEN_LINE, fp) != NULL; line_count++) {
        // remove line feed character.
        lines[line_count].len = strlen(lines[line_count].str);
        lines[line_count].str[--lines[line_count].len] = '\0';
    }

    // print answer.
#ifndef PART_TWO
    printf("%zu\n", part_one(lines, line_count));
#else
    printf("%zu\n", part_two(lines, line_count));
#endif

    fclose(fp);
}

int main(int argc, char** argv) {
    if (argc <= 1) {
        fprintf(stderr, "Error: input file path not provided.\n");
        return 1;
    }

    solve(argv[1]);

    return 0;
}
