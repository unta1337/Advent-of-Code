#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

#define LEN_LINES 4000
#define LEN_LINE 2000

#define ANSWER_TYPE int
#define ANSWER_FORMAT "%d"

typedef struct {
    char str[LEN_LINE];
    size_t len;
} string_t;

typedef struct {
    size_t begin;
    size_t end;
} range_t;

ANSWER_TYPE part_one(string_t* lines, size_t len_lines) {
    ANSWER_TYPE ret = 0;
    for (size_t i = 0; i < len_lines; i++) {
        char* buf = lines[i].str;

        range_t p = { 0, };
        range_t q = { 0, };

        char* token = strtok(buf, "-,");
        buf += strlen(token) + 1;
        p.begin = strtol(token, NULL, 10);

        token = strtok(buf, "-,");
        buf += strlen(token) + 1;
        p.end = strtol(token, NULL, 10);

        token = strtok(buf, "-,");
        buf += strlen(token) + 1;
        q.begin = strtol(token, NULL, 10);

        token = strtok(buf, "-,");
        buf += strlen(token) + 1;
        q.end = strtol(token, NULL, 10);

        ret += (p.begin <= q.begin && q.end <= p.end)
               || (q.begin <= p.begin && p.end <= q.end);
    }

    return ret;
}

ANSWER_TYPE part_two(string_t* lines, size_t len_lines) {
    ANSWER_TYPE ret = 0;
    for (size_t i = 0; i < len_lines; i++) {
        char* buf = lines[i].str;

        range_t p = { 0, };
        range_t q = { 0, };

        char* token = strtok(buf, "-,");
        buf += strlen(token) + 1;
        p.begin = strtol(token, NULL, 10);

        token = strtok(buf, "-,");
        buf += strlen(token) + 1;
        p.end = strtol(token, NULL, 10);

        token = strtok(buf, "-,");
        buf += strlen(token) + 1;
        q.begin = strtol(token, NULL, 10);

        token = strtok(buf, "-,");
        buf += strlen(token) + 1;
        q.end = strtol(token, NULL, 10);

        ret += (p.begin <= q.begin && q.begin <= p.end)
               || (q.begin <= p.begin && p.begin <= q.end);
    }

    return ret;
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
