#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <ctype.h>

#define LEN_LINES 800
#define LEN_LINE 400

#define ANSWER_TYPE int
#define ANSWER_FORMAT "%d"

typedef struct {
    char str[LEN_LINE];
    size_t len;
} string_t;

typedef struct {
    char elems[LEN_LINES];
    size_t count;
} stack_t;

void stack_push(stack_t* ths, char elem) {
    ths->elems[ths->count++] = elem;
}

void stack_pop(stack_t* ths) {
    if (ths->count > 0) {
        ths->count--;
    }
}

char stack_top(stack_t* ths) {
    if (ths->count == 0) {
        return '\0';
    }

    return ths->elems[ths->count - 1];
}

size_t parse_number(char** str) {
    while (**str != '\0' && !isdigit(**str)) {
        (*str)++;
    }

    size_t ret = 0;
    while (**str != '\0' && isdigit(**str)) {
        ret *= 10;
        ret += **str - '0';

        (*str)++;
    }

    return ret;
}

ANSWER_TYPE part_one(string_t* lines, size_t len_lines) {
    stack_t stacks[LEN_LINES] = { 0, };
    size_t cols = (lines[0].len + 1) / 4;
    size_t rows = 0;

    size_t i = 0;
    for (; i < len_lines; i++) {
        if (lines[i].len == 0) {
            rows = (i++) - 1;
            break;
        }
    }

    for (size_t r = rows - 1; r < rows; r--) {
        for (size_t c = 0; c < cols; c++) {
            if (isupper(lines[r].str[c * 4 + 1])) {
                stack_push(&stacks[c + 1], lines[r].str[c * 4 + 1]);
            }
        }
    }

    for (; i < len_lines; i++) {
        char* buf = lines[i].str;
        size_t count = parse_number(&buf);
        size_t from = parse_number(&buf);
        size_t to = parse_number(&buf);

        for (size_t c = 0; c < count; c++) {
            stack_push(&stacks[to],
                       stack_top(&stacks[from]));
            stack_pop(&stacks[from]);
        }
    }

    for (size_t c = 1; c <= cols; c++) {
        printf("%c", stack_top(&stacks[c]));
    }
    printf("\n");

    return 0;
}

ANSWER_TYPE part_two(string_t* lines, size_t len_lines) {
    stack_t stacks[LEN_LINES] = { 0, };
    size_t cols = (lines[0].len + 1) / 4;
    size_t rows = 0;

    size_t i = 0;
    for (; i < len_lines; i++) {
        if (lines[i].len == 0) {
            rows = (i++) - 1;
            break;
        }
    }

    for (size_t r = rows - 1; r < rows; r--) {
        for (size_t c = 0; c < cols; c++) {
            if (isupper(lines[r].str[c * 4 + 1])) {
                stack_push(&stacks[c + 1], lines[r].str[c * 4 + 1]);
            }
        }
    }

    for (; i < len_lines; i++) {
        char* buf = lines[i].str;
        size_t count = parse_number(&buf);
        size_t from = parse_number(&buf);
        size_t to = parse_number(&buf);

        stack_t temp = { 0, };

        for (size_t c = 0; c < count; c++) {
            stack_push(&temp, stack_top(&stacks[from]));
            stack_pop(&stacks[from]);
        }

        for (size_t c = 0; c < count; c++) {
            stack_push(&stacks[to],
                       stack_top(&temp));
            stack_pop(&temp);
        }
    }

    for (size_t c = 1; c <= cols; c++) {
        printf("%c", stack_top(&stacks[c]));
    }
    printf("\n");

    return 0;
}

void solve(const char* input_path) {
    // read input file.
    FILE* fp = fopen(input_path, "r");
    if (fp == NULL) {
        fprintf(stderr, "Error: could not open input file \"%s\": %s.\n", input_path, strerror(errno));
        exit(errno);
    }

    string_t lines[LEN_LINES] = { 0, };
    size_t line_count = 0;

    // load input file into buffer line-by-line.
    for (line_count = 0; fgets(lines[line_count].str, LEN_LINE, fp) != NULL; line_count++) {
        // remove line feed character.
        lines[line_count].len = strlen(lines[line_count].str);
        lines[line_count].str[--lines[line_count].len] = '\0';
    }

    // print answer.
#ifndef PART_TWO
    printf(ANSWER_FORMAT "\n", part_one(lines, line_count));
#else
    printf(ANSWER_FORMAT "\n", part_two(lines, line_count));
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
