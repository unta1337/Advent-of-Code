#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <math.h>
#include <ctype.h>

#define LEN_LINES 4000
#define LEN_LINE 2000

#define ANSWER_TYPE int
#define ANSWER_FORMAT "%d"

typedef struct {
    char str[LEN_LINE];
    size_t len;
} string_t;

size_t part_one(string_t* lines, size_t len_lines) {
    size_t times[LEN_LINE] = { 0, };
    size_t time_count = 0;

    size_t dists[LEN_LINE] = { 0, };
    size_t dist_count = 0;

    for (size_t i = 0; i < len_lines; i++) {
        char* buf = lines[i].str;

        char* token = strtok(buf, ":");
        buf += strlen(token) + 1;

        while (*buf != '\0' && *buf == ' ') {
            buf++;
        }

        while ((token = strtok(buf, " ")) != NULL) {
            buf += strlen(token) + 1;
            while (*buf != '\0' && *buf == ' ') {
                buf++;
            }

            size_t temp = 0;
            sscanf(token, "%zu", &temp);

            if (i == 0) {
                times[time_count++] = temp;
            } else {
                dists[dist_count++] = temp;
            }
        }
    }

    size_t answer = 1;
    for (size_t i = 0; i < time_count; i++) {
        size_t T = times[i];
        size_t D = dists[i];

        size_t res = 0;
        for (size_t t = 0; t <= T; t++) {
            size_t d = t * (T - t);
            res += d > D;
        }

        answer *= res;
    }

    return answer;
}

size_t part_two(string_t* lines, size_t len_lines) {
    size_t times[LEN_LINE] = { 0, };
    size_t time_count = 0;

    size_t dists[LEN_LINE] = { 0, };
    size_t dist_count = 0;

    for (size_t i = 0; i < len_lines; i++) {
        char* buf = lines[i].str;

        size_t temp = 0;
        while (*buf != '\0') {
            if (isdigit(*buf)) {
                temp *= 10;
                temp += *buf - '0';
            }
            buf++;
        }

        if (i == 0) {
            times[time_count++] = temp;
        } else {
            dists[dist_count++] = temp;
        }
    }

    size_t answer = 1;
    for (size_t i = 0; i < time_count; i++) {
        size_t T = times[i];
        size_t D = dists[i];

        size_t res = 0;
        for (size_t t = 0; t <= T; t++) {
            size_t d = t * (T - t);
            res += d > D;
        }

        answer *= res;
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
