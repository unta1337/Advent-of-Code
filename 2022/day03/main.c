#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

#define BUF_LEN 200

#define ANSWER_TYPE int
#define ANSWER_FORMAT "%d"

ANSWER_TYPE part_one(size_t line_number, char* line, size_t line_len) {
    size_t len = line_len / 2;

    size_t lookup_first[128] = { 0, };
    size_t lookup_second[128] = { 0, };

    for (size_t i = 0; i < line_len; i++) {
        if (i < len) {
            lookup_first[line[i]]++;
        } else {
            lookup_second[line[i]]++;
        }
    }

    for (char c = 'a'; c <= 'z'; c++) {
        if (lookup_first[c] > 0 && lookup_second[c]) {
            return c - 'a' + 1;
        }
    }
    for (char c = 'A'; c <= 'Z'; c++) {
        if (lookup_first[c] > 0 && lookup_second[c]) {
            return c - 'A' + 27;
        }
    }

    return 0;
}

ANSWER_TYPE part_two(size_t line_number, char* line, size_t line_len) {
    static size_t lookups[3][128] = { 0, };
    if ((line_number - 1) % 3 == 0) {
        memset(lookups, 0, sizeof(lookups));
    }

    for (size_t i = 0; i < line_len; i++) {
        lookups[(line_number - 1) % 3][line[i]]++;
    }

    if ((line_number - 1) % 3 == 2) {
        for (char c = 'a'; c <= 'z'; c++) {
            if (lookups[0][c] > 0 && lookups[1][c] > 0 && lookups[2][c] > 0) {
                return c - 'a' + 1;
            }
        }
        for (char c = 'A'; c <= 'Z'; c++) {
            if (lookups[0][c] > 0 && lookups[1][c] > 0 && lookups[2][c] > 0) {
                return c - 'A' + 27;
            }
        }
    }

    return 0;
}

void solve(const char* input_path) {
    // read input file.
    FILE* fp = fopen(input_path, "r");
    if (fp == NULL) {
        fprintf(stderr, "Error: could not open input file \"%s\": %s.\n", input_path, strerror(errno));
        exit(errno);
    }

    // load input file into buffer line-by-line.
    char buffer[BUF_LEN] = "";
    size_t line_count = 0;

    ANSWER_TYPE answer = 0;

    for (size_t line_count = 1; fgets(buffer, BUF_LEN, fp) != NULL; line_count++) {
        // remove line feed character.
        size_t len = strlen(buffer);
        buffer[--len] = '\0';

#ifndef PART_TWO
        ANSWER_TYPE answer_line = part_one(line_count, buffer, len);
#else
        ANSWER_TYPE answer_line = part_two(line_count, buffer, len);
#endif

        answer += answer_line;
    }

    printf(ANSWER_FORMAT "\n", answer);

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
