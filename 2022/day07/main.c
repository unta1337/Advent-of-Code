#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <ctype.h>

#define MIN(x, y) ((x) < (y) ? (x) : (y))

#define LEN_LINES 4000
#define LEN_LINE 2000

typedef struct {
    char str[LEN_LINE];
    size_t len;
} string_t;

size_t part_one(string_t* lines, size_t len_lines) {
    size_t stk[LEN_LINE] = { 0, };
    size_t stk_count = 0;

    size_t res[LEN_LINE] = { 0, };
    size_t res_count = 0;

    for (size_t i = 0; i < len_lines; i++) {
        if (strncmp(lines[i].str, "$ cd", 4) == 0) {
            if (strncmp(lines[i].str + 5, "..", 2) == 0) {
                size_t top = stk[stk_count - 1];
                stk[--stk_count] = 0;
                res[res_count++] = top;

                stk[stk_count - 1] += top;
            } else {
                stk_count++;
            }
        } else if (isdigit(lines[i].str[0])) {
            size_t size = 0;
            sscanf(lines[i].str, "%zu", &size);
            stk[stk_count - 1] += size;
        }
    }

    while (stk_count > 0) {
        size_t top = stk[stk_count - 1];
        stk[--stk_count] = 0;
        res[res_count++] = top;

        stk[stk_count - 1] += top;
    }

    size_t answer = 0;
    for (size_t i = 0; i < res_count; i++) {
        if (res[i] <= 100000) {
            answer += res[i];
        }
    }

    return answer;
}

size_t part_two(string_t* lines, size_t len_lines) {
    size_t stk[LEN_LINE] = { 0, };
    size_t stk_count = 0;

    size_t res[LEN_LINE] = { 0, };
    size_t res_count = 0;

    for (size_t i = 0; i < len_lines; i++) {
        if (strncmp(lines[i].str, "$ cd", 4) == 0) {
            if (strncmp(lines[i].str + 5, "..", 2) == 0) {
                size_t top = stk[stk_count - 1];
                stk[--stk_count] = 0;
                res[res_count++] = top;

                stk[stk_count - 1] += top;
            } else {
                stk_count++;
            }
        } else if (isdigit(lines[i].str[0])) {
            size_t size = 0;
            sscanf(lines[i].str, "%zu", &size);
            stk[stk_count - 1] += size;
        }
    }

    while (stk_count > 0) {
        size_t top = stk[stk_count - 1];
        stk[--stk_count] = 0;
        res[res_count++] = top;

        stk[stk_count - 1] += top;
    }

    size_t total_usage = res[res_count - 1];
    size_t free_space = 70000000 - total_usage;

    size_t answer = 1 << 32 - 1;
    for (size_t i = 0; i < res_count; i++) {
        if (free_space + res[i] >= 30000000) {
            answer = MIN(answer, res[i]);
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
